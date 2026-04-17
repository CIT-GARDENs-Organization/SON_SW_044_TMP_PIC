#include "../uart.h" // ヘッダーをインクルード

// リングバッファの定義 (割り込みで受信したデータを一時保存)
volatile uint8_t rx_ring_buffer[RX_RING_BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

// 従来互換用リニアバッファ
volatile unsigned int8 boss_receive_buffer[RECEIVE_BUFFER_MAX] = {0x00};
volatile int8 boss_receive_buffer_size = 0;

// ----------------------------------------------------
// UART3 受信割り込みハンドラ
// ----------------------------------------------------
#INT_RDA3
void RDA_isr(void)
{
    // ハードウェアから受信したデータを直ちにリングバッファに格納
    uint8_t c = fgetc(BOSS);
    uint8_t next_head = (rx_head + 1) % RX_RING_BUFFER_SIZE;

    // バッファに空きがあれば格納 (オーバーフロー時は古いデータを保護し新しいものを破棄)
    if (next_head != rx_tail)
    {
        rx_ring_buffer[rx_head] = c;
        rx_head = next_head;
    }
}

void setup_uart_to_boss()
{
    fprintf(PC, "UART Initialize\r\n");

    rx_head = 0;
    rx_tail = 0;
    boss_receive_buffer_size = 0;

    // 割り込み駆動を有効化
    enable_interrupts(INT_RDA3);
    enable_interrupts(GLOBAL);

    fprintf(PC, "\tComplete\r\n");
}

// ----------------------------------------------------
// リングバッファ操作関数
// ----------------------------------------------------
bool uart_has_data(void)
{
    return (rx_head != rx_tail);
}

uint8_t uart_getc(void)
{
    uint8_t c = 0;
    if (rx_head != rx_tail)
    {
        c = rx_ring_buffer[rx_tail];
        rx_tail = (rx_tail + 1) % RX_RING_BUFFER_SIZE;
    }
    return c;
}

// ----------------------------------------------------
// 割り込みで貯まったデータをパケット単位で吸い上げる
// ----------------------------------------------------
void uart_fetch_to_buffer(void)
{
    if (uart_has_data())
    {
        uint8_t timeout_ms = 0;
        // データが途切れる（5ms間無通信）まで全力で吸い上げる
        while(timeout_ms < 5)
        {
            if (uart_has_data()) {
                if (boss_receive_buffer_size < RECEIVE_BUFFER_MAX) {
                    boss_receive_buffer[boss_receive_buffer_size++] = uart_getc();
                } else {
                    uart_getc(); // 満杯時は捨てる
                }
                timeout_ms = 0;
            } else {
                delay_ms(1);
                timeout_ms++;
            }
        }
    }
}

void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size)
{
    memset(receive_signal, 0x00, *receive_signal_size);
    *receive_signal_size = 0;
}