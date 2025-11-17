CC=gcc
CFLAGS=-mfpmath=both
LIBS=-lasound -lm

all:
	$(CC) $(CFLAGS) main.c curses/terminal.c colors/colors.c ./FFT/FFT.c -Wall $(LIBS) -fno-math-errno -fno-fast-math -O3 -march=native -g -o ftaudio
