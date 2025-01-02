CC=gcc
CFLAGS=
LIBS=-ltinfo -lasound -lm -lncurses

all:
	$(CC) $(CFLAGS) main.c colors/ncolors.c fourier.c -Wall $(LIBS) -fno-math-errno -O2 -march=native -g -o ftaudio
