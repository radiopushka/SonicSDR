#include "FFT.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>


void populate_fft(int rate,int oversampling,int window_size,float* arr_sin,float* arr_cos,float freq){
    double shifter = (freq / ((float)(rate * oversampling)))*(2*M_PI);
    float* ps = arr_sin;
    double svf = 0;
    for(float* pc = arr_cos; pc < arr_cos + window_size; pc++){
        double avs = 0;
        double avc = 0;
        for(int i = 0;i<oversampling;i++){
            avs = avs + (double)sin(svf);
            avc = avc + (double)cos(svf);
            svf = svf + shifter;
            if(svf > 2*M_PI){
                svf = svf - 2*M_PI;
            }
        }
        avs = (avs/((double)oversampling));
        avc = (avc/((double)oversampling));
        *pc = (float)avc;
        *ps = (float)avs;
        ps++;
    }
}
FFT create_FFT(float *frequency_map, int frequencies, int oversampling, int window_size,int samplerate){
    FFT fft = malloc(sizeof(struct FFT));
    fft->frequency_map = malloc(sizeof(float)*frequencies);
    fft->frequencies = frequencies;
    memmove(fft->frequency_map, frequency_map, sizeof(float)*frequencies);
    fft->window_size = window_size;


    fft->FFT_array_cosine = malloc(sizeof(float*)*window_size);
    fft->FFT_array_sine = malloc(sizeof(float*)*window_size);
    for(float** ap = fft->FFT_array_cosine; ap < fft->FFT_array_cosine + window_size; ap++){
        *ap = malloc(sizeof(float)*frequencies);

    }
    for(float** ap = fft->FFT_array_sine; ap < fft->FFT_array_sine + window_size; ap++){
        *ap = malloc(sizeof(float)*frequencies);
    }
    float* fm = fft->frequency_map;

    float** cos_helper = malloc(sizeof(float*)*frequencies);
    for(float** ap = cos_helper; ap < cos_helper + frequencies; ap++){
        *ap = malloc(sizeof(float)*window_size);
    }
    float** sin_helper = malloc(sizeof(float*)*frequencies);
    for(float** ap = sin_helper; ap < sin_helper + frequencies; ap++){
        *ap = malloc(sizeof(float)*window_size);
    }



    for(int i =0;i<frequencies;i++){
        populate_fft(samplerate, oversampling, window_size, sin_helper[i], cos_helper[i], *fm);
        fm++;

    }


    for(int fl =0; fl<frequencies;fl++){
        for(int wl = 0;wl<window_size;wl++){
            fft->FFT_array_sine[wl][fl] = sin_helper[fl][wl];
            fft->FFT_array_cosine[wl][fl] = cos_helper[fl][wl];
        }
    }
    for(float** ap = cos_helper; ap < cos_helper + frequencies; ap++){
        free(*ap);
    }
    for(float** ap = sin_helper; ap < sin_helper + frequencies; ap++){
        free(*ap);
    }
    free(cos_helper);
    free(sin_helper);


    fft->itterator = 0;
    fft->cosine_aggregation = malloc(sizeof(float)*frequencies);
    memset(fft->cosine_aggregation,0,sizeof(float)*frequencies);
    fft->sine_aggregation = malloc(sizeof(float)*frequencies);
    memset(fft->sine_aggregation,0,sizeof(float)*frequencies);
    fft->fft_aggregation = malloc(sizeof(float)*frequencies);
    memset(fft->fft_aggregation,0,sizeof(float)*frequencies);
    fft->avg_fft = malloc(sizeof(double)*frequencies);
    memset(fft->avg_fft,0,sizeof(float)*frequencies);
    fft->acc_fft = malloc(sizeof(double)*frequencies);
    memset(fft->acc_fft,0,sizeof(float)*frequencies);
    fft->acs_fft = malloc(sizeof(double)*frequencies);
    memset(fft->acs_fft,0,sizeof(float)*frequencies);

    return fft;
}

void free_FFT(FFT fft){
    free(fft->cosine_aggregation);
    free(fft->sine_aggregation);
    free(fft->fft_aggregation);
    free(fft->frequency_map);
    free(fft->acs_fft);
    free(fft->acc_fft);
    for(float** ap = fft->FFT_array_cosine; ap < fft->FFT_array_cosine + fft->window_size; ap++){
        free(*ap);
    }
    for(float** ap = fft->FFT_array_sine; ap < fft->FFT_array_sine + fft->window_size; ap++){
        free(*ap);
    }
    free(fft->FFT_array_cosine);
    free(fft->FFT_array_sine);
    free(fft->avg_fft);
    free(fft);
}
struct FFT_group calculate_FFT_single(FFT fft,float input){
    //input: the input signal
    //fft: the FFT object
    //first pull out the pointers from the object

    unsigned int itterator = fft->itterator;
    if(itterator >= fft->window_size){
        itterator = 0;
    }


    float* restrict cos_lp = *((fft->FFT_array_cosine)+itterator);
    float* restrict cos_lp_limit = cos_lp + fft->frequencies;
    float* restrict sin_lp = *((fft->FFT_array_sine)+itterator);


