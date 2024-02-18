# Sonic SDR
##Software Define "radio", produces a visual waterfall model of interfaces
You can use it to for example detect certain frequnecies in audio. If you have a sound card(or a device recognized by Alsa) that is able to sample at higher frequencies, you can use it for various other purposes. To do this, you need to add the V option ./ftaudio V <audio device> \[sample rate\] \[ft rate\] the arguments in brackets are optional. typically, your sound card samples at 44100 khz, divide that by two to get the maximum frequency possible.

## interface
It uses a command line ncurses based GUI, some people might hate this while others might love it. You can use the arrow keys to zoom in and out as well as move left right, press 0 to go to frequency 0 and press d for default zoom.
Press space to pause or unpause the waterfall.
If you press any other key you will exit the program.

## binaries:
[compiled on slackware x86 core](x86_64_fft_program/ftaudio)

## Screenshots


![Screenshot_2024-02-17_19-13-07](https://github.com/radiopushka/SonicSDR/assets/48099825/13b13c51-e950-483c-b3a4-34357ef87fca)
