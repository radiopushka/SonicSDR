CC=gcc
CFLAGS=
LIBS=-lasound -lm

all:
	$(CC) $(CFLAGS) main.c curses/terminal.c colors/colors.c fourier.c -Wall $(LIBS) -fno-math-errno -O2 -march=native -g -o ftaudio
