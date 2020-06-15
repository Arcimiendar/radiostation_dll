//
// Created by artim on 15.06.2020.
//

#include "Client.h"

Client::Client(const string& ip_address)
{

    if (this->socket.connect(ip_address.c_str(), 5000))
    {
        //throw std::string("Либо сервера не существует, либо проверьте подключение!");
    }
}

void Client::send(Message &msg)
{
    this->socket.send(reinterpret_cast<char*>(&msg), sizeof(msg));
}

void Client::receive(Message &msg)
{
    this->socket.receive(reinterpret_cast<char*>(&msg), sizeof(msg));
}

Client::~Client()
{
    Message m{};
    m.disconnect = true;
    m.sending = false;

    send(m);
}