//
// Created by artim on 15.06.2020.
//

#ifndef RADIOSTATION_DLL_MESSAGE_H
#define RADIOSTATION_DLL_MESSAGE_H


#define MESSAGE_SIZE 1024
#define RATE_BYTES 8
struct Message
{
    char audio_data[MESSAGE_SIZE];
    int frequency;
    bool disconnect;
    bool call;
    bool sending;
    int id;

public:

    bool operator==(const Message& other) const
    {
        if (frequency != other.frequency)
            return false;
        if (disconnect != other.disconnect)
            return false;
        if (call != other.call)
            return false;
        if (sending != other.sending)
            return false;

        for(int i = 0; i < MESSAGE_SIZE; i++)
            if (audio_data[i] != other.audio_data[i])
                return false;

        return true;
    }
};

#endif //RADIOSTATION_DLL_MESSAGE_H
