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


![SDR waterfall view](https://github.com/radiopushka/SonicSDR/assets/48099825/13b13c51-e950-483c-b3a4-34357ef87fca)


![SDR waterfall view one sine wave](https://github.com/radiopushka/SonicSDR/assets/48099825/e8c665d4-17b4-49ee-b15c-ad7f0e2d98a0)

![Screenshot_2024-02-17_22-24-33](https://github.com/radiopushka/SonicSDR/assets/48099825/59c01d54-8c32-4e0c-ad53-6b657d3f0a06)

![Screenshot_2024-02-17_22-24-46](https://github.com/radiopushka/SonicSDR/assets/48099825/4aeb45fe-4c1c-4c78-8768-3830ce30d9c5)

![Screenshot_2024-02-17_22-26-28](https://github.com/radiopushka/SonicSDR/assets/48099825/e67e7a48-e0f0-44fb-9696-d8b6353181da)

![Screenshot_2024-02-17_22-26-53](https://github.com/radiopushka/SonicSDR/assets/48099825/c2d52350-b582-43ec-a7fc-fd978c3089fc)
