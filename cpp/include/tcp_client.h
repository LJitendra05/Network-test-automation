#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <winsock2.h>
#include <string>

class TcpClient {
private:
    SOCKET clientSocket;

public:
    TcpClient();

    bool connectToServer(
        const std::string& ip,
        int port
    );

    bool sendData(const std::string& message);
    bool receiveData(std::string& response);
    
    void disconnect();
};

#endif