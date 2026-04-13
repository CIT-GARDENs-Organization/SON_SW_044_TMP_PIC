#ifndef SON_TMP_FLASH_H
#define SON_TMP_FLASH_H

#include <stdint.h>
#include <string.h>
#include "../../system/son_tmp_config.h"

// ============================================================================
// 新しいメモリマップ定義 (MT25QL128ABA: 16MB)
// ============================================================================
#define MISF_START 0x00000000
#define MISF_END   0x00FFFFFF

#define SECTOR_64K_BYTE  0x10000

// ----------------------------------------------------
// 1. 管理用データ領域 (4KB サブセクタ × 2)
// ----------------------------------------------------
#define MISF_TMP_DATA_TABLE_SIZE     64
#define MISF_TMP_DATA_TABLE_START    0x00000000
#define MISF_TMP_DATA_TABLE_END      0x00000FFF // 1つ目 (4KB)
#define MISF_TMP_DATA_TABLE_BK_START 0x00001000 // 2つ目 (バックアップ/退避用: 4KB)
#define MISF_TMP_DATA_TABLE_BK_END   0x00001FFF

// ----------------------------------------------------
// 2. PICLOG 領域 (64KB セクタ × 1)
// ※週1回の運用で1年間に約3.3KB消費。約19年分保存可能。
// ----------------------------------------------------
#define MISF_TMP_PICLOG_START        0x00010000
#define MISF_TMP_PICLOG_END          0x0001FFFF

// ----------------------------------------------------
// 3. STRデータ領域 (残りすべて: 約15.8MB)
// ※広大なローカルバッファとして使用。未送信分をここからCPLDへ送る。
// ----------------------------------------------------
#define MISF_TMP_STR_DATA_START      0x00020000
#define MISF_TMP_STR_DATA_END        0x00FFFFFF

// ============================================================================
// データ構造体定義
// ============================================================================

// フラッシュカウンタ構造体 (11 Bytes)
typedef struct
{
    uint8_t   mission_id;
    uint32_t  used_counter;
    uint32_t  uncopied_counter;
    uint8_t   reserve_counter1;
    uint8_t   reserve_counter2;
} Flash_t;

// テーブル保存用の64Byteパケット構造体
typedef union
{
    struct
    {
        union
        {
            struct
            {
                Flash_t piclog;
                Flash_t str_data;
            } logdata;
            uint8_t reserve[63];
        } payload;
        uint8_t crc; // 1バイトのCRC
    } packet;
    uint8_t bytes[64];
} FlashData_t;

typedef struct
{
    uint32_t start_address;
    uint32_t end_address;
} MisfStatusStruct;

typedef struct
{
    uint32_t start_address;
    uint32_t size;
} MisfWriteStruct;

// ============================================================================
// グローバル変数 (宣言のみ、実体は.cファイルに記述)
// ============================================================================
extern Flash_t piclog_data;
extern Flash_t str_data;

// ============================================================================
// 外部公開関数プロトタイプ
// ============================================================================
void misf_init(void);
void write_misf_address_area(void);
MisfStatusStruct get_misf_status_struct(uint8_t mission_id);
MisfWriteStruct get_misf_write_struct(uint8_t mission_id);
void print_flash_status(void);

#endif // SON_TMP_FLASH_H