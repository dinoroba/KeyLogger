#define WIN32_LEAN_AND_MEAN

#include "Communication.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib

#define DEFAULT_BUFLEN 512

//Define here the port you will be using in the server side
#define DEFAULT_PORT "27017"

//Define here the ipaddress you will be using in the server side
#define DEFAULT_IP_ADDRESS "127.0.0.1"

void Communicate_with_server(SOCKET connection_socket, std::string logFile){

    SendUsername(connection_socket);

    SendFile(connection_socket, logFile, chunksize_file);

    CloseConnection(connection_socket);

}
    
SOCKET SetUpConnectionSocket(){

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return -1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_IP_ADDRESS, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return -1;
    }

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(result->ai_family, result->ai_socktype, 
        result->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    // Connect to server.
    iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(result);
        std::cout << "unable to connect to server" << std::endl;
        WSACleanup();
        return -1;
    }

    return ConnectSocket;

}

void CloseConnection(SOCKET connection_socket){

    if (shutdown(connection_socket, SD_SEND) == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
    }

    closesocket(connection_socket);
    WSACleanup();
}



void SendUsername(SOCKET connection_socket){

    char* username = std::getenv("USERNAME");
    int32_t usernameSize = htonl(strlen(username));
    char *dataUserSize = (char *)&usernameSize;
    int missing_bytes = sizeof(usernameSize);

    //Sends size of the username in a int32_t
    do{
        int bytes_sent = send(connection_socket, dataUserSize,missing_bytes, 0);
        if(bytes_sent < 0){
            std::cout << "ERROR" << std::endl;
        }
        else{
            missing_bytes -= bytes_sent;
            dataUserSize += bytes_sent;
        }
    }while(missing_bytes > 0);


    //Sends the username with the expected size
    int nBytes = strlen(username);
    std::cout << nBytes << std::endl;
    int bytes_sent;
    do{
        bytes_sent = send(connection_socket, username, nBytes, 0);

        if(bytes_sent < 0){
            std::cout << "ERROR" << std::endl;
        }
        else{
            nBytes -= bytes_sent;
            username += bytes_sent;
        }
    }while(nBytes > 0);
}



int64_t GetFileSize(const std::string& fileName) {
    FILE* f;
    if (fopen_s(&f, fileName.c_str(), "r") != 0) {
        return -1;
    }
    _fseeki64(f, 0, SEEK_END);
    const int64_t len = _ftelli64(f);
    fclose(f);
    return len;
}

int SendBuffer(SOCKET connection_socket, const char* buffer, int bufferSize, int chunkSize) {

    int i = 0;
    while (i < bufferSize) {
        const int l = send(connection_socket, &buffer[i], __min(chunkSize, bufferSize - i), 0);
        if (l < 0) { return l; } // this is an error
        i += l;
    }
    return i;
}

int64_t SendFile(SOCKET connection_socket, const std::string& fileName, int chunkSize) {

    const int64_t fileSize = GetFileSize(fileName);
    if (fileSize < 0) { 
        return -1; 
        }
    std::ifstream file(fileName, std::ifstream::binary);
    if (file.fail()) { 
        return -1; 
        }
    if (SendBuffer(connection_socket, reinterpret_cast<const char*>(&fileSize),
        sizeof(fileSize), chunksize_buffer) != sizeof(fileSize)) {
        return -2;
    }


    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t i = fileSize;
    while (i != 0) {
        const int64_t ssize = __min(i, (int64_t)chunkSize);
        if (!file.read(buffer, ssize)) { 
            errored = true; break; 
            }
        const int l = SendBuffer(connection_socket, buffer, (int)ssize,chunksize_buffer);
        if (l < 0) { 
            errored = true; 
            break; 
            }
        i -= l;
    }
    delete[] buffer;

    file.close();

    return errored ? -3 : fileSize;
}



