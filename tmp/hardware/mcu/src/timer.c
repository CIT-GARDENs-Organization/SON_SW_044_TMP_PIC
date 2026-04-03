// 一つ上の階層にある timer.h をインクルード
#include "../timer.h"

volatile uint32_t sec = 0;
volatile uint8_t timer_tick_100ms = 0;

#INT_TIMER1
void TIMER1_isr()
{
    set_timer1(15536);
    timer_tick_100ms++;

    if (timer_tick_100ms >= 10)
    {
        timer_tick_100ms = 0;
        sec++;
    }
}

void setup_timer()
{
    fprintf(PC, "Timer Initialize (16MHz Main Clock Base)\r\n");
    clear_interrupt(INT_TIMER1);

    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    set_timer1(15536);

    timer_tick_100ms = 0;

    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);

    fprintf(PC, "\tComplete\r\n");
}

void set_current_sec(uint32_t new_sec)
{
    sec = new_sec;
}

uint32_t get_current_sec()
{
    return sec;
}