#include "timer.h"

// from lab 3

extern void enable_interrupt(void);

void intInit(void) {
  unsigned int cycles = 1000000; // 100 ms is 3 million for a 30Mhz processor
  timer[2] = cycles & 0x0000FFFF; // mask the higher bits keep the lower bits 
  timer[3] = (cycles >> 16) & 0x0000FFFF; // mask the lower bits keep the higher bits
  timer[0] = 0; // clear timeout
  timer[1] = 0x7; // Start and Cont and ITO

  *switchInterrupts = 0x200;
  *switchEdge = 0x0;
  enable_interrupt();
  return;
}