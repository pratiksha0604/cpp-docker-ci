cmake_minimum_required(VERSION 3.16)
project(cpp_docker_ci LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(router src/router.cpp)
add_executable(server src/main.cpp)
target_link_libraries(server router)

enable_testing()
add_executable(test_router tests/test_router.cpp)
target_link_libraries(test_router router)
add_test(NAME router_tests COMMAND test_router)
