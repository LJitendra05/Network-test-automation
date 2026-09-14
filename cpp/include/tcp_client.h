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
    
    bool connectToServer(const string& ip,int port);
    
    bool sendAll(const char* data, int length);

    bool sendData(const string& message);

    bool receiveAll(char* data,int length);

    bool receiveData(string& response);

    void disconnect();
};

#endif