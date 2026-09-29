#ifndef timerH
#define timerH

volatile int* timer = (volatile int*) 0x04000020;

void intInit(void);

#endif
