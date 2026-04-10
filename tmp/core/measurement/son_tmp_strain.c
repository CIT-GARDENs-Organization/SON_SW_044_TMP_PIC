#include "son_tmp_strain.h"
#include "son_tmp_temp.h" // 温度計測モジュールをインクルード

// 必要な外部モジュールのインクルード
#include "../../hardware/mcu/timer.h"
#include "../storage/son_tmp_flash.h"
#include "../../../lib/tool/calc_tools.h"
#include "../../../lib/device/mt25q.h"

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
// ポーリング監視用関数 (UARTバッファからデータを拾う)
// ============================================================================
void check_boss_status_polling(void)
{
    static uint8_t rx_state = 0;
    static uint8_t expected_len = 0;

    while (kbhit(BOSS))
    {
        uint8_t c = fgetc(BOSS);

        if (rx_state == 0 && c == 0xAA) {
            rx_state = 1;
        }
        else if (rx_state == 1) {
            if (c == 0xA1 || c == 0xC1) {
                expected_len = 3;
                rx_state = 2;
            }
            else if (c == 0xA3 || c == 0xC3) {
                expected_len = 7;
                rx_state = 2;
            }
            else {
                rx_state = (c == 0xAA) ? 1 : 0;
            }
        }
        else if (rx_state >= 2) {
            rx_state++;

            if (rx_state == expected_len) {
                fprintf(PC, "\r\n[INFO] STATUS/SYNC Received during measurement. Sending Status...\r\n");
                transmit_status();
                rx_state = 0;
            }
        }
    }
}

// ============================================================================
// チラ見機能付きの待機関数 (バッファ溢れ防止)
// ============================================================================
void delay_ms_with_polling(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        delay_ms(1);
        check_boss_status_polling();
    }
}

// ============================================================================
// 計測シーケンスとFlash保存処理
// ============================================================================
void execute_measurement(uint8_t mode, uint8_t hw_channel, uint8_t samplingRate)
{
    fprintf(PC, "Start Synchronized Measurement (Mode:0x%02X, HW_Ch:%u)\r\n", mode, hw_channel);

    temp_io_init();
    delay_ms(10);

    // 0. 測定開始時に電源・VREF・LEDをONにする
    output_high(PIN_VREF_EN);
    output_high(PIN_LDO_EN);
    output_high(PIN_LED2);
    output_high(PIN_LED1);

    // 1. バッファとパケット管理変数の初期化
    uint16_t data_count = 319;
    uint16_t packet_num = 0;
    uint8_t packet_buffer[PACKET_SIZE];
    uint8_t packet_idx = 0;
    uint8_t block_count = 0;
    uint8_t data_num = 0;

    memset(packet_buffer, 0, PACKET_SIZE);

    // パケット0のヘッダー作成
    uint32_t ts = get_current_sec();
    packet_buffer[0] = (ts >> 24) & 0xFF;
    packet_buffer[1] = (ts >> 16) & 0xFF;
    packet_buffer[2] = (ts >> 8) & 0xFF;
    packet_buffer[3] = ts & 0xFF;
    packet_buffer[4] = ((mode & 0x07) << 5) | ((hw_channel & 0x01) << 4) | (samplingRate & 0x0F);
    packet_buffer[5] = (data_count >> 4) & 0xFF;
    packet_buffer[6] = ((data_count & 0x0F) << 4) | 0x00;

    packet_idx = 7;

    // 2. メインサンプリングループ
    for (uint16_t step = 0; step < data_count; step++)
    {
        // 測定開始前にも一度チラ見する
        check_boss_status_polling();

        // [A] データの読み取り
        uint16_t temp_val = read_adc_internal();

        uint16_t strain1 = read_adc_ltc2452();

        switch_channel(hw_channel);
        delay_us(10);

        uint16_t strain2 = read_adc_ltc2452();

        // CMD_STR_PRINT (0xA2) の場合は、シリアル出力するだけ
        if (mode == 0x03)
        {
            fprintf(PC, "[PRINT] Step:%lu, Temp:0x%03X, STR1:0x%04X, STR2:0x%04X\r\n",
                    step, temp_val, strain1, strain2);
        }

        // [B] 6Byteデータ塊のパッキング
        packet_buffer[packet_idx++] = ((data_num & 0x0F) << 4) | ((temp_val >> 8) & 0x0F);
        packet_buffer[packet_idx++] = temp_val & 0xFF;
        packet_buffer[packet_idx++] = (strain1 >> 8) & 0xFF;
        packet_buffer[packet_idx++] = strain1 & 0xFF;
        packet_buffer[packet_idx++] = (strain2 >> 8) & 0xFF;
        packet_buffer[packet_idx++] = strain2 & 0xFF;

        data_num = (data_num + 1) & 0x0F;
        block_count++;

        // [C] 満杯判定と保存/出力処理
        uint8_t max_blocks = (packet_num == 0) ? 9 : 10;

        if (block_count >= max_blocks)
        {
            uint32_t crc24 = calc_crc24(packet_buffer, 61);
            packet_buffer[61] = (crc24 >> 16) & 0xFF;
            packet_buffer[62] = (crc24 >> 8) & 0xFF;
            packet_buffer[63] = crc24 & 0xFF;

            // ★修正: mode 0x01(保存のみ) と 0x04(保存+ダンプ) でFlashへ書き込み
            if (mode == 0x01 || mode == 0x04)
            {
                uint32_t flash_addr = MISF_TMP_STR_DATA_START + str_data.used_counter;
                write_data_bytes(mis_fm, flash_addr, (int8*)packet_buffer, PACKET_SIZE);

                str_data.used_counter += PACKET_SIZE;
                str_data.uncopied_counter += PACKET_SIZE;
            }

            // ★修正: mode 0x02(ダンプのみ) と 0x04(保存+ダンプ) でシリアルダンプ出力
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

        // [D] サンプリングレートに応じたウェイト
        switch (samplingRate)
        {
            case SAMP_RATE_10MS:   delay_ms_with_polling(10);   break;
            case SAMP_RATE_50MS:   delay_ms_with_polling(50);   break;
            case SAMP_RATE_100MS:  delay_ms_with_polling(100);  break;
            case SAMP_RATE_500MS:  delay_ms_with_polling(500);  break;
            case SAMP_RATE_1000MS: delay_ms_with_polling(1000); break;
            case SAMP_RATE_5000MS: delay_ms_with_polling(5000); break;
            default:               delay_ms_with_polling(10);   break;
        }
    }

    // 3. ループ終了後、端数の処理
    if (block_count > 0)
    {
        uint32_t crc24 = calc_crc24(packet_buffer, 61);
        packet_buffer[61] = (crc24 >> 16) & 0xFF;
        packet_buffer[62] = (crc24 >> 8) & 0xFF;
        packet_buffer[63] = crc24 & 0xFF;

        // ★修正: 端数の処理も独立したif文に変更
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

    // Flashのアドレス管理領域を一度だけセーブ (保存モードの場合のみ)
    if (mode == 0x01 || mode == 0x04)
    {
        write_misf_address_area();
    }

    // 4. 測定終了後にアナログ回路と内蔵ADCをOFFに戻す
    setup_adc_ports(NO_ANALOGS);
    setup_adc(ADC_OFF);

    output_low(PIN_VREF_EN);
    output_low(PIN_LDO_EN);
    output_low(PIN_LED2);
    output_low(PIN_LED1);

    fprintf(PC, "End Synchronized Measurement Sequence\r\n");
}