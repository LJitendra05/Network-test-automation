#include <iostream>
#include <winsock2.h>
#include "tcp_client.h"
#include <chrono>
#include <vector>
using namespace std;
int main(int argc, char* argv[]) {
    int testCount = 5;

    if (argc > 1) {
        try {
            testCount = stoi(argv[1]);
        }
        catch (const std::exception&) {
            cerr << "Invalid test count. "
                 << "Please provide a positive integer.\n";
            return 1;
        }
        if (testCount <= 0) {
            cerr << "Invalid test count. "
                 << "Test count must be greater than 0.\n";
            return 1;
        }
    }
    
    int payloadSize = 12;
    if (argc > 2) {
        try {
            payloadSize = stoi(argv[2]);
        }
        catch (const std::exception&) {
            cerr << "Invalid payload size. "
            << "Please provide a positive integer.\n";
            
            return 1;
        }
        
        if (payloadSize <= 0) {
            cerr << "Invalid payload size. "
            << "Payload size must be greater than 0.\n";
            
            return 1;
        }
    }    

    string serverIp = "127.0.0.1";
    int serverPort = 8080;
    if (argc > 3) {
        serverIp = argv[3];
    }
    if (argc > 4) {
        try {
            serverPort = stoi(argv[4]);
        }
        catch (const std::exception&) {
            cerr << "Invalid port. "
                 << "Please provide a valid port number.\n";
                 
                 return 1;
                }

        if (serverPort < 1 || serverPort > 65535) {
            cerr << "Invalid port. "
            << "Port must be between 1 and 65535.\n";
            
            return 1;
        }
    }

    string message(payloadSize, 'A');
    cout << "Test count: " << testCount << "\n";
    cout << "Payload size: " << payloadSize << " bytes\n";
    cout << "Server: " << serverIp << ":" << serverPort << "\n";
    
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
    if (!client.connectToServer(serverIp, serverPort)) {
        client.disconnect();
        WSACleanup();
        return 1;
    }
    cout << "Connected to server!\n";
    vector<long long> rttValues;
    int passedTests = 0;
    int failedTests = 0;
    for (int i = 1; i <= testCount; i++) {
        auto start =chrono::high_resolution_clock::now();
        if (client.sendData(message)) {
            string response;
            if (client.receiveData(response)) {
                auto end = chrono::high_resolution_clock::now();
                auto duration =
                    chrono::duration_cast<std::chrono::microseconds>(
                        end - start
                    );
                long long rttMicroseconds = duration.count();
                rttValues.push_back(rttMicroseconds);
                if (response == message) {
                    passedTests++;
                    cout << "Test " << i << ": PASS\n";
                } else {
                    failedTests++;
                    cout << "Test " << i << ": FAIL - Response mismatch\n";
                }
                cout << "RTT: "
                     << rttMicroseconds
                     << " us ("
                     << rttMicroseconds / 1000.0
                     << " ms)\n";
            }
            else {
                failedTests++;
                cout << "Test " << i << ": FAIL - Receive failed\n";
            }
        }
        else {
            failedTests++;
            cout << "Test " << i << ": FAIL - Send failed\n";
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

            cout << "\n--- Test Summary ---\n";
            cout << "Passed: " << passedTests << "\n";
            cout << "Failed: " << failedTests << "\n";
            if (failedTests == 0 && passedTests > 0) {
                cout << "Result: PASS\n";
            } else {
                cout << "Result: FAIL\n";
            }
        }   
    

    client.disconnect();

    WSACleanup();

    return 0;
}