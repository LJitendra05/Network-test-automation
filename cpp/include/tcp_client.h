#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <winsock2.h>
#include <string>
using namespace std;

class TcpClient {
private:
    SOCKET clientSocket;

public:
    TcpClient();

    bool connectToServer(
        const std::string& ip,
        int port
    );

    bool sendData(const string& message);
    bool receiveData(string& response);

    void disconnect();
};

#endif