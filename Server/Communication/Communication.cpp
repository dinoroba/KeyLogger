#include "Communication.h"

std::string ReadUsername(SOCKET connection_socket){

    int32_t size_of_message;
    char * size_data = (char *) &size_of_message;
    int missing = sizeof(size_of_message);

    do{
        int bytes_received = recv(connection_socket, size_data, missing, 0);

        if(bytes_received < 0){
            closesocket(connection_socket);
            return "error__";
        }
        size_data += bytes_received;
        missing -= bytes_received;

    }while(missing > 0);

    
    int message_size = ntohl(size_of_message);
    char* username = new char[message_size+1];
    int currBytes = 0;
    int bytes_received;
    do{
        bytes_received = recv(connection_socket, &username[currBytes], message_size, 0);

        if(bytes_received < 0){
            closesocket(connection_socket);
            return "error__";
        }
        message_size -= bytes_received;
        currBytes += bytes_received;
    }while(message_size > 0);

    username[currBytes] = '\0';

    return std::string(username);
}

int RecvBuffer(SOCKET s, char* buffer, int bufferSize, int chunkSize) {
    int i = 0;
    while (i < bufferSize) {
        const int l = recv(s, &buffer[i], __min(chunkSize, bufferSize - i), 0);
        if (l < 0) { return l; } // this is an error
        i += l;
    }
    return i;
}

std::string RecvFile(SOCKET s, const std::string& fileName, int chunkSize) {
    std::ofstream file(fileName, std::ofstream::binary);
    std::cout << fileName << std::endl;
    if (file.fail()) { return "error opening file"; }

    int64_t fileSize;
    if (RecvBuffer(s, reinterpret_cast<char*>(&fileSize),
            sizeof(fileSize), chunksize_buffer) != sizeof(fileSize)) {
        return "error receiving filezise";
    }

    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t i = fileSize;
    while (i != 0) {
        const int r = RecvBuffer(s, buffer, (int)__min(i, (int64_t)chunkSize), chunksize_buffer);
        if ((r < 0) || !file.write(buffer, r)) { errored = true; break; }
        i -= r;
    }
    delete[] buffer;

    file.close();

    return errored ? "error receiving file" : "file received";
}

void ClientConnection(SOCKET connection_socket){
    
    std::string username = ReadUsername(connection_socket);

    if(username == "error__"){return;}

    std::string filename = username + "_logFile";
 
    std::cout << RecvFile(connection_socket, filename , chunksize_file) + " --->  username:  " + username << std::endl;;

    int shutdown_socket = shutdown(connection_socket, SD_SEND);
    if (shutdown_socket == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connection_socket);
    }

    closesocket(connection_socket);

}