#ifndef _FOURIER
#define _FOURIER
  float calc_amplitude(float* buffer, int size,int freq);
  float* produce_period_gram(float* buffer);//initially you could set the step size but for audio I want to see all the frequencies so i made the step increse.
  int get_fourier_size();
  //do not free the array this returns
  void init_fourier_transform(int size,int start,int stop,int width,int rate);//depth must be 1 or less, the lower the depth the more points get put in
  void free_fourier_transform();
  void f16_array_to_f(short* in,int size,float* buffer,int channels);
  int get_freq_at_index(int index);
  //size of array buffer should be size/2
  int find_max_freq(int size,float* buffer);
#endif // !_FOURIER
