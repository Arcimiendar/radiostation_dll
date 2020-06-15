//
// Created by artim on 15.06.2020.
//

#ifndef RADIOSTATION_DLL_SERVER_H
#define RADIOSTATION_DLL_SERVER_H
#include "../tools/Socket.h"
#include <list>
#include "message.h"
#include <mutex>
#include <thread>

class Server
{

    bool exit_flag;

    std::mutex list_mutex{};

    Socket socket;

    std::list<Socket*> sockets;
    std::list<Message> messages;

    friend class ListenningThread;
    friend class SendRecieveThread;

    class ListeningThread : public std::thread
    {
        Server * parentServer;
        void run();
    public:
        ListeningThread(Server* parent);

    } listeningThread;

    class SendReceiveThread : public std::thread
    {

        Server * parentServer;
        void run();
    public:
        SendReceiveThread(Server * parent);

    } receiveThread;

    public:
        Server();

        void close();
    };



#endif //RADIOSTATION_DLL_SERVER_H