    float* restrict agr_sin = fft->sine_aggregation;
    float* restrict agr_cos = fft->cosine_aggregation;
    struct FFT_group fftg;
    fftg.sine = agr_sin;
    fftg.cosine = agr_cos;
    fft->itterator = itterator + 1;
    //calculate cosine and sine values
    for(;cos_lp < cos_lp_limit;cos_lp++){
        *agr_cos = (*cos_lp)*(input);
        *agr_sin = (*sin_lp)*(input);
        agr_cos++;
        agr_sin++;
        sin_lp++;
    }
    return fftg;
}
double* calculate_FFT(FFT fft,float* input,int input_size){

    double* restrict acc_c;
    double* restrict acc_s;
    float* restrict ist = input;
    float* restrict input_end = input + input_size;
    double* restrict end = fft->acs_fft + fft->frequencies;
    float* restrict ss;
    float* restrict cc;
    for(;input < input_end;input++){
        calculate_FFT_single(fft, *input);
        acc_c = fft->acc_fft;
        acc_s = fft->acs_fft;
        ss = fft->sine_aggregation;
        cc = fft->cosine_aggregation;
        for(;acc_s < end;acc_s++){
            if(input!=ist){
                *acc_c = *acc_c + (double)*cc;
                *acc_s = *acc_s + (double)*ss;
            }else{
                *acc_c = (double)*cc;
                *acc_s = (double)*ss;
            }
            ss++;
            cc++;
            acc_c++;
        }
    }
    acc_c = fft->acc_fft;
    for(acc_s = fft->acs_fft;acc_s < end;acc_s++){
        *acc_c = (*acc_c)*(*acc_c);
        *acc_s = (*acc_s)*(*acc_s);
        acc_c++;
    }
    acc_c = fft->acc_fft;
    acc_s = fft->acs_fft;


    double* restrict avg = fft->avg_fft;
    double* restrict avg_end = fft->avg_fft + fft->frequencies;
    for(avg = fft->avg_fft;avg < avg_end;avg++){
        *avg = (double)sqrtf(*acc_c + *acc_s);
        acc_c++;
        acc_s++;
    }
    return fft->avg_fft;
}
double* calculate_FFT_phase(FFT fft,float* input,int input_size){
    double* restrict acc_c;
    double* restrict acc_s;
    float* restrict ist = input;
    float* restrict input_end = input + input_size;
    double* restrict end = fft->acs_fft + fft->frequencies;
    float* restrict ss;
    float* restrict cc;
    for(;input < input_end;input++){
        calculate_FFT_single(fft, *input);
        acc_c = fft->acc_fft;
        acc_s = fft->acs_fft;
        ss = fft->sine_aggregation;
        cc = fft->cosine_aggregation;
        for(;acc_s < end;acc_s++){
            if(input!=ist){
                *acc_c = *acc_c + (double)*cc;
                *acc_s = *acc_s - (double)*ss;
            }else{
                *acc_c = (double)*cc;
                *acc_s = (double)*ss;
            }
            ss++;
            cc++;
            acc_c++;
        }
    }
    acc_c = fft->acc_fft;
    acc_s = fft->acs_fft;


    double* restrict avg = fft->avg_fft;
    double* restrict avg_end = fft->avg_fft + fft->frequencies;
    for(avg = fft->avg_fft;avg < avg_end;avg++){
        *avg = atan2(*acc_s,*acc_c);
        acc_c++;
        acc_s++;
    }
    return fft->avg_fft;
}
double* calculate_FFT_phase_delta(FFT fft,float* input,int input_size,int carrier){
    double* restrict acc_c;
    double* restrict acc_s;
    float* restrict ist = input;
    float* restrict input_end = input + input_size;
    double* restrict end = fft->acs_fft + fft->frequencies;
    float* restrict ss;
    float* restrict cc;

    for(;input < input_end;input++){
        calculate_FFT_single(fft, *input);
        acc_c = fft->acc_fft;
        acc_s = fft->acs_fft;
        ss = fft->sine_aggregation;
        cc = fft->cosine_aggregation;
        for(;acc_s < end;acc_s++){
            if(input!=ist){
                *acc_c = *acc_c + (double)*cc;
                *acc_s = *acc_s - (double)*ss;
            }else{
                *acc_c = (double)*cc;
                *acc_s = (double)*ss;
            }
            ss++;
            cc++;
            acc_c++;
        }
    }
    acc_c = fft->acc_fft;
    acc_s = fft->acs_fft;

    float t_i = *(acc_s + carrier);
    float t_r = *(acc_c + carrier);

    double* restrict avg = fft->avg_fft;
    double* restrict avg_end = fft->avg_fft + fft->frequencies;
    for(avg = fft->avg_fft;avg < avg_end;avg++){
        *avg = atan2(((*acc_c)*t_i)+((*acc_s)*t_r),((*acc_c)*t_r)+((*acc_s)*t_i));
        acc_c++;
        acc_s++;
    }
    return fft->avg_fft;
}

struct FFT_entry FFT_get_instance(FFT fft,int increment_counter,int findex){

    unsigned int itterator = fft->itterator;
    if(itterator >= fft->window_size){
        itterator = 0;
    }

    float** cos_lp = fft->FFT_array_cosine;
    float** sin_lp = fft->FFT_array_sine;

    struct FFT_entry entry;
    entry.sine = *(*(cos_lp + itterator)+findex);
    entry.cosine = *(*(sin_lp + itterator)+findex);

    if(increment_counter){
        fft->itterator = itterator + 1;
    }

    return entry;

}
