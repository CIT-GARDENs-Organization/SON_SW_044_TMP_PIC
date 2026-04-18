#include "../uart.h" // ヘッダーをインクルード

volatile unsigned int8 boss_receive_buffer[RECEIVE_BUFFER_MAX] = {0x00};
volatile int8 boss_receive_buffer_size = 0;

void setup_uart_to_boss(void)
{
    fprintf(PC, "UART Initialize\r\n");

    // コンパイラの制限によりフリーズの要因となるため、割り込みは使用しません
    // enable_interrupts(INT_RDA3);
    // enable_interrupts(GLOBAL);

    fprintf(PC, "\tComplete\r\n");
}

void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size)
{
    memset(receive_signal, 0x00, *receive_signal_size);
    *receive_signal_size = 0;
}