//
// Created by artim on 15.06.2020.
//

#ifndef RADIOSTATION_DLL_NETWORKCONTROLLER_H
#define RADIOSTATION_DLL_NETWORKCONTROLLER_H
#include "../model/Server.h"
#include "../model/Client.h"

class NetworkController : std::thread
{

    Server * server;
    Client * client;

    bool n_exit;

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
    NetworkController(bool server, string ip = "127.0.0.1");

    void config_send(int frequency);
    void config_listen(int frequency);
    void config_kill();

    void call_on();
    void call_off();

    void setVolume(char level);
    void setNoiseLevel(float level);
    void setRecordVolume(char level);

    ~NetworkController();
};



#endif //RADIOSTATION_DLL_NETWORKCONTROLLER_H
