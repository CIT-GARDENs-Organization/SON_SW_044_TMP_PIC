#include "son_tmp_strain.h"
#include "son_tmp_temp.h" // 温度計測モジュールをインクルード

// 必要な外部モジュールのインクルード
#include "../../hardware/mcu/timer.h"
#include "../storage/son_tmp_flash.h"
#include "../../../lib/tool/calc_tools.h"
#include "../../../lib/device/mt25q.h"

extern Flash mis_fm;
extern bool is_mission_aborted;

// ============================================================================
// ハードウェア・ペリフェラル制御
// ============================================================================

void io_init()
{
    fprintf(PC, "IO Initialize\r\n");

    // SPI Chip Select の初期化 (Highで非選択)
    output_high(PIN_CS_ADC);
    output_high(MIS_FM_CS);
    output_high(SMF_CS);

    // 電源・制御ピンの初期化 (LowでOFF)
    output_low(PIN_LDO_EN);
    output_low(PIN_VREF_EN);

    // アナログスイッチの初期化
    output_low(PIN_SW_DIO0);
    output_low(PIN_SW_DIO1);

    // テレメトリLEDの初期化
    output_low(PIN_LED1);
    output_low(PIN_LED2);

    // 内蔵ADC(温度センサ用)の初期化もここで呼び出す
    temp_io_init();

    delay_ms(1);
    fprintf(PC, "\tComplete\r\n");
}

void switch_channel(uint8_t ch)
{
    // MAX4734EUB+ は 2bit (DIO1, DIO0) で 4ch を切り替える
    if (ch & 0x01)
    {
        output_high(PIN_SW_DIO0);
    }
    else
    {
        output_low(PIN_SW_DIO0);
    }

    if (ch & 0x02)
    {
        output_high(PIN_SW_DIO1);
    }
    else
    {
        output_low(PIN_SW_DIO1);
    }

    // スイッチ切り替え後の安定待ち
    delay_us(10);
}

uint16_t read_adc_ltc2452()
{
    uint16_t adc_val = 0;

    output_low(PIN_CS_ADC);

    // LTC2452は16bitデータを出力 (MSBファースト)
    adc_val = spi_xfer(MIS_FM_STREAM, 0x00);                  // 上位8bit
    adc_val = (adc_val << 8) | spi_xfer(MIS_FM_STREAM, 0x00); // 下位8bit

    output_high(PIN_CS_ADC);

    return adc_val;
}

// ============================================================================
// ポーリング監視用関数 (中身は excute_mission.c にあるので extern 宣言のみ)
// ============================================================================
extern void check_boss_status_polling(void);

// ============================================================================
// チラ見機能付きの待機関数 (オーバーヘッド削減版)
// ============================================================================
void delay_ms_with_polling(uint16_t ms)
{
    uint16_t loops = ms / 50;
    uint16_t remainder = ms % 50;

    for (uint16_t i = 0; i < loops; i++)
    {
        if (is_mission_aborted) return;

        delay_ms(50);
        check_boss_status_polling();
    }

    if (!is_mission_aborted && remainder > 0)
    {
        delay_ms(remainder);
        check_boss_status_polling();
    }
}

