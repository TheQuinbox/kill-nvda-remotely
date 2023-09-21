cmake_minimum_required(VERSION 3.25.0)
project(kill_nvda_remotely)

include_directories(ini/src)

add_executable(client WIN32 client.c ini/src/ini.c)
target_link_libraries(client PRIVATE ws2_32)

add_executable(server WIN32 server.c ini/src/ini.c)
target_link_libraries(server PRIVATE ws2_32)
