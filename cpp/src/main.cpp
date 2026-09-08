#include <iostream>
#include <winsock2.h>
#include "tcp_client.h"
#include <chrono>

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

        const int TEST_COUNT = 5;

    for (int i = 1; i <= TEST_COUNT; i++) {

        auto start =std::chrono::high_resolution_clock::now();

        client.sendData("Hello Server");

        std::string response;

        if (client.receiveData(response)) {

            auto end =std::chrono::high_resolution_clock::now();

            auto duration =std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            double rtt =duration.count() / 1000.0;
            std::cout << "Test "<< i<< " RTT: "<< rtt<< " ms\n";
            }
        }
    }

    client.disconnect();

    WSACleanup();

    return 0;
}