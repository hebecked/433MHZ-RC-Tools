// gcc -Wall -o test main.c -lpigpio
// sudo ./test

#include <stdio.h>
#include <unistd.h>
#include "libpigpio.h"

int main(int argc, char **argv)
{
  int g,rep;

  setup_io();

  // Set GPIO pins 7-11 to output
  for (g=7; g<=11; g++)
  {
    set_out(g);
  }

  for (rep=0; rep<10; rep++)
  {
     for (g=7; g<=11; g++)
     {
       switch_gpio(1, g);
       sleep(1);
     }
     for (g=7; g<=11; g++)
     {
       switch_gpio(0, g);
       sleep(1);
     }
  }

  return 0;

} // main
