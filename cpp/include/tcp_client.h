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

    void disconnect();
};

#endif