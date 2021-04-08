#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#define chunksize_buffer 4096

#define chunksize_file 65536

void ReadUsername(SOCKET connection_socket);

int64_t GetFileSize(const std::string& fileName);

int RecvBuffer(SOCKET s, char* buffer, int bufferSize, int chunkSize);

int64_t RecvFile(SOCKET s, const std::string& fileName, int chunkSize);
