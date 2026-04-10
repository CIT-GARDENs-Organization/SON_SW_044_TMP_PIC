#include "son_tmp_mode_mission.h"
#include "../../core/measurement/son_tmp_strain.h" // ※環境に合わせて変更してください
#include "../../core/logging/son_tmp_piclog.h"
#include "son_tmp_excute_mission.h"

// ============================================================================
// ログ表示用のヘルパー関数
// ============================================================================
const char* get_sampling_rate_str(uint8_t rate) {
    switch(rate) {
        case 0x01: return "10ms";
        case 0x02: return "50ms";
        case 0x03: return "100ms";
        case 0x04: return "500ms";
        case 0x05: return "1000ms";
        case 0x06: return "5000ms";
        case 0x07: return "2432ms";
        case 0x08: return "4865ms";
        case 0x09: return "9730ms";
        default:   return "Unknown";
    }
}

const char* get_mode_str(uint8_t mode) {
    switch(mode) {
        case 0x01: return "SAVE (A0)";
        case 0x02: return "DEBUG_DUMP (A1)";
        case 0x03: return "PRINT_ONLY (A2)";
        case 0x04: return "DEBUG_SAVE (A3)";
        default:   return "Unknown";
    }
}

// ============================================================================
// ミッション実行シーケンス
// ============================================================================

void execute_mission_sequence(uint8_t samplingRate, uint8_t mode)
{
    fprintf(PC, "--- Mission Sequence Start ---\r\n");

    // ミッション開始時に中断フラグをクリア
    is_mission_aborted = false;

    // 巨大な数字に化けていないか確認するための時間出力
    uint32_t current_time = get_current_sec();
    fprintf(PC, "Current Time Check: %lu sec\r\n", current_time);

    piclog_make(0x11, 0x00);

    // パケットヘッダーの互換性維持用に hw_channel は 0 固定とします。
    uint8_t hw_channel = 0;

        fprintf(PC, "Executing Target -> Fixed CH1 & CH2, SamplingRate: %s, Mode: %s\r\n",
            get_sampling_rate_str(samplingRate),
            get_mode_str(mode));


    // 計測処理を呼び出し (この中で mode に応じた分岐が行われます)
    execute_measurement(mode, samplingRate);

    // ミッション終了ログの記録
    piclog_make(0x12, 0x00);

    fprintf(PC, "--- Mission Sequence Complete ---\r\n");
}