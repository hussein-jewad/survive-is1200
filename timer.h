#ifndef timerH
#define timerH
#define timer ((volatile int*) 0x04000020)
#define switches ((volatile int*) 0x04000010)
#define switchInterrupts ((volatile int*) 0x04000018)
#define switchEdge ((volatile int*) 0x0400001C)
void intInit(void);

#endif