// ============================================================================
// 計測シーケンスとFlash保存処理
// ============================================================================
void execute_measurement(uint8_t mode, uint8_t samplingRate)
{
    fprintf(PC, "Start Synchronized Measurement (Mode:0x%02X)\r\n", mode);

    temp_io_init();
    delay_ms(10);

    output_high(PIN_VREF_EN);
    output_high(PIN_LDO_EN);
    output_high(PIN_LED2);
    output_high(PIN_LED1);

    uint16_t data_count = 319;
    uint16_t packet_num = 0;
    uint8_t packet_buffer[PACKET_SIZE];
    uint8_t packet_idx = 0;
    uint8_t block_count = 0;
    uint8_t data_num = 0;

    memset(packet_buffer, 0, PACKET_SIZE);

    uint32_t ts = get_current_sec();

    packet_buffer[0] = (ts >> 24) & 0xFF;
    packet_buffer[1] = (ts >> 16) & 0xFF;
    packet_buffer[2] = (ts >> 8) & 0xFF;
    packet_buffer[3] = ts & 0xFF;
    packet_buffer[4] = ((mode & 0x07) << 5) | (0x00 << 4) | (samplingRate & 0x0F);
    packet_buffer[5] = (data_count >> 4) & 0xFF;
    packet_buffer[6] = ((data_count & 0x0F) << 4) | 0x00;

    packet_idx = 7;

    // 2. メインサンプリングループ
    for (uint16_t step = 0; step < data_count; step++)
    {
        if (is_mission_aborted)
        {
            fprintf(PC, "\r\n[INFO] Measurement aborted at step %lu.\r\n", (unsigned long)step);
            break;
        }

        check_boss_status_polling();

        // ----------------------------------------------------
        // [A] データの読み取り (CH1/CH2 固定)
        // ----------------------------------------------------
        uint16_t temp_val = read_adc_internal();

        // --- CH1 ---
        switch_channel(0);
        delay_us(100);             // アンプの出力が安定するまで待つ
        read_adc_ltc2452();        // ★ダミーリード (空読みをしてCH1の電圧変換をスタートさせる)
        delay_ms(25);              // LTC2452の最大変換時間(23ms)を確実に待つ
        uint16_t strain1 = read_adc_ltc2452(); // CH1の正しいデータを取得

        // --- CH2 ---
        switch_channel(1);
        delay_us(100);             // アンプの出力が安定するまで待つ
        read_adc_ltc2452();        // ★ダミーリード (空読みをしてCH2の電圧変換をスタートさせる)
        delay_ms(25);              // LTC2452の最大変換時間(23ms)を確実に待つ
        uint16_t strain2 = read_adc_ltc2452(); // CH2の正しいデータを取得

        if (mode == 0x03)
        {
            fprintf(PC, "[PRINT] Step:%lu, Temp:%lu, STR1:%lu, STR2:%lu\r\n",
                    step, temp_val, strain1, strain2);
        }

        // ----------------------------------------------------
        // [B] パッキングと保存処理
        // ----------------------------------------------------
        packet_buffer[packet_idx++] = ((data_num & 0x0F) << 4) | ((temp_val >> 8) & 0x0F);
        packet_buffer[packet_idx++] = temp_val & 0xFF;
        packet_buffer[packet_idx++] = (strain1 >> 8) & 0xFF;
        packet_buffer[packet_idx++] = strain1 & 0xFF;
        packet_buffer[packet_idx++] = (strain2 >> 8) & 0xFF;
        packet_buffer[packet_idx++] = strain2 & 0xFF;

        data_num = (data_num + 1) & 0x0F;
        block_count++;

        uint8_t max_blocks = (packet_num == 0) ? 9 : 10;

        if (block_count >= max_blocks)
        {
            uint32_t crc24 = calc_crc24(packet_buffer, 61);
            packet_buffer[61] = (crc24 >> 16) & 0xFF;
            packet_buffer[62] = (crc24 >> 8) & 0xFF;
            packet_buffer[63] = crc24 & 0xFF;

            if (mode == 0x01 || mode == 0x04)
            {
                uint32_t flash_addr = MISF_TMP_STR_DATA_START + str_data.used_counter;
                write_data_bytes(mis_fm, flash_addr, (int8*)packet_buffer, PACKET_SIZE);

                str_data.used_counter += PACKET_SIZE;
                str_data.uncopied_counter += PACKET_SIZE;
            }

            if (mode == 0x02 || mode == 0x04)
            {
                fprintf(PC, "[DEBUG] Packet %lu: ", packet_num);
                for (uint8_t i = 0; i < PACKET_SIZE; i++)
                {
                    fprintf(PC, "%02X ", packet_buffer[i]);
                }
                fprintf(PC, "\r\n");
            }

            packet_num++;
            memset(packet_buffer, 0, PACKET_SIZE);
            packet_buffer[0] = packet_num & 0xFF;
            packet_idx = 1;
            block_count = 0;
        }

        // ★修正: 10ms, 50ms の設定を削除し、不正値が渡された場合のデフォルトも 100ms に引き上げました
        switch (samplingRate)
        {
            case SAMP_RATE_100MS:  delay_ms_with_polling(100);  break;
            case SAMP_RATE_500MS:  delay_ms_with_polling(500);  break;
            case SAMP_RATE_1000MS: delay_ms_with_polling(1000); break;
            case SAMP_RATE_2432MS: delay_ms_with_polling(2432); break;
            case SAMP_RATE_4865MS: delay_ms_with_polling(4865); break;
            case SAMP_RATE_5000MS: delay_ms_with_polling(5000); break;
            case SAMP_RATE_9730MS: delay_ms_with_polling(9730); break;
            default:               delay_ms_with_polling(100);  break;
        }
    }

    if (block_count > 0)
    {
        uint32_t crc24 = calc_crc24(packet_buffer, 61);
        packet_buffer[61] = (crc24 >> 16) & 0xFF;
        packet_buffer[62] = (crc24 >> 8) & 0xFF;
        packet_buffer[63] = crc24 & 0xFF;

        if (mode == 0x01 || mode == 0x04)
        {
            uint32_t flash_addr = MISF_TMP_STR_DATA_START + str_data.used_counter;
            write_data_bytes(mis_fm, flash_addr, (int8*)packet_buffer, PACKET_SIZE);

            str_data.used_counter += PACKET_SIZE;
            str_data.uncopied_counter += PACKET_SIZE;
        }

        if (mode == 0x02 || mode == 0x04)
        {
            fprintf(PC, "[DEBUG] Packet %lu (Fraction): ", packet_num);
            for (uint8_t i = 0; i < PACKET_SIZE; i++)
            {
                fprintf(PC, "%02X ", packet_buffer[i]);
            }
            fprintf(PC, "\r\n");
        }
    }

    if (mode == 0x01 || mode == 0x04)
    {
        write_misf_address_area();
    }

    setup_adc_ports(NO_ANALOGS);
    setup_adc(ADC_OFF);

    output_low(PIN_VREF_EN);
    output_low(PIN_LDO_EN);
    output_low(PIN_LED2);
    output_low(PIN_LED1);

    fprintf(PC, "End Synchronized Measurement Sequence\r\n");
}