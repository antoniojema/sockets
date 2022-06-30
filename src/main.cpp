#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

int main(int argc, char** argv) {
    WSAData wsaData;

    // Initialize Winsock
    int i_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (i_res != 0) {
        printf("WSAStartup failed: %d\n", i_res);
        return 1;
    }

    // Set hints for connection
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    // Get address info
    addrinfo *result = nullptr;
    i_res = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (i_res != 0) {
        printf("getaddrinfo failed: %d\n", i_res);
        WSACleanup();
        return 1;
    }

    // Create socket
    addrinfo *ptr = result;
    SOCKET ConnectSocket = INVALID_SOCKET;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Connect to server.
    int iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    std::cout << "Hello world!" << std::endl;

    return 0;
}
