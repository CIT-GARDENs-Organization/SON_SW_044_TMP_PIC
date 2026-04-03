#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// __________ values _________

// パケットヘッダーの timestamp で使用するための秒カウンタ
extern volatile uint32_t sec;


// __________ functions _________
uint32_t get_current_sec();
void set_current_sec(uint32_t new_sec);
void setup_timer();

#INT_TIMER1
void TIMER1_isr();

#endif