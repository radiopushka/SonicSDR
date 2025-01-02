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

![image on sdr](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-01.png)


![digital ultrasonic signal](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-07.png)

![music in DSB](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-12.png)

![music in DSB](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-14.png)

![music in DSB](https://github.com/radiopushka/SonicSDR/blob/main/Screenshots/2025-01-01_23-16.png)

