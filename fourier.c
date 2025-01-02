#include <math.h>
#include<stdlib.h>
#define PI 3.141592653589793



float** frequency_pre_calc_c=NULL;
float** frequency_pre_calc_s=NULL;
float* frequencymap=NULL;
int loc_size=0;
int loc_range;
int sample_rate=48000;

float calc_amplitude(float* buffer,int size,int bloc){
	

  if(bloc>loc_size)
    return -1;

  float psum=0;
  float ssum=0;
  //pre calculate the sine and cosine
  int i;
  if(frequency_pre_calc_c[bloc]==NULL){
  double precalc=(frequencymap[bloc] / sample_rate) * (2*PI);
 	frequency_pre_calc_c[bloc]=malloc(sizeof(float)*size);
 	frequency_pre_calc_s[bloc]=malloc(sizeof(float)*size);
  double spos=0;
  	for(i=0;i<size;i++){
  		frequency_pre_calc_c[bloc][i]=cos(spos);
  		frequency_pre_calc_s[bloc][i]=sin(spos);
      spos=spos+precalc;
      if(spos >= 2*PI){
        spos = spos - (2*PI);
  	  }
    }
  }
  //now use the calculated values
  float* carray=frequency_pre_calc_c[bloc];
  float* sarray=frequency_pre_calc_s[bloc];

  psum=(*carray)*(*buffer);
  ssum=(*sarray)*(*buffer);
  carray++;
  sarray++;
  float* buffer_start=buffer+1;
  float* buffer_end=buffer+size;

  for(buffer=buffer_start;buffer<buffer_end;buffer++){
    
    
    
    psum=(psum+(*carray)*(*buffer));
    ssum=(ssum+(*sarray)*(*buffer));
    carray++;
    sarray++;
    
  }
  return sqrt(psum*psum+ssum*ssum)/size;
}

//calculation parameters
float* darray=NULL;
int gramsize=0;
int step=0;
int f_range=0;
int global_buffer_size;
//
void free_fourier_transform(){
	free(darray);
	darray=NULL;
  free(frequencymap);
  frequencymap=NULL;
  if(frequency_pre_calc_c!=NULL){
  int i;
  for(i=0;i<loc_range;i++){
  	free(frequency_pre_calc_c[i]);
  	free(frequency_pre_calc_s[i]);
  }
  	free(frequency_pre_calc_c);
  	free(frequency_pre_calc_s);
  	frequency_pre_calc_c=NULL;
  	frequency_pre_calc_s=NULL;
  	
  }
}
void init_fourier_transform(int size,int start,int stop,int width,int rate){//this size is f16 char divided by 2
   sample_rate=rate;
	 gramsize=width;
   global_buffer_size=size;
	 int i;
	 free_fourier_transform();

    f_range=stop-start;

    step=f_range/width; 
   int array_tracker=0;
    for(i=start;i<stop;i=i+step){
      array_tracker++;
    }
    loc_size=array_tracker;
    loc_range=array_tracker+1;

	 frequency_pre_calc_c=malloc(sizeof(float*)*(loc_range));
 	 frequency_pre_calc_s=malloc(sizeof(float*)*(loc_range));
 	 for(i=0;i<loc_range;i++){
 	 	frequency_pre_calc_c[i]=NULL;
 	 	frequency_pre_calc_s[i]=NULL;
 	 }
    frequencymap=malloc(sizeof(float)*(loc_range));
    
    array_tracker=0;
    for(i=start;i<stop;i=i+step){
      frequencymap[array_tracker]=i;
      array_tracker++;
    }
 	  darray=malloc(sizeof(float)*(loc_range));
}
int get_freq_at_index(int i){
  return frequencymap[i];
}
float* produce_period_gram(float* buffer){//this size is f16 char divided by 2

  for(int i=0;i<loc_size;i++){
    darray[i]=calc_amplitude(buffer,global_buffer_size,i);
  }
  return darray;

}
int find_max_freq(int size,float* buffer){
 	 return 0;
}
int get_fourier_size(){
	return loc_size;
}
void f16_array_to_f(short* in,int size,float* buffer,int channels){//size of buffer: size/2
  

  short* end=in+size*channels;
  
  int c=1;
  float avg=0;

  for(short* start=in;start<end;start++){
    

    if(c>=channels){
      *buffer=(avg+*start)/channels;
      buffer++;
      c=1;
      avg=0;
    }else{
      avg=(avg+*start);
      c++;
    }
    
  }
}
