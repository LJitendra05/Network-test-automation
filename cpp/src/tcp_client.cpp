#include "tcp_client.h"
#include <iostream>
#include <ws2tcpip.h>
#include <chrono>

TcpClient::TcpClient() {
    clientSocket = INVALID_SOCKET;
}

bool TcpClient::connectToServer(
    const std::string& ip,
    int port
) {
    clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        return false;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());

    auto start = std::chrono::high_resolution_clock::now();
    int result = connect(
        clientSocket,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)
    );
    auto end = std::chrono::high_resolution_clock::now();
    if (result == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        std::cerr << "TCP connection failed\n";
        std::cerr << "Error code: " << errorCode << "\n";
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        return false;
    }

    auto duration =
    std::chrono::duration_cast<std::chrono::microseconds>(
        end - start
    );

    std::cout << "TCP connection successful\n";
    std::cout << "Connection time: "
          << duration.count() / 1000.0
          << " ms\n";

    return true;
}

bool TcpClient::sendData(const std::string& message) {
    int bytesSent = send(
        clientSocket,
        message.c_str(),
        static_cast<int>(message.size()),
        0
    );

    if (bytesSent == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();

        std::cerr << "Send failed\n";
        std::cerr << "Error code: " << errorCode << "\n";

        return false;
    }

    std::cout << "Bytes sent: " << bytesSent << "\n";

    return true;
}

bool TcpClient::receiveData(std::string& response) {
    char buffer[1024];

    int bytesReceived = recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytesReceived == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();

        std::cerr << "Receive failed\n";
        std::cerr << "Error code: " << errorCode << "\n";

        return false;
    }

    if (bytesReceived == 0) {
        std::cerr << "Server closed the connection\n";
        return false;
    }

    buffer[bytesReceived] = '\0';

    response = buffer;

    std::cout << "Bytes received: "
              << bytesReceived
              << "\n";

    return true;
}
void TcpClient::disconnect() {

    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
}