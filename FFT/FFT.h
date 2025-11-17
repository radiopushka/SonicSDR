//Evan Nikitin 2025
//Windowed FFT algorithm with oversampling and pipelining
//Faster than the Cooley-Tukey FFT algorithm due to hardware properties of CPUs
#ifndef FFT_
#define FFT_

struct FFT{
    float** FFT_array_sine;
    float** FFT_array_cosine;
    unsigned int frequencies;
    float* frequency_map;
    unsigned int window_size;
    unsigned int itterator;
    //accelerator
    float* fft_aggregation;
    float* cosine_aggregation;
    float* sine_aggregation;
    double* avg_fft;

    double* acc_fft;
    double* acs_fft;
};
struct FFT_entry{
    float sine;
    float cosine;
};
struct FFT_group{
    float* sine;
    float* cosine;
};


typedef struct FFT* FFT;

//frequency map: an array of frequencies in Hz
//frequencies: the number of frequencies (number of elements in array above)
//oversampling: the oversampling factor
//window_size: the FFT window size, larger windows can capture lower frequencies
//sampling_rate: the sampling rate in Hz
FFT create_FFT(float* frequency_map,int frequencies,int oversampling,int window_size,int sampling_rate);
void free_FFT(FFT fft);

//input: one value from the sampled array
struct FFT_group calculate_FFT_single(FFT fft,float input);

//increment_counter: set to 0 to not increment the fft instance counter
//findex: the index of the frequency
//set increment counter to 0 for all the frequencies you want to extract but for the last one set it to one
struct FFT_entry FFT_get_instance(FFT fft,int increment_counter,int findex);

//calculate phase and amplitude, FFT on a sampling window
//input: array of samples
//input_size: size of array
double* calculate_FFT(FFT fft,float* input,int input_size);
double* calculate_FFT_phase(FFT fft,float* input,int input_size);
double* calculate_FFT_phase_delta(FFT fft,float* input,int input_size,int carrier_index);
//note: phase returns an array of phase values for each frequency of the fft
//carrier index is the frequency from which the phase difference is measured
#endif

