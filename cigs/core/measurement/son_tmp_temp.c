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

// ============================================================================
// 温度計測シーケンスとデータパッキング
// ============================================================================

/*
void execute_temp_measurement(uint8_t startPacketNum, uint8_t packetCount)
{
    TempPacketBuffer packet;
    uint8_t currentPacketNum = startPacketNum;
    uint8_t endPacketNum = startPacketNum + packetCount - 1;

    fprintf(PC, "Start Temp Measurement (Packets %u to %u)\r\n", startPacketNum, endPacketNum);

    while (currentPacketNum <= endPacketNum)
    {
        memset(packet.raw, 0, PACKET_SIZE);
        packet.temp.packetNum = currentPacketNum;

        uint8_t idx = 0;

        // 1. 2つの12bitデータを3Byteに圧縮する処理 (20ペア = 40データ)
        for (uint8_t i = 0; i < 20; i++)
        {
            uint16_t temp1 = read_adc_internal();
            uint16_t temp2 = read_adc_internal();

            // temp1の上位8bit
            packet.temp.tempData[idx++] = (temp1 >> 4) & 0xFF;
            // temp1の下位4bit + temp2の上位4bit
            packet.temp.tempData[idx++] = ((temp1 & 0x0F) << 4) | ((temp2 >> 8) & 0x0F);
            // temp2の下位8bit
            packet.temp.tempData[idx++] = temp2 & 0xFF;
        }

        // 2. 最後の2Byteに1つの12bitデータと4bitの0fillを格納 (41データ目)
        uint16_t temp3 = read_adc_internal();
        packet.temp.tempData[idx++] = (temp3 >> 4) & 0xFF;
        packet.temp.tempData[idx++] = (temp3 & 0x0F) << 4; // 残り4bitは自動的に0fill

        // 3. CRC-8の計算と格納
        packet.temp.crc8 = calc_crc8(packet.raw, PACKET_SIZE - 1);

        // 4. Flashへ書き込み (IVデータ領域にそのまま連ねて保存する)
        uint32_t write_address = MISF_CIGS_IV_DATA_START + iv_data.used_counter;
        write_data_bytes(mis_fm, write_address, packet.raw, PACKET_SIZE);

        // カウンタの更新
        iv_data.used_counter += PACKET_SIZE;
        iv_data.uncopied_counter += PACKET_SIZE;

        currentPacketNum++;

        // 計測間隔のウェイト (必要に応じて調整)
        delay_ms(1);
    }

    fprintf(PC, "End Temp Measurement\r\n");
}
*/