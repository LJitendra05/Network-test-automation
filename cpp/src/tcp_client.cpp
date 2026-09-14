#include "tcp_client.h"
#include <iostream>
#include <ws2tcpip.h>
#include <chrono>
using namespace std;

string getSocketErrorMessage(int errorCode) {

    switch (errorCode) {

        case WSAECONNREFUSED:
            return "Connection refused";

        case WSAETIMEDOUT:
            return "Connection timed out";

        case WSAEHOSTUNREACH:
            return "Host unreachable";

        case WSAENETUNREACH:
            return "Network unreachable";

        default:
            return "Unknown socket error";
    }
}
TcpClient::TcpClient() {
    clientSocket = INVALID_SOCKET;
}

bool TcpClient::connectToServer(
    const string& ip,
    int port
) {
    clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed\n";
        return false;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());

    auto start = chrono::high_resolution_clock::now();
    int result = connect(
        clientSocket,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)
    );
    auto end = chrono::high_resolution_clock::now();
    if (result == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        cerr << "TCP connection failed\n";
        cerr << "Error code: " << errorCode << "\n";
        cerr << "Reason: "
             << getSocketErrorMessage(errorCode)
             << "\n";
        cerr << "Target: " << ip << ":" << port << "\n";
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        return false;
    }

    auto duration =
    chrono::duration_cast<chrono::microseconds>(
        end - start
    );

    cout << "TCP connection successful\n";
    cout << "Connection time: "<< duration.count() / 1000.0<< " ms\n";

    int timeoutMs = 3000;
    setsockopt(clientSocket,SOL_SOCKET,SO_RCVTIMEO,
        reinterpret_cast<const char*>(&timeoutMs),sizeof(timeoutMs));

    return true;
}

bool TcpClient::sendAll(const char* data,int length) {
    int totalSent = 0;
    while (totalSent < length) {
        int bytesSent = send(
            clientSocket,
            data + totalSent,
            length - totalSent,
            0
        );
        if (bytesSent == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            cerr << "Send failed\n";
            cerr << "Error code: "
                 << errorCode
                 << "\n";
            return false;
        }
        if (bytesSent == 0) {
            cerr << "Send returned 0 bytes\n";
            return false;
        }
        totalSent += bytesSent;
    }
    return true;
}

bool TcpClient::sendData(const string& message) {
    int messageSize =
        static_cast<int>(message.size());
    int networkSize =
        htonl(messageSize);
    if (!sendAll(
            reinterpret_cast<const char*>(&networkSize),
            sizeof(networkSize)
        )) {
        return false;
    }
    if (!sendAll(
            message.c_str(),
            messageSize
        )) {
        return false;
    }
    cout << "Bytes sent: "
         << messageSize
         << "\n";
    return true;
}

bool TcpClient::receiveAll(char* data,int length) {
    int totalReceived = 0;
    while (totalReceived < length) {
        int bytesReceived = recv(
            clientSocket,
            data + totalReceived,
            length - totalReceived,
            0
        );
        if (bytesReceived == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            if (errorCode == WSAETIMEDOUT) {
                cerr << "Receive timeout\n";
            } else {
                cerr << "Receive failed\n";
                cerr << "Error code: "
                     << errorCode
                     << "\n";
            }
            return false;
        }
        if (bytesReceived == 0) {
            cerr << "Server closed the connection\n";
            return false;
        }
        totalReceived += bytesReceived;
    }
    return true;
}

bool TcpClient::receiveData(string& response) {
    int networkSize;
    if (!receiveAll(
            reinterpret_cast<char*>(&networkSize),
            sizeof(networkSize)
        )) {
        return false;
    }
    int messageSize = ntohl(networkSize);
    if (messageSize < 0 || messageSize > 100000000) {
        cerr << "Invalid message size: "
             << messageSize
             << "\n";
        return false;
    }
    response.resize(messageSize);
    if (messageSize > 0) {
        if (!receiveAll(
                &response[0],
                messageSize
            )) {
            return false;
        }
    }
    cout << "Bytes received: "
         << messageSize
         << "\n";
    return true;
}

void TcpClient::disconnect() {

    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
}