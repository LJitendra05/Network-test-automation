#include <iostream>
#include <winsock2.h>
#include "tcp_client.h"

int main() {

    WSADATA wsaData;

    int result = WSAStartup(
        MAKEWORD(2, 2),
        &wsaData
    );

    if (result != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    TcpClient client;

    if (client.connectToServer("127.0.0.1", 8080)) {
        std::cout << "Connected to server!\n";
    }

    client.disconnect();

    WSACleanup();

    return 0;
}