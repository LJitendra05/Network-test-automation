#include <iostream>
#include <winsock2.h>
#include "tcp_client.h"
#include <chrono>
#include <vector>
using namespace std;
int main() {

    WSADATA wsaData;

    int result = WSAStartup(
        MAKEWORD(2, 2),
        &wsaData
    );

    if (result != 0) {
        cerr << "WSAStartup failed\n";
        return 1;
    }

    TcpClient client;

    if (client.connectToServer("127.0.0.1", 8080)) {
        cout << "Connected to server!\n";

        const int TEST_COUNT = 5;
        vector<long long> rttValues;

        for (int i = 1; i <= TEST_COUNT; i++) {

            auto start =chrono::high_resolution_clock::now();
            client.sendData("Hello Server");
            string response;
            if (client.receiveData(response)) {
                auto end =chrono::high_resolution_clock::now();
                auto duration =chrono::duration_cast<std::chrono::microseconds>(end - start);
                long long rttMicroseconds =duration.count();
                rttValues.push_back(rttMicroseconds);
                cout << "Test "<< i<< " RTT: "<< rttMicroseconds
                                            << " us ("<< rttMicroseconds / 1000.0<< " ms)\n";
            }
        }
        if (!rttValues.empty()) {
            long long minimum =rttValues[0];
            long long maximum =rttValues[0];
            long long total = 0;

            for (long long rtt : rttValues) {
                if (rtt < minimum) minimum = rtt;
                
                if (rtt > maximum) maximum = rtt;
                
                total += rtt;
            }
            double average =static_cast<double>(total) / rttValues.size();
            cout << "\n--- RTT Summary ---\n";
            cout << "Minimum RTT: "<< minimum<< " us ("<< minimum / 1000.0<< " ms)\n";
            cout << "Maximum RTT: "<< maximum<< " us ("<< maximum / 1000.0<< " ms)\n";
            cout << "Average RTT: "<< average<< " us ("<< average / 1000.0<< " ms)\n";
}
    }

    client.disconnect();

    WSACleanup();

    return 0;
}