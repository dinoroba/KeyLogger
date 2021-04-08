#include "Communication.h"

void ReadUsername(SOCKET connection_socket){

    int32_t size_of_message;
    char * size_data = (char *) &size_of_message;
    int missing = sizeof(size_of_message);

    do{
        int bytes_received = recv(connection_socket, size_data, missing, 0);

        if(bytes_received < 0){
            std::cout << "ERROR" << std::endl;
            closesocket(connection_socket);
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
            std::cout << "ERROR" << std::endl;
            closesocket(connection_socket);
        }
        message_size -= bytes_received;
        currBytes += bytes_received;
    }while(message_size > 0);

    username[currBytes] = '\0';
}


int RecvBuffer(SOCKET s, char* buffer, int bufferSize, int chunkSize) {
    int i = 0;
    while (i < bufferSize) {
        const int l = recv(s, &buffer[i], __min(chunkSize, bufferSize - i), 0);
        if (l < 0) { return l; } // this is an error
        i += l;
        std::cout << i << std::endl;
        std::cout << bufferSize << std::endl;
    }
    return i;
}

int64_t RecvFile(SOCKET s, const std::string& fileName, int chunkSize) {
    std::ofstream file(fileName, std::ofstream::binary);
    if (file.fail()) { return -1; }

    std::cout << "TOU AQUI" << std::endl;
    int64_t fileSize;
    std::cout << "TOU AQUI" << std::endl;
    if (RecvBuffer(s, reinterpret_cast<char*>(&fileSize),
            sizeof(fileSize), chunksize_buffer) != sizeof(fileSize)) {
        return -2;
    }
    std::cout << "TOU AQUI" << std::endl;

    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t i = fileSize;
    printf("SOU O FILE SIZE %d\n", i);
    while (i != 0) {
        const int r = RecvBuffer(s, buffer, (int)__min(i, (int64_t)chunkSize), chunksize_buffer);
        if ((r < 0) || !file.write(buffer, r)) { errored = true; break; }
        i -= r;
    }
    delete[] buffer;

    file.close();

    return errored ? -3 : fileSize;
}
