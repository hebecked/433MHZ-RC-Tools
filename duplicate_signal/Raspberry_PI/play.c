##gcc -Wall -pedantic -O2 -o play play.c -std=gnu99 -lwiringPi

#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <stdlib.h>

int main (int argc, char* argv[]){
	if (wiringPiSetup () == -1){
		printf("Error!!!\n");
    		return 1 ;
	}

	if(argc<2)printf("ERROR!!!\n");

	int i;
	int array[10000];
  	pinMode (1, OUTPUT) ;         // aka BCM_GPIO pin 17
		
	FILE* input=fopen(argv[1],"r");
	if(input==NULL)printf("ERROR!!!\n");
	for (i=0;i<1000;i++){
		fscanf(input,"%i\n",&array[i]);
	}
	fclose(input);

	for (i=0;i<1000;i++){
		digitalWrite(1,array[i]);
		delayMicroseconds(100); // muS
	}
	
	return 0 ;
}
