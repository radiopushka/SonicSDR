#include <math.h>
#include<stdlib.h>
#define PI 3.141592653589793

double STEP_INC=0.5;

int totalsize=0;

double** frequency_pre_calc_c=NULL;
double** frequency_pre_calc_s=NULL;

double calc_amplitude(int* buffer,int size,int freq){
	
  if(freq>totalsize){
  	return -1;
  }
  if(freq<=0){
  	return -1;
  }
  register double psum1=0;
  register double psum2=0;
  register double precalc=(((PI*2)*freq)/(size));
  //pre calculate the sine and cosine
  int i;
  if(frequency_pre_calc_c[freq-1]==NULL){
 	frequency_pre_calc_c[freq-1]=malloc(sizeof(double)*size);
 	frequency_pre_calc_s[freq-1]=malloc(sizeof(double)*size);
  	for(i=0;i<size;i++){
  		frequency_pre_calc_c[freq-1][i]=cos(precalc*i);
  		frequency_pre_calc_s[freq-1][i]=sin(precalc*i);
  	}
  }
  //now use the calculated values
  for(i=0;i<size;i++){
    
    
    
    psum1=psum1+frequency_pre_calc_c[freq-1][i]*buffer[i];
    psum2=psum2+frequency_pre_calc_s[freq-1][i]*buffer[i];
    
  }
  return (sqrt(psum1*psum1+psum2*psum2))/(size);
}
double* darray=NULL;
double* attwindow=NULL;
int gramsize=0;
int* frequencymap=NULL;
void free_fourier_transform(){
	free(darray);
	darray=NULL;
	free(attwindow);
	attwindow=NULL;
  free(frequencymap);
  frequencymap=NULL;
  if(frequency_pre_calc_c!=NULL){
  int i;
  for(i=0;i<totalsize;i++){
  	free(frequency_pre_calc_c[i]);
  	free(frequency_pre_calc_s[i]);
  }
  	free(frequency_pre_calc_c);
  	free(frequency_pre_calc_s);
  	frequency_pre_calc_c=NULL;
  	frequency_pre_calc_s=NULL;
  	
  }
  totalsize=0;
}
void init_fourier_transform(int size,double depth){//this size is f16 char divided by 2
	 gramsize=0;
	 int i;
	 free_fourier_transform();
	 totalsize=size;
	 STEP_INC=depth;
	 
	 double step=1;
	 for(i=0;i<(size>>1);i=i+step){
 	   step=step+STEP_INC;
 	   gramsize++;
 	 }
 	 frequency_pre_calc_c=malloc(sizeof(double*)*size);
 	 frequency_pre_calc_s=malloc(sizeof(double*)*size);
 	 for(i=0;i<size;i++){
 	 	frequency_pre_calc_c[i]=NULL;
 	 	frequency_pre_calc_s[i]=NULL;
 	 }
    frequencymap=malloc(sizeof(int)*(gramsize+1));
 	  darray=malloc(sizeof(double)*(gramsize));
 	  attwindow=malloc(sizeof(double)*(size));
 	  double stepc=(PI)/(size-1);
 	  double pos=0;
 	  for(i=0;i<size;i++){
 	  	attwindow[i]=cos(pos);
 	  	pos=pos+stepc;
 	  }
}
int get_freq_at_index(int i){
  return frequencymap[i];
}
double* produce_period_gram(int* buffer, int size,int start,int stop){//this size is f16 char divided by 2
  int i;
  double step=1;
  int array_step=0; 
  for(i=start;i<((size>>1)-stop);i=i+step){
    step=step+STEP_INC;
    darray[array_step]=calc_amplitude(buffer,size,i);
    frequencymap[array_step]=i;
    array_step++;
    frequencymap[array_step]=-1;
  }
  return darray;

}
int find_max_freq(int size,int* buffer){
	int i;
	double maxf=0;
	double f;
	int thef=0;
	for(i=0;i<(size>>1);i=i+1){
    		f=calc_amplitude(buffer,size,i);
 	 	if(f>maxf){
 	 		maxf=f;
 	 		thef=i;
 	 	}
 	 }
 	 return thef;
}
int get_fourier_size(){
	return gramsize;
}
void f16_array_to_int(char* in,int size,int* buffer,int channels){//size of buffer: size/2
  int i;
  short perm;
  int ssize=(size>>1)*channels;

  short* typecc=(short*)in;
  int ccount=0;
  int nindex=0;
  int f16index=0;
  for(i=0;i<ssize;i++){
    perm=typecc[i];
    nindex=i;//if we have more than one channel, we or them so that both are displayed
    for(ccount=1;ccount<channels;ccount++){
      nindex++;
      perm=(perm+typecc[nindex])>>1;
    }
    //take the average of the channels
    ccount=0;
    perm=perm*attwindow[i];
    buffer[f16index]=perm;
    f16index++;//convert it to mono
    i=nindex;
  }
}
