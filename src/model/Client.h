//
// Created by artim on 15.06.2020.
//

#ifndef RADIOSTATION_DLL_CLIENT_H
#define RADIOSTATION_DLL_CLIENT_H


#include "../tools/Socket.h"
#include "message.h"
#include <string>
using std::string;

class Client {

    Socket socket;

public:
    explicit Client(const string& ip_address);

    void send(Message& msg);
    void receive(Message& msg);

    ~Client();
};


#endif //RADIOSTATION_DLL_CLIENT_H
