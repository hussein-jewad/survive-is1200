#ifndef otherH
#define otherH

const unsigned char seg[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; 
// lookup table for 7seg display

void set_display(int display_number, int value);

int get_sw(void);

int get_btn(void);

#endif
