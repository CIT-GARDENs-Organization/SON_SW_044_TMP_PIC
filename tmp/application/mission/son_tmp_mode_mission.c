#include "son_tmp_mode_mission.h"
#include "../../core/measurement/son_tmp_strain.h" // ※環境に合わせて変更してください
#include "../../core/logging/son_tmp_piclog.h"
#include "son_tmp_excute_mission.h"

// ============================================================================
// ミッション実行シーケンス
// ============================================================================

void execute_mission_sequence(uint8_t rx_channel, uint8_t samplingRate, uint8_t mode)
{
    fprintf(PC, "--- Mission Sequence Start ---\r\n");
    piclog_make(0x11, 0x00);

    uint8_t hw_channel = 0;

    // BOSSからのチャンネル(1〜4)をハードウェアのスイッチ用(0〜3)に変換
    if (rx_channel >= 1 && rx_channel <= 4)
    {
        hw_channel = rx_channel - 1;
    }
    else
    {
        fprintf(PC, "[WARN] Invalid Channel %u. Forced to CH 1 (HW:0).\r\n", rx_channel);
        hw_channel = 0;
    }

    fprintf(PC, "Executing Target -> BOSS_CH: %u (HW_CH: %u), SamplingRate: 0x%02X\r\n", rx_channel, hw_channel, samplingRate);

    // ★長時間の測定処理へ飛ぶ
    // ※この関数内のループの合間で UART受信(STATUS_CHECK) に答える処理が必要です。
    execute_measurement(mode, hw_channel, samplingRate);

    piclog_make(0x12, 0x00);

    // ★オリジナルの思想通り、終わったらステータスを FINISHED(完了)に変更する
    // (BOSSから STATUS_CHECK が来た時にこれを返すため)
    status = FINISHED;

    fprintf(PC, "--- Mission Sequence Complete ---\r\n");
}