#include "library.h"

#include <iostream>

NetworkController * controller;
char * sound;

void init(const char ip_address[], bool is_server) {
    sound = new char[1026];
    sound[1025] = 0;
    Socket::init();
    controller = new NetworkController(is_server, ip_address);
    controller->detach();
}

void stop() {
    delete sound;
    delete controller;
    Socket::clear();
}

void set_noise_level(float level) {
    controller->setNoiseLevel(level);
}

void is_call_active(bool is_active) {
    if (is_active)
        controller->call_on();
    else
        controller->call_off();
}

void make_not_tuned() {
    controller->config_kill();
}

void make_tuned_for_listening_on_frequency(int frequency) {
    controller->config_listen(frequency);
}

void make_tuned_for_sending_on_frequency(int frequency) {
    controller->config_send(frequency);
}

char* handle_cycle(char *in_sound) {
    Message& msg = controller->handle_cycle(in_sound);
    memcpy(sound, msg.audio_data, MESSAGE_SIZE);
    sound[1024] = msg.call;

    return sound;
}
