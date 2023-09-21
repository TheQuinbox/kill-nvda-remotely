cmake_minimum_required(VERSION 3.25.0)
project(kill_nvda_remotely)

add_executable(client client.c)
set_target_properties(client PROPERTIES WIN32_EXECUTABLE TRUE LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
target_link_libraries(client PRIVATE ws2_32)

add_executable(server server.c)
set_target_properties(server PROPERTIES WIN32_EXECUTABLE TRUE LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
target_link_libraries(server PRIVATE ws2_32)
