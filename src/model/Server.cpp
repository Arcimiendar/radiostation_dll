//
// Created by artim on 15.06.2020.
//

#include "Server.h"
#include "Client.h"
#include <smmintrin.h>
#include <iostream>
using std::cout;

Server::Server() :
        listeningThread(this),
        receiveThread(this)
{
    this->socket.bind(5000);
    this->exit_flag = true;

    listeningThread.init_passed = true;
    receiveThread.init_passed = true;

    listeningThread.detach();
    receiveThread.detach();

}

void Server::close()
{
   list_mutex.lock();
   exit_flag = false;
   list_mutex.unlock();

   Client mock_client("127.0.0.1"); // to unlock listening thread

   while(listeningThread.is_running) {
       cout << "await stop of listening thread" << std::endl;
       cout.flush();

   }
   while(receiveThread.is_running) {
       cout << "await stop of receiving thread" << std::endl;
       cout.flush();
   }
}

void Server::ListeningThread::run()
{
    while(!init_passed);

    while(parentServer->exit_flag)
    {
        parentServer->socket.listen();
        Socket * client = parentServer->socket.accept();

        cout << "launched";

        parentServer->list_mutex.lock();
        if (!parentServer->exit_flag)
        {
            is_running = false;
            parentServer->list_mutex.unlock();
            return;
        }
        parentServer->sockets.push_front(client);
        parentServer->messages.push_front(Message());
        parentServer->list_mutex.unlock();
    }
    is_running = false;
}

Server::ListeningThread::ListeningThread(Server *parent)
: init_passed(false), thread(&Server::ListeningThread::run, this), is_running(true)
{
    this->parentServer = parent;
}

void Server::SendReceiveThread::run()
{
    while(!init_passed);

    while(parentServer->exit_flag)
    {
        parentServer->list_mutex.lock();
        if (!parentServer->exit_flag) {
            is_running = false;
            parentServer->list_mutex.unlock();
            return;
        }

        auto clientPtr = parentServer->sockets.begin();
        auto messagePtr = parentServer->messages.begin();
        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
            if ((*clientPtr)->receive(reinterpret_cast<char*>(&*messagePtr),
                                      sizeof(Message)) == -1)
            {
                messagePtr->call = false;
                messagePtr->sending = false;
                messagePtr->disconnect = true;
                messagePtr->frequency = -1;
            }
        }

        clientPtr = parentServer->sockets.begin();
        messagePtr = parentServer->messages.begin();
        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
            if(!messagePtr->sending && !messagePtr->disconnect)
            {

                memset(messagePtr->audio_data, '\0', MESSAGE_SIZE);

                for (auto &message : parentServer->messages)
                {
                    if (message.sending && !message.disconnect &&
                        message.frequency == messagePtr->frequency)
                    {
/*
                        for (int i = 0; i < MESSAGE_SIZE / 8; i++)
                        {
                            reinterpret_cast<long long *>(messagePtr->audio_data)[i] +=
                                    reinterpret_cast<long long *>(message.audio_data)[i];
                        }
*/

                        auto* destiny = reinterpret_cast<__m128i*>(messagePtr->audio_data);
                        auto* source  = reinterpret_cast<__m128i*>(message.audio_data);

                        for (int i = 0; i < MESSAGE_SIZE / 16; i++)
                        {
                            destiny[i] = _mm_hadd_epi32(destiny[i], source[i]);
                        }

                        if (message.call)
                            messagePtr->call = true;
                    }
                }
            }
        }

        clientPtr = parentServer->sockets.begin();
        messagePtr = parentServer->messages.begin();

        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
            if(messagePtr->disconnect)
            {

                cout << "disconnect";
                parentServer->messages.remove(*messagePtr);
                parentServer->sockets.remove(*clientPtr);

                messagePtr = parentServer->messages.begin();
                clientPtr = parentServer->sockets.begin();
            }
        }

        clientPtr = parentServer->sockets.begin();
        messagePtr = parentServer->messages.begin();
        for(;messagePtr != parentServer->messages.end(); clientPtr++, messagePtr++)
        {
            (*clientPtr)->send(reinterpret_cast<char*>(&*messagePtr), sizeof(Message));
        }

        parentServer->list_mutex.unlock();
    }
    is_running = false;
}

Server::SendReceiveThread::SendReceiveThread(Server *parent)
: init_passed(false), std::thread(&Server::SendReceiveThread::run, this), is_running(true)
{
    this->parentServer = parent;
}
