#include "son_tmp_temp.h"
#include "../../hardware/mcu/timer.h"
#include "../storage/son_tmp_flash.h"
#include "../../../lib/tool/calc_tools.h"
#include "../../../lib/device/mt25q.h"

// ============================================================================
// ハードウェア・ペリフェラル制御 (PIC内蔵ADC)
// ============================================================================

void temp_io_init(void)
{
    fprintf(PC, "Temp IO Initialize\r\n");

    // ※ 実際のハードウェア配線に合わせて sANx (アナログピン) を調整してください
    // ここでは仮に AN0 (sAN0) を使用する設定としています
    setup_adc_ports(sAN0);
    setup_adc(ADC_CLOCK_INTERNAL);
    set_adc_channel(0);

    fprintf(PC, "\tComplete\r\n");
}

uint16_t read_adc_internal(void)
{
    // ADCのチャネル切り替え・サンプリング安定待ち
    delay_us(20);

    // PIC18F67J94の12bit ADC値を読み取って返す
    return read_adc();
}
