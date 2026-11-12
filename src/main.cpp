#include "router.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    const char* env = std::getenv("PORT");   // Railway injects PORT
    int port = env ? std::atoi(env) : 8080;

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) { std::perror("socket"); return 1; }
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(server, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) { std::perror("bind"); return 1; }
    if (listen(server, 16) < 0) { std::perror("listen"); return 1; }
    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        int client = accept(server, nullptr, nullptr);
        if (client < 0) continue;
        char buf[4096] = {0};
        ssize_t n = read(client, buf, sizeof(buf) - 1);
        if (n > 0) {
            std::string body = render_route(parse_path(std::string(buf, n)));
            std::string status = body.empty() ? "404 Not Found" : "200 OK";
            if (body.empty()) body = "<h1>404 Not Found</h1>";
            std::string resp = "HTTP/1.1 " + status + "\r\nContent-Type: text/html\r\nContent-Length: " +
                               std::to_string(body.size()) + "\r\nConnection: close\r\n\r\n" + body;
            write(client, resp.c_str(), resp.size());
        }
        close(client);
    }
}
