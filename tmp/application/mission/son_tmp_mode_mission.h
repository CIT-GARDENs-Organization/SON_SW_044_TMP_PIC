#ifndef son_tmp_MODE_MISSION_H
#define son_tmp_MODE_MISSION_H

#include <stdint.h>
#include "../../system/son_tmp_config.h"


// ============================================================================
// 外部公開関数プロトタイプ
// ============================================================================

// ミッションの全体シーケンスを実行する
void execute_mission_sequence(uint8_t samplingRate, uint8_t mode);

#endif // son_tmp_MODE_MISSION_H
