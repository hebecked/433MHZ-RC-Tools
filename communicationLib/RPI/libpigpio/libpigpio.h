#ifndef libpigpio_h__
#define libpigpio_h__
 
extern void setup_io();
extern void set_in(int pin);
extern void set_out(int pin);
extern void switch_gpio(int val, int pin);
extern int check_gpio(int pin);

#endif  // libpigpio_h__

