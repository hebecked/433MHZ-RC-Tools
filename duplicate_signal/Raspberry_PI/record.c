// Small code snipped to play recorded control signals for 433 MHz (and other) Modules
// Code written by Dustin Hebecker (https://github.com/hebecked)
// The Software is provided "as is," with all faults, defects and errors, and without warranty of any kind.
// Feel free to share and modify as long as the original author is given credit.
// To compile run:
//gcc -Wall -pedantic -O2 -o record record.c -std=gnu99 -lwiringPi

#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <stdlib.h>

int main (){
	if (wiringPiSetup () == -1){
		printf("Error!!!\n");
    		return 1 ;
	}

	int i,start;
	int array[10000];
  	pinMode (0, INPUT) ;         // aka BCM_GPIO pin 17
	
	printf ("Recording in ...") ;
	fflush(stdout);
	for(i=0;i<3;i++){
		printf(" %i",i+1);
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	
	for (start=clock(),i=0;((double) clock()-start)/CLOCKS_PER_SEC<1;i++){
		array[i]=digitalRead (0);
		delayMicroseconds(100); // muS
	}

	FILE* output=fopen("signal.txt","w");
	if(output==NULL)printf("ERROR!!!");

	for(i=0;i<1000;i++){
		fprintf(output,"%i\n",array[i]);
	}

	fclose(output);
	
	return 0 ;
}
