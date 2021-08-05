cmake_minimum_required(VERSION 3.19)
project(communicate_hc)

set(CMAKE_CXX_STANDARD 11)

add_library(message OBJECT communicate/message/message.cpp)
add_library(host OBJECT communicate/host.cpp)
add_library(client OBJECT communicate/client.cpp)

target_link_libraries(host pthread)
target_link_libraries(client pthread)

target_link_libraries(host message)
target_link_libraries(client message)

add_executable(main_host main_host.cpp)
target_link_libraries(main_host host)
target_link_libraries(main_host message)

add_executable(main_client main_client.cpp)
target_link_libraries(main_client client)
target_link_libraries(main_client message)

target_precompile_headers(host PUBLIC communicate/safe_queue.hpp)
target_precompile_headers(client PUBLIC communicate/safe_queue.hpp)
