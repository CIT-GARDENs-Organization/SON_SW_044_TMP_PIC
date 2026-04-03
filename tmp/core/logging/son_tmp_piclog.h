#ifndef son_tmp_PICLOG_H
#define son_tmp_PICLOG_H

#include <stdint.h>
#include <string.h>

// ============================================================================
// PICLOG 定数定義
// ============================================================================
#define PICLOG_STARTUP     0x00
#define PICLOG_PARAM_START 0x00
#define PICLOG_PARAM_END   0xFF

#define PICLOG_PACKET_SIZE 6
#define PICLOG_BLANK_SIZE  4

// ============================================================================
// PICLOG データ構造体 (6 Byte)
// ============================================================================
typedef union
{
    struct
    {
        uint32_t time;       // 4 Byte: タイムスタンプ
        uint8_t  function;   // 1 Byte: 実行関数・イベントID
        uint8_t  parameter;  // 1 Byte: パラメータ・状態
    } fields;
    uint8_t bytes[PICLOG_PACKET_SIZE];
} PICLOG_t;

// ============================================================================
// 外部公開関数プロトタイプ
// ============================================================================

// ログを作成し、フラッシュメモリに書き込む
// function: イベントID
// parameter: 状態パラメータ
void piclog_make(uint8_t function, uint8_t parameter);

#endif // son_tmp_PICLOG_H