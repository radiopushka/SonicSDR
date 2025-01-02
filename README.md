# Sonic SDR
##Software Define "radio", produces a visual waterfall model of interfaces
You can use it to for example detect certain frequnecies in audio. If you have a sound card(or a device recognized by Alsa) that is able to sample at higher frequencies, you can use it for various other purposes. typically, your sound card samples at 48000 khz, divide that by two to get the maximum frequency possible.
more information if you call ./ftaudio
there will be a print once you exit
to look up the keystrokes in interactive mode, hold h

audio device format: hw:CARD,DEVICE


Could be use for the reception of VLF and LF signals with proper hardware
## Linux Dependencies
Alsa 
## interface
It uses a command line ncurses based GUI, some people might hate this while others might love it. 
a usefull program: https://github.com/radiopushka/Ultrasonic_Toolkit

## binaries:
[compiled on gentoo i5 12th gen adler](ftaudio)

## Screenshots


![SDR waterfall view](https://github.com/radiopushka/SonicSDR/assets/48099825/13b13c51-e950-483c-b3a4-34357ef87fca)


![SDR waterfall view one sine wave](https://github.com/radiopushka/SonicSDR/assets/48099825/e8c665d4-17b4-49ee-b15c-ad7f0e2d98a0)

![Screenshot_2024-02-17_22-24-33](https://github.com/radiopushka/SonicSDR/assets/48099825/59c01d54-8c32-4e0c-ad53-6b657d3f0a06)

![Screenshot_2024-02-17_22-24-46](https://github.com/radiopushka/SonicSDR/assets/48099825/4aeb45fe-4c1c-4c78-8768-3830ce30d9c5)

![Screenshot_2024-02-17_22-26-28](https://github.com/radiopushka/SonicSDR/assets/48099825/e67e7a48-e0f0-44fb-9696-d8b6353181da)

![Screenshot_2024-02-17_22-26-53](https://github.com/radiopushka/SonicSDR/assets/48099825/c2d52350-b582-43ec-a7fc-fd978c3089fc)
