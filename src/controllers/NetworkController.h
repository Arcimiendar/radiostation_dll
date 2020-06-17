//
// Created by artim on 15.06.2020.
//

#ifndef RADIOSTATION_DLL_NETWORKCONTROLLER_H
#define RADIOSTATION_DLL_NETWORKCONTROLLER_H
#include "../model/Server.h"
#include "../model/Client.h"

class NetworkController : public std::thread
{
    bool init_passed;

    Server * server;
    Client * client;

    bool n_exit;
    bool is_running;

    std::mutex change_state;

    Message nothing;
    Message sendSound;
    Message callingSound;
    Message receiveSound;

    bool is_send;
    bool is_call;
    bool is_config;

//    QAudioInput * audioInput;
//    QAudioOutput * audioOutput;
// TODO: do something with that

    float noise_level;

    void run();

    Message getNoise();

    void prepareToAudioOutput(Message& msg);

public:
    NetworkController(bool server, const string& ip = "127.0.0.1");

    void config_send(int frequency);
    void config_listen(int frequency);
    void config_kill();

    void call_on();
    void call_off();

    void setNoiseLevel(float level);

    Message& handle_cycle(char msg[MESSAGE_SIZE]);
    ~NetworkController();
};



#endif //RADIOSTATION_DLL_NETWORKCONTROLLER_H
