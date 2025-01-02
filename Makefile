all:
	cc main.c colors/ncolors.c fourier.c -Wall -lm -lasound -lncurses -ltinfo -lpthread -fno-math-errno -O2 -march=native -g -o ftaudio
