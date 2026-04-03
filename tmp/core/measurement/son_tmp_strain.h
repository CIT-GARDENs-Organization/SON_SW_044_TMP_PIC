#ifndef son_tmp_strain_H
#define son_tmp_strain_H

#include <stdint.h>
#include <stdbool.h>
#include "../../system/son_tmp_config.h"

// ============================================================================
// 外部公開関数プロトタイプ
// ============================================================================

// ペリフェラルの初期化 (ピンの出力設定など)
void io_init(void);

// アナログスイッチの切り替え (0〜3チャンネル)
void switch_channel(uint8_t ch);

// LTC2452から16bitのADC値を読み取る
uint16_t read_adc_ltc2452(void);

// 計測シーケンスの実行とFlashへの保存 (IVデータ ＋ 温度データを連続実行)
// mode: 動作モード
// channel: 測定チャンネル (MAX4734EUB+ の選択)
// samplingRate: サンプリングレート設定値
void execute_measurement(uint8_t mode, uint8_t channel, uint8_t samplingRate);

#endif // son_tmp_strain_H
