#ifndef son_tmp_MAIN_H
#define son_tmp_MAIN_H

#opt 0 // 0 = no optimization

//==============================================================================
// CCS C 最適化構造: ヘッダー分散 + .cファイル統合
//==============================================================================

// レベル1: システム設定
#include "son_tmp_config.h"

// レベル2: ハードウェア抽象化層
#include "../hardware/mcu/timer.h"
#include "../hardware/mcu/uart.h"

// レベル3: 基本ライブラリヘッダー（型定義・通信・ツール）
#include "../../lib/communication/typedef_content.h"
#include "../../lib/communication/value_status.h"
#include "../../lib/tool/calc_tools.h"
#include "../../lib/tool/smf_queue.h"
#include "../../lib/tool/mmj_smf_memorymap.h"

// レベル4: デバイスドライバヘッダー
#include "../../lib/device/mt25q.h"

// レベル5: 通信ライブラリヘッダー
#include "../../lib/communication/communication.h"
#include "../../lib/communication/communication_driver.h"
#include "../../lib/communication/mission_tools.h"

// レベル6: コア機能ヘッダー
#include "../core/measurement/son_tmp_strain.h"
#include "../core/measurement/son_tmp_temp.h"
#include "../core/storage/son_tmp_flash.h"
#include "../core/logging/son_tmp_piclog.h"

// レベル7: アプリケーションヘッダー
#include "../application/mission/son_tmp_excute_mission.h"
#include "../application/mission/son_tmp_mode_mission.h"
#include "../application/mission/son_tmp_mode_flash.h"


//==============================================================================
// .cファイル統合（CCS C単一コンパイル単位）
//==============================================================================

// ハードウェア層実装ファイル
#include "../hardware/mcu/src/timer.c"
#include "../hardware/mcu/src/uart.c"

// ライブラリ実装ファイル
#include "../../lib/device/mt25q.c"
#include "../../lib/tool/calc_tools.c"
#include "../../lib/tool/smf_queue.c"
#include "../../lib/communication/communication.c"
#include "../../lib/communication/communication_driver.c"
#include "../../lib/communication/mission_tools.c"

// コア機能実装ファイル
#include "../core/measurement/son_tmp_strain.c"
#include "../core/measurement/son_tmp_temp.c"
#include "../core/logging/son_tmp_piclog.c"
#include "../core/storage/son_tmp_smf.c"
#include "../core/storage/son_tmp_flash.c"

// アプリケーション実装ファイル
#include "../application/mission/son_tmp_excute_mission.c"
#include "../application/mission/son_tmp_mode_mission.c"
#include "../application/mission/son_tmp_mode_flash.c"

#endif // son_tmp_MAIN_H
//------------------End of File------------------
