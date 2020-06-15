#include "library.h"

#include <iostream>

NetworkController * controller;

void init(const char*& ip_address, bool is_server) {
    Socket::init();
    controller = new NetworkController(is_server, ip_address);
    controller->detach();
}

void stop() {
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

void handle_cycle(char sound[MESSAGE_SIZE], bool& is_calling_to_you) {
    Message& msg = controller->handle_cycle(sound);
    memcpy(sound, msg.audio_data, MESSAGE_SIZE);

    is_calling_to_you = msg.call;
}
