//
// Created by artim on 15.06.2020.
//

#include "NetworkController.h"
#include <cmath>
#include <immintrin.h>
#include <iostream>
using std::cout;

void NetworkController::run()
{

//    auto audioOut = this->audioOutput->start();
//    auto audioIn = this->audioInput->start();

    bool switched = false;
    bool call = false;

    cout << "network thread is run";

    while(n_exit)
    {
        this->change_state.lock();

        if (this->is_config)
        {
            if (this->is_send)
            {
                if (this->is_call)
                {
                    if (!call)
                    {
                        switched = true;
                    }
                    call = true;

                    client->send(callingSound);
                    client->receive(callingSound);

//                    audioOut->write(callingSound.audio_data, MESSAGE_SIZE);

                }
                else
                {
                    if (call != false)
                    {
                        switched = true;
                    }
                    call = false;
//                    audioIn->read(sendSound.audio_data, MESSAGE_SIZE);

                    client->send(sendSound);
                    client->receive(sendSound);


//                    audioOut->write(sendSound.audio_data, MESSAGE_SIZE);
                }
            }
            else
            {
                memset(this->receiveSound.audio_data, '\0', MESSAGE_SIZE);
                client->send(this->receiveSound);
                client->receive(this->receiveSound);


                prepareToAudioOutput(receiveSound);

//                audioOut->write(recieveSound.audio_data, MESSAGE_SIZE);

                if (receiveSound.call)
                {
                    if (!call)
                    {
                        switched = true;
                    }
                    call = true;
                    receiveSound.call = false;
                }
                else
                {
                    if (call)
                    {
                        switched = true;
                    }
                    call = false;
                }
            }
        }
        else
        {

            client->send(nothing);
            client->receive(nothing);
        }

        this->change_state.unlock();

        if(switched)
        {
            switched = false;
            if (call)
            {
//                emit set_call();
            }
            else
            {
//                emit reset_call();
            }
        }
    }
}

Message NetworkController::getNoise()
{
    Message message;
    for (int i = 0; i <MESSAGE_SIZE; i ++)
        message.audio_data[i] = rand() % 100;

    return message;
}

void NetworkController::prepareToAudioOutput(Message &msg)
{
    Message noise = getNoise();

/*
    destiny = reinterpret_cast<__m128i*>(msg.audio_data);
    source = reinterpret_cast<__m128i*>(szum.audio_data);
    for (int i = 0; i < MESSAGE_SIZE / 16; i++)
    {
        destiny[i] += source[i];
    }
*/

    for (int i = 0; i < MESSAGE_SIZE; i++)
    {
        noise.audio_data[i] *= noise_level;
        msg.audio_data[i] += noise.audio_data[i];
    }
}

NetworkController::NetworkController(bool server, string ip)
: std::thread(&NetworkController::run, this)
{
    if (server)
        this->server = new Server();
    else
        this->server = nullptr;


    nothing.call = false;
    nothing.sending = false;
    nothing.disconnect = false;
    nothing.frequency = -1;

    sendSound.call = false;
    sendSound.sending = true;
    sendSound.disconnect = false;

    callingSound.call = true;
    callingSound.sending = true;
    callingSound.disconnect = false;
    for (int i = 0; i < MESSAGE_SIZE; i++)
        callingSound.audio_data[i] = char(50 * sin(i * MESSAGE_SIZE));

    receiveSound.call = false;
    receiveSound.sending = false;
    receiveSound.disconnect = false;


    this->n_exit = true;
//
//    QAudioDeviceInfo infoInput = QAudioDeviceInfo::defaultInputDevice();
//    QAudioDeviceInfo infoOutput = QAudioDeviceInfo::defaultOutputDevice();
//
//    QAudioFormat format;
//    format.setSampleRate(8000);
//    format.setChannelCount(1);
//    format.setSampleSize(RATE_BYTES);
//    format.setSampleType(RATE_BYTES == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
//    format.setByteOrder(QAudioFormat::LittleEndian);
//    format.setCodec("audio/pcm");
//
//    this->audioInput = new QAudioInput(infoInput, format);
//    this->audioOutput = new QAudioOutput(infoOutput, format);


    if (server)
        this->client = new Client("127.0.0.1");
    else
        this->client = new Client(ip);

    this->is_call = false;
    this->is_send = false;
    this->is_config = false;

    this->noise_level = 2;
}

void NetworkController::config_send(int frequency)
{
    this->change_state.lock();

    this->is_config = true;
    this->is_send = true;

    sendSound.frequency = frequency;
    callingSound.frequency = frequency;

    this->change_state.unlock();
}

void NetworkController::config_listen(int frequency)
{
    this->change_state.lock();

    this->is_config = true;
    this->is_send = false;

    receiveSound.frequency = frequency;

    this->change_state.unlock();
}

void NetworkController::config_kill()
{
    this->change_state.lock();

    this->is_config = false;

    this->change_state.unlock();
}

void NetworkController::call_on()
{
    this->change_state.lock();

    this->is_call = true;

    this->change_state.unlock();
}

void NetworkController::call_off()
{
    this->change_state.lock();

    this->is_call = false;

    this->change_state.unlock();
}

void NetworkController::setVolume(char level)
{
    change_state.lock();
//    this->audioOutput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

void NetworkController::setNoiseLevel(float level)
{
    change_state.lock();
    this->noise_level = level;
    change_state.unlock();
}

void NetworkController::setRecordVolume(char level)
{
    change_state.lock();
//    this->audioInput->setVolume(qreal(level) / 100);
    change_state.unlock();
}

NetworkController::~NetworkController()
{
    if (server)
        delete server;
    delete client;

//    delete audioInput;
//    delete audioOutput;

    this->n_exit = false;
}