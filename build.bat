@echo off
g++ client.cpp -o client.exe -lws2_32
g++ server.cpp -o server.exe -lws2_32
pause
