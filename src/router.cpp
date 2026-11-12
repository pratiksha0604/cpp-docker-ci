#include "router.hpp"
#include <sstream>

static std::string page(const std::string& title, const std::string& body) {
    return "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\">"
           "<title>" + title + "</title></head><body>"
           "<nav><a href=\"/\">Home</a> | <a href=\"/about\">About</a> | "
           "<a href=\"/projects\">Projects</a></nav><h1>" + title + "</h1>" +
           body + "</body></html>";
}

std::string render_route(const std::string& path) {
    if (path == "/")         return page("Home", "<p>Welcome! Built with C++ and shipped via Docker CI.</p>");
    if (path == "/about")    return page("About", "<p>Pratiksha Bhende - web developer.</p>");
    if (path == "/projects") return page("Projects", "<p>C++ CI/CD pipeline project.</p>");
    if (path == "/health")   return "OK";
    return "";
}

std::string parse_path(const std::string& request) {
    std::istringstream in(request);
    std::string method, path, version;
    in >> method >> path >> version;
    if (method != "GET" || path.empty() || path[0] != '/') return "";
    return path;
}
