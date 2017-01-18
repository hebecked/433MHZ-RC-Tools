#define __STDC_LIMIT_MACROS
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>


/*
// gcc -Wall -pedantic -o receiver receiver.c -lm -lwiringPi -std=gnu99

#define BITE_LENGTH 25 (0-longer or predefined if longer 3 bytes to store, if predefined compare and run command from config file)


commands:
play
pause
volume up
volume down
kill xbmc
run xbmc
run retroarch
stop retroarch

Read in corresponding times from file

 waitForInterrupt (int pin, int timeOut) ;

 */

/*struct timespec{ 
	time_t tv_sec; // seconds 
	long tv_nsec; // nanoseconds  
};
*/

struct config{
	int32_t* codes;
	char** commands;
	int length;
};

struct config data;


//struct config read_config(char* filename){
void read_config(char* filename){
	//struct config data;
	int i,j,l;
    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;
    int32_t itemp;
	FILE* datafile=fopen(filename, "r");
	if(datafile==NULL)printf("ERROR!!!");
	data.codes=NULL;
	data.commands=NULL;
	for(i=0;!feof(datafile);i++){
  		read = getline (&buffer, &len, datafile);
		if (read == -1){
      		break;
    	}
		switch(buffer[0]){
			case 'n':
				sscanf(buffer,"n%i ",&itemp);
				break;
			case '0':
			case '1':
				itemp=0;
				for(j=0;j<32;j++){
					if(buffer[j]=='1'){
						itemp+=pow(2,31-j);
					}else{
						if(buffer[j]!='0'){
							printf("ERROR!!!");
							break;
						}
					}
				}
				break;
			default:
				printf("ERROR!!!");
				break;
		}
		for(j=0;j<40;j++){
			if(buffer[j]==' '){
				break;
			}
		}
		j++;
		data.codes=realloc(data.codes,(i+1)*sizeof(int32_t));
		if(data.codes==NULL)printf("ERROR!!!");
		data.commands=realloc(data.commands,(i+1)*sizeof(char*));
		if(data.commands==NULL)printf("ERROR!!!");
		data.commands[i]=NULL;
		data.commands[i]=realloc(data.commands[i],(read-j+2)*sizeof(char));
		if(data.commands[i]==NULL)printf("ERROR!!!");
		for(l=0;l<read-j;l++){
			data.commands[i][l]=buffer[j+l+1];
		}
		//data.commands[i][l]='\0';
		data.codes[i]=itemp;
	}
	data.length=i;
/*	for(i=0;i<data.length;i++){
		printf("%li\n",data.codes[i]);
		printf("%s\n",data.commands[i]);
	}*/
	return;
//	return data;
}


void eval(struct timespec *array){
	int array2[32];
	int i;
	int32_t code;
	long sec_temp,nsec_temp;
	for(i=1;i<64;i+=2){
		sec_temp=array[i+1].tv_sec-array[i].tv_sec;
		if(sec_temp==0){
			nsec_temp=array[i+1].tv_nsec-array[i].tv_nsec;
		}else{
			if(sec_temp==1){
				nsec_temp=array[i+1].tv_nsec-array[i].tv_nsec+1000000000;
			}else{
				return;
			}
		}
		printf("%li\n",nsec_temp);
		//if(nsec_temp>100000 && nsec_temp<500000){
		if(nsec_temp<500000){
			array2[i/2]=0;
		}else{
			//if(nsec_temp>600000 && nsec_temp<1000000){
			if(nsec_temp>500000){
				array2[i/2]=1;
			}else{
				return;
			}
		}
	}
	code=0;
	for(i=0;i<32;i++){
		if(array2[i]=='1'){
			code+=pow(2,31-i);
		}
	}
	//compare with other arrays (binary)
	// if match run scripts accordingly
	printf("%i\n",code);
	for(i=0;i<data.length;i++){
		if(data.codes[i]==code){
			system(data.commands[i]);
		}
	}
	return;
}

void register_swap(){
	static int i=0;
 	static struct timespec array[64];
	clock_gettime(CLOCK_MONOTONIC, &array[i]);
	if(i!=0 && array[i-1].tv_sec-array[i].tv_sec>1){
		i=0;
	}
	i++;
	if(i>=64){
		eval(array);
		i=0;
	}
	return;
}



 int main(){
 	//int i;
 	int pin=0;
	wiringPiSetup();
  	pinMode(pin, OUTPUT);
	wiringPiISR(pin, INT_EDGE_BOTH, &register_swap);
	read_config("config.txt");
	while(1){
		sleep(1);
	}
 	return 0;
 }

