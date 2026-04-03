#ifndef son_tmp_SMF_H
#define son_tmp_SMF_H

#include <stdint.h>
#include "../../system/son_tmp_config.h"

// ============================================================================
// 外部公開関数プロトタイプ
// ============================================================================

// SMFへの転送準備（アドレスとサイズの計算）
void cigs_smf_prepare(void);

// MIS_FMからデータを読み出し、SMF(CPLD)へSPI送信
void cigs_smf_copy(void);

// 転送完了を確認し、フラッシュのカウンタを更新
void cigs_smf_permit(void);

#endif // son_tmp_SMF_H