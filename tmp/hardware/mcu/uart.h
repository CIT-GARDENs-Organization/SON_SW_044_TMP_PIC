#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

// _________ defines ________________

#define RECEIVE_BUFFER_MAX 32
#define RX_RING_BUFFER_SIZE 64

// _____________ values _______________

extern volatile unsigned int8 boss_receive_buffer[RECEIVE_BUFFER_MAX];
extern volatile int8 boss_receive_buffer_size;

// _______________ functions ___________

void setup_uart_to_boss(void);
void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size);

// --- 割り込み受信によるリングバッファ用関数 ---
bool uart_has_data(void);
uint8_t uart_getc(void);
void uart_fetch_to_buffer(void); // リングバッファからリニアバッファへ安全に吸い上げる

#endif // UART_H