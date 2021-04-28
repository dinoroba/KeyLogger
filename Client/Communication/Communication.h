#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

//Define here the port you will be using in the server side
#define DEFAULT_PORT "27017"

//Define here the ipaddress you will be using in the server side
#define DEFAULT_IP_ADDRESS "127.0.0.1"

#define chunksize_buffer 4096

#define chunksize_file 65536

#define DEFAULT_BUFLEN 512

void SendUsername(SOCKET connection_socket);

void CloseConnection(SOCKET connection_socket);

SOCKET SetUpConnectionSocket();

void Communicate_with_server(SOCKET connection_socket, std::string logFile);

int64_t GetFileSize(const std::string& fileName);

int SendBuffer(SOCKET connection_socket, const char* buffer, int bufferSize, int chunkSize);

int64_t SendFile(SOCKET connection_socket, const std::string& fileName, int chunkSize);
