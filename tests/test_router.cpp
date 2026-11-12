#include "../src/router.hpp"
#include <cstdlib>
#include <iostream>

#define CHECK(cond) do { if (!(cond)) { std::cerr << "FAILED: " #cond " (line " << __LINE__ << ")\n"; return EXIT_FAILURE; } } while (0)

int main() {
    CHECK(parse_path("GET /about HTTP/1.1\r\n") == "/about");
    CHECK(parse_path("POST /about HTTP/1.1\r\n").empty());
    CHECK(parse_path("garbage").empty());
    CHECK(render_route("/").find("Home") != std::string::npos);
    CHECK(render_route("/about").find("About") != std::string::npos);
    CHECK(render_route("/projects").find("Projects") != std::string::npos);
    CHECK(render_route("/health") == "OK");
    CHECK(render_route("/nope").empty());
    std::cout << "All tests passed\n";
    return EXIT_SUCCESS;
}
