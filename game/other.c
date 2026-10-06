#include "timer.h"
// random functions from lab 3 that may be useful
#include <stdbool.h>
const unsigned char seg[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; 
// lookup table for 7seg display

void set_display(int display_number, int value) {
 volatile unsigned char *dp1 = (volatile unsigned char*) (0x04000050 + (0x10 * display_number)); // each 7seg address is 16 bits apart
  *dp1 = seg[(value & 0x0F)]; // mask out everything except 4 least significant bits to convert from ascii to normal integer this integer is then mapped correctly as an index to our lookup table
}

int get_sw(void) {
  unsigned int result = *switches; // switches sit next to eachother by bit where sw0 is the least singificant bit
  result = result & 0x3FF; // mask out all other bits except our 10 bits
  return result;
}

int get_btn(void) {
  volatile unsigned char *btnptr = (volatile unsigned char*) 0x040000d0;
  unsigned int result = *btnptr & 0x1; // mask out other bits; button 0 means not pressed, 1 means pressed
  return result;
}

bool get_btn_pressed(void) { // help by ai
  static bool wasPressed = false;
  static unsigned int cooldownFrames = 0;
  const bool pressed = get_btn() != 0;
  const bool newPress = pressed && !wasPressed;
  wasPressed = pressed;

  if (cooldownFrames > 0) {
    cooldownFrames--;
    return false;
  }
  if (!newPress) return false;

  cooldownFrames = 9;
  return true;
}