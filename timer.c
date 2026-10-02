#include "timer.h"

extern void enable_interrupt(void);

void intInit(void) {
  unsigned int cycles = 3000000; // 100 ms is 3 million for a 30Mhz processor
  timer[2] = cycles & 0x0000FFFF; // mask the higher bits keep the lower bits 
  timer[3] = (cycles >> 16) & 0x0000FFFF; // mask the lower bits keep the higher bits
  timer[0] = 0; // clear timeout
  timer[1] = 0x7; // Start and Cont and ITO
  enable_interrupt();
  return;
}
