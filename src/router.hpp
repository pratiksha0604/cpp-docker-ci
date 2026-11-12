#pragma once
#include <string>

// Returns the HTML body for a given route, or an empty string if unknown.
std::string render_route(const std::string& path);

// Parses the request line "GET /path HTTP/1.1" and returns the path ("" if invalid).
std::string parse_path(const std::string& request);
