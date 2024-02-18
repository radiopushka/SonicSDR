all:
	gcc main.c fourier.c -Wall -lm -lasound -lncursesw -lpthread -fno-math-errno -o1 -g -o ftaudio
