#include "Socket.h"

#include <winsock2.h>
#define INVALID_SOCK_VALUE( SOCK ) SOCK == INVALID_SOCKET

#include <cstring>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using std::cout;

Socket::Socket()
{
    if (INVALID_SOCK_VALUE((this->sock = socket(AF_INET, SOCK_STREAM, 0))))
        cout << "error sock init";
    cout << "sock is inited";
}

Socket::~Socket()
{
    closesocket(this->sock);
}

int Socket::connect(const char * ip_addr, int port)
{
    memset(&this->address, '\0', sizeof(this->address));
    this->address.sin_addr.s_addr = inet_addr(ip_addr);
    this->address.sin_family = AF_INET;

    this->address.sin_port = htons(port);

    if (::connect(
            this->sock,
            (sockaddr*)&this->address,
            sizeof(address)
    ) < 0)
    {
        cout << "error connection";
        return 2;
    }
    cout << "conetion is good";
    return 0;
}

int Socket::send(char* data, int len)
{
    if (::send(this->sock, data, len, 0) < 0)//is_server ? 0 : MESSAGE_SIZE) < 0)
        return 1;
    else
        return 0;
}
int Socket::receive(char* buffer, int len)
{
    int recv_size;
    recv_size = recv(this->sock, buffer, len, 0);
    if (recv_size == SOCKET_ERROR)
        return -1;
    else {
        return recv_size;
    }
}

int Socket::bind(int port)
{
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    auto resp = ::bind(this->sock, (sockaddr*)&this->address, sizeof(address));

    return resp < 0;
}
int Socket::listen() const
{
    cout << "listening";
    auto resp = ::listen(this->sock, 1);
    return resp < 0;
}
Socket * Socket::accept()
{
    auto *sock = new Socket(6);

    int size = sizeof(address);

    sock->sock = ::accept(this->sock, (sockaddr*)&this->address, &size);

    cout << (INVALID_SOCK_VALUE(sock->sock));

    return sock;
}

Socket::Socket(int)
{
}

void Socket::init() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
}
void Socket::clear() {

    WSACleanup();

}
