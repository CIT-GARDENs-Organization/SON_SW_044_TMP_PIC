#ifndef son_tmp_MODE_MISSION_H
#define son_tmp_MODE_MISSION_H

#include <stdint.h>
#include "../../system/son_tmp_config.h"


// ============================================================================
// 外部公開関数プロトタイプ
// ============================================================================

// ミッションの全体シーケンスを実行する
// ここから core/measurement/son_tmp_strain.c の計測関数を呼び出す
void execute_mission_sequence(uint8_t rx_channel, uint8_t samplingRate);

#endif // son_tmp_MODE_MISSION_H
