#ifndef RADIOSTATION_DLL_LIBRARY_H
#define RADIOSTATION_DLL_LIBRARY_H
#define _WINSOCKAPI_
#include <Windows.h>
#include "src/controllers/NetworkController.h"


extern "C" __declspec(dllexport) void init(const char ip_address[], bool is_server);
extern "C" __declspec(dllexport) void stop();

extern "C" __declspec(dllexport) void set_noise_level(float level);
extern "C" __declspec(dllexport) void is_call_active(bool is_active);
extern "C" __declspec(dllexport) void make_not_tuned();
extern "C" __declspec(dllexport) void make_tuned_for_listening_on_frequency(int frequency);
extern "C" __declspec(dllexport) void make_tuned_for_sending_on_frequency(int frequency);
extern "C" __declspec(dllexport) char* handle_cycle(char* in_sound);

#endif //RADIOSTATION_DLL_LIBRARY_H
