#include "son_tmp_mode_flash.h"
#include "son_tmp_excute_mission.h"
#include "../../core/logging/son_tmp_piclog.h"
#include "../../core/storage/son_tmp_smf.h"
#include "../../../lib/tool/mmj_smf_memorymap.h"


// ============================================================================
// SMF（CPLD経由）転送シーケンス
// ============================================================================

void prepare_smf_transfer(void)
{
    fprintf(PC, "--- SMF Transfer Prepare ---\r\n");

    // 準備開始のログ記録 (イベントID: 0x20, パラメータ: 0x00 と仮定)
    piclog_make(0x20, 0x00);

    // 状態を COPYING に遷移 (excute_mission.c 側でも遷移済みだが念のため)
    //status = COPYING;

    // TODO: cigs_smf.c 側の転送準備処理（転送先頭アドレスの計算やキューの準備）を呼び出す
    // cigs_smf_prepare();
    cigs_smf_prepare();
}

void execute_smf_transfer(void)
{
    fprintf(PC, "--- SMF Transfer Execute ---\r\n");

    // 転送開始のログ記録 (イベントID: 0x21, パラメータ: 0x00 と仮定)
    piclog_make(0x21, 0x00);

    cigs_smf_copy();
}

void permit_smf_transfer(void)
{
    fprintf(PC, "--- SMF Transfer Permit ---\r\n");

    // 許可のログ記録 (イベントID: 0x22, パラメータ: 0x00 と仮定)
    piclog_make(0x22, 0x00);

    cigs_smf_permit();

    // 一連の転送シーケンスが完了したため、システム状態を IDLE に戻す
    status = IDLE;

    fprintf(PC, "--- SMF Transfer Sequence Complete ---\r\n");
}

// ============================================================================
// Flash保存データ全読み出し (デバッグ用)
// ============================================================================
void execute_flash_dump(void)
{
    unsigned int8 read_buffer[PACKET_SIZE];
    unsigned int32 flash_addr;
    unsigned int16 p_num;
    unsigned int16 i;
    unsigned int32 r_len;

    int8 *p_dst;
    p_dst = (int8 *)read_buffer;
    r_len = (unsigned int32)PACKET_SIZE;

    fprintf(PC, "\r\n--- Flash Data Dump Start ---\r\n");

    for (p_num = 1; p_num <= 32; p_num++)
    {
        // 1. アドレス計算
        flash_addr = (unsigned int32)(p_num - 1);
        flash_addr *= r_len;

        // 2. Error 54対策: FLASH定数ではなく、ストリーム変数 mis_fm を渡す
        read_data_bytes(mis_fm, flash_addr, p_dst, r_len);

        // 3. Error 114対策: 32bitアドレスを確実に動く 8bit x 4つ に完全に分解する
        unsigned int8 a3 = (unsigned int8)((flash_addr >> 24) & 0xFF);
        unsigned int8 a2 = (unsigned int8)((flash_addr >> 16) & 0xFF);
        unsigned int8 a1 = (unsigned int8)((flash_addr >> 8)  & 0xFF);
        unsigned int8 a0 = (unsigned int8)(flash_addr         & 0xFF);

        // パケット番号も 8bit にキャスト (1〜32なので問題なし)
        unsigned int8 p_num_8 = (unsigned int8)p_num;

        // 全て 8bit 用のフォーマット (%u, %02X) に統一して出力
        fprintf(PC, "Packet [%u] ", p_num_8);
        fprintf(PC, "Addr: 0x%02X%02X%02X%02X | ", a3, a2, a1, a0);

        for (i = 0; i < PACKET_SIZE; i++)
        {
            fprintf(PC, "%02X ", read_buffer[i]);

            if (((i + 1) % 16 == 0) && (i < (PACKET_SIZE - 1)))
            {
                fprintf(PC, "\r\n                         | ");
            }
        }
        fprintf(PC, "\r\n------------------------------------------------------------\r\n");

        delay_ms(10);
    }

    fprintf(PC, "--- Flash Data Dump Complete ---\r\n");
}