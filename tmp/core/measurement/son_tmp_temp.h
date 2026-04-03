#ifndef son_tmp_TEMP_H
#define son_tmp_TEMP_H

#include <stdint.h>
#include <stdbool.h>
#include "../../system/son_tmp_config.h"

// ============================================================================
// 温度データ用パケット構造体 (64Byte)
// ============================================================================
typedef struct
{
    uint8_t packetNum;       // パケット番号 (1 Byte)
    uint8_t tempData[62];    // 温度データ本体 (62 Byte = 12bit x 41データ)
    uint8_t crc8;            // CRC-8 (1 Byte)
} TempPacket;

// メモリバッファ用共用体
typedef union
{
    uint8_t raw[PACKET_SIZE];
    TempPacket temp;
} TempPacketBuffer;

// ============================================================================
// 外部公開関数プロトタイプ
// ============================================================================

// 内蔵ADCの初期化
void temp_io_init(void);

// 内蔵ADCから温度センサーの値を読み取る (12bit)
uint16_t read_adc_internal(void);

// 温度計測シーケンスの実行とFlashへの保存
// startPacketNum: 開始パケット番号 (19)
// packetCount: 実行するパケット数 (14)
void execute_temp_measurement(uint8_t startPacketNum, uint8_t packetCount);

#endif // son_tmp_TEMP_H