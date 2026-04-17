#include "son_tmp_mode_mission.h"
#include "../../core/measurement/son_tmp_strain.h" // ※環境に合わせて変更してください
#include "../../core/logging/son_tmp_piclog.h"
#include "son_tmp_excute_mission.h"

// ============================================================================
// ログ表示用のヘルパー関数
// ============================================================================
void print_sampling_rate(uint8_t rate) {
    switch(rate) {
        // ★修正: 10msと50msの表示を削除しました
        // case 0x01: fprintf(PC, "10ms"); break;
        // case 0x02: fprintf(PC, "50ms"); break;
        case 0x03: fprintf(PC, "100ms"); break;
        case 0x04: fprintf(PC, "500ms"); break;
        case 0x05: fprintf(PC, "1000ms"); break;
        case 0x06: fprintf(PC, "5000ms"); break;
        case 0x07: fprintf(PC, "2432ms"); break;
        case 0x08: fprintf(PC, "4865ms"); break;
        case 0x09: fprintf(PC, "9730ms"); break;
        default:   fprintf(PC, "Unknown(0x%02X)", rate); break;
    }
}

void print_mode(uint8_t mode) {
    switch(mode) {
        case 0x01: fprintf(PC, "SAVE(A0)"); break;
        case 0x02: fprintf(PC, "DEBUG_DUMP(A1)"); break;
        case 0x03: fprintf(PC, "PRINT_ONLY(A2)"); break;
        case 0x04: fprintf(PC, "DEBUG_SAVE(A3)"); break;
        default:   fprintf(PC, "Unknown(0x%02X)", mode); break;
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

    // ポインタエラーを防ぐため、直接print関数を呼び出して出力をつなげます
    fprintf(PC, "Executing Target -> Fixed CH1 & CH2, SamplingRate: ");
    print_sampling_rate(samplingRate);
    fprintf(PC, ", Mode: ");
    print_mode(mode);
    fprintf(PC, "\r\n");


    // 計測処理を呼び出し (この中で mode に応じた分岐が行われます)
    execute_measurement(mode, samplingRate);

    // ミッション終了ログの記録
    piclog_make(0x12, 0x00);

    fprintf(PC, "--- Mission Sequence Complete ---\r\n");
}