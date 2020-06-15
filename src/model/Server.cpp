//
// Created by artim on 15.06.2020.
//

#include "Server.h"
#include <smmintrin.h>
#include <iostream>
using std::cout;

Server::Server() :
        listeningThread(this),
        receiveThread(this)
{
    this->socket.bind(5000);
    this->exit_flag = true;

    listeningThread.detach();
    receiveThread.detach();

}

void Server::close()
{
    this->exit_flag = false;
}

void Server::ListeningThread::run()
{
    while(parentServer->exit_flag)
    {
        parentServer->socket.listen();
        Socket * client = parentServer->socket.accept();

        cout << "launched";

        parentServer->list_mutex.lock();
        parentServer->sockets.push_front(client);
        parentServer->messages.push_front(Message());
        parentServer->list_mutex.unlock();
    }
}

Server::ListeningThread::ListeningThread(Server *parent)
: thread(&Server::ListeningThread::run, this)
{
    this->parentServer = parent;
}

void Server::SendReceiveThread::run()
{

    while(parentServer->exit_flag)
    {
        parentServer->list_mutex.lock();

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
}

Server::SendReceiveThread::SendReceiveThread(Server *parent)
: std::thread(&Server::SendReceiveThread::run, this)
{
    this->parentServer = parent;
}
