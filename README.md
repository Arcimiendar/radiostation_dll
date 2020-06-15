# RADIOSTATION DLL
"Radiostation dll" is a core part of application

## API
### void init(const char*& ip_address, bool is_server)
Initiate application (create a server or connect to)
#### Arguments
``bool is_server`` - create a server or not <br>
``const char* & ip_address`` - connect to the server by specified ip address. 
If is_server == true, we ignore the content of this argument
#### Example
``init("192.168.1.1", false)`` <br>
``init("", true)``

### void stop()
Stop application
#### Example
``stop()``

### void set_noise_level(float level)
Set level of noise that will be added to receive sound
#### Arguments
`float level` - level of the noise. It's equal to 2 by default. 
If level == 0 than noise disappears. Recommended range of usage is [0..2]
#### Example
`set_noise_level(1.)`

### void make_not_tuned()
It makes radiostation not tuned again.
In this state we are not able to send and receive sound and calls
#### Example
`make_not_tuned()`

### void make_tuned_for_listening_on_frequency(int frequency)
It makes radiostation be able to listen on specified frequency
#### Arguments
`int frequency` - frequency on that application will be able to receive sound
#### Example
`make_tuned_for_listening_on_frequency(38000)`

### void void make_tuned_for_sending_on_frequency(int frequency);
It makes radiostation be able to send on specified frequency
#### Arguments
`int frequency` - frequency on that application will be able to send sound or call
#### Example
`make_tuned_for_sending_on_frequency(38000)`

### void is_call_active(bool is_active)
Activate or Disable calling. Call shall be sent if we made it tuned for sending (see function above).
You should not worry about calling that function or not. Call will be proceed after making tuned for sending.
#### Arguments
`bool is_active` - if is_active == true than we calling. If is_active == false than we disable calling
#### Example
`is_call_active(true)`

### void handle_cycle(char sound[MESSAGE_SIZE], bool& is_calling_to_you)
<b>This function must work in infinity loop!</b> <br>
A sound in the array must always be played after every call!
If we send sound - it must be in the array
#### Arguments
`char sound[MESSAGE_SIZE]` - MESSAGE_SIZE = 1024 for now.  <br>
If radiostation is turned to sending state, the content of this array will be sent.
So that means that it is an input of sending sound.<br>
We must always play the content of this array after calling function!
<b>EVEN IF THE RADIOSTATION IS CALLING, IS NOT CONFIGURED, SENDING OR RECEIVING!!!!!</b>

Also expected sound type:
* sample rate = 8000 (recommended. May be different)
* channel count = 1 (required)
* rate bytes = 8 (required)
* byte order = little endian (required, but i'm not sure. It seems like it will work with big endian too)
* code = audio/pcm (required)
* sample type = Unsigned int (recommended, but <b>USING FLOAT IS FORBIDDEN</b>)

`bool& is_calling_to_you` - this param will be false after calling function if nobody is calling now.
It will be true if somebody is actually calling. We need it to draw calling signals.

#### Example
void handle_cycle(buffer, flag)
