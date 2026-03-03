#INT_RDA

volatile unsigned int8 boss_receive_buffer[RECEIVE_BUFFER_MAX] = {0x00};
volatile int8 boss_receive_buffer_size = 0;

static void RDA_isr(void)
{
    // 受信したデータを1バイト読み取る
    char c = fgetc(BOSS);

    // BOSS(TeraTerm)にそのまま打ち返す (エコーバック)
    fputc(c, BOSS);

    // ついでにPC側のモニターにも表示する
    fputc(c, PC);
}

/*
static void RDA_isr(void)
{
    if (!(status == EXECUTING_MISSION || status == COPYING) || is_use_smf_req_in_mission)
        if (boss_receive_buffer_size < RECEIVE_BUFFER_MAX)
            boss_receive_buffer[boss_receive_buffer_size++ % RECEIVE_BUFFER_MAX] = fgetc(BOSS);

   // fprintf(PC, "RDA Interrupt: %d bytes received\r\n", boss_receive_buffer_size);
}
*/

void setup_uart_to_boss()
{
    fprintf(PC, "UART Initialize\r\n");
    enable_interrupts(INT_RDA);
    enable_interrupts(global);
    fprintf(PC, "\tComplete\r\n");
}

void clear_receive_signal(unsigned int8 receive_signal[], int8 *receive_signal_size)
{
    memset(receive_signal, 0x00, *receive_signal_size);
    *receive_signal_size = 0;
}
// End of file