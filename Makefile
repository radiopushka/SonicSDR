all:
	gcc main.c fourier.c -Wall -lm -lasound -lncurses -ltinfo -lpthread -fno-math-errno -o1 -g -o ftaudio
