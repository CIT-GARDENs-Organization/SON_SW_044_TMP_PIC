import sys
import os
import csv

# サンプリングレートIDと文字列のマッピング (son_tmp_config.hに基づく)
SAMP_RATE_MAP = {
    0x01: "10ms",
    0x02: "50ms",
    0x03: "100ms",
    0x04: "500ms",
    0x05: "1sec",
    0x06: "5sec"
}

def parse_dump_file(filepath):
    """テキストファイルから | の右側の16進数を抽出してリストにする"""
    hex_list = []
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            if '|' in line:
                # '|' の右側にある16進数部分を抽出
                data_part = line.split('|')[1].strip()
                # 空白で区切られた文字列を数値に変換
                for item in data_part.split():
                    try:
                        hex_list.append(int(item, 16))
                    except ValueError:
                        pass # 16進数に変換できないゴミデータは無視
    return hex_list

def decode_packets(hex_list):
    """64Byteのパケット群を解析してレコードリストを返す"""
    records = []
    header_info = {}
    
    # 64バイトごとにパケットとして分割
    packets = [hex_list[i:i+64] for i in range(0, len(hex_list), 64)]
    
    data_count = 0
    current_step = 0
    
    for i, packet in enumerate(packets):
        # 64バイト揃っていない不完全なパケットはスキップ
        if len(packet) < 64:
            print(f"[WARN] パケット {i+1} が不完全なためスキップしました ({len(packet)} bytes).")
            continue
            
        if i == 0:
            # ===== 第1パケット (ヘッダー付き) の解析 =====
            time_sec = (packet[0] << 24) | (packet[1] << 16) | (packet[2] << 8) | packet[3]
            
            mode_ch_rate = packet[4]
            mode = (mode_ch_rate >> 5) & 0x07
            hw_ch = (mode_ch_rate >> 4) & 0x01
            samp_rate = mode_ch_rate & 0x0F
            
            data_count = (packet[5] << 4) | ((packet[6] >> 4) & 0x0F)
            
            header_info = {
                'time_sec': time_sec,
                'mode': mode,
                'hw_ch': hw_ch,
                'samp_rate': samp_rate,
                'data_count': data_count
            }
            
            start_idx = 7
            num_blocks = 9
        else:
            # ===== 第2パケット以降の解析 =====
            packet_num = packet[0]
            start_idx = 1
            num_blocks = 10
            
        # ===== 6Byteのデータ塊(ブロック)のパース =====
        for b in range(num_blocks):
            if current_step >= data_count:
                break # 予定された総データ数に達したら終了
                
            idx = start_idx + b * 6
            
            # 温度(12bit): DataNumの4bitは無視して下位12bitのみ抽出
            temp_val = ((packet[idx] & 0x0F) << 8) | packet[idx+1]
            
            # ひずみ1(16bit): 2Byteを結合
            str1_val = (packet[idx+2] << 8) | packet[idx+3]
            
            # ひずみ2(16bit): 2Byteを結合
            str2_val = (packet[idx+4] << 8) | packet[idx+5]
            
            records.append({
                'Step': current_step,
                'Temperature_RAW': temp_val,
                'Strain1_RAW': str1_val,
                'Strain2_RAW': str2_val
            })
            
            current_step += 1
            
    return header_info, records

def main():
    # 引数が指定されていない場合は使い方を表示
    if len(sys.argv) < 2:
        print("使い方: シリアルからコピーしたテキストファイルをこのスクリプトにドラッグ＆ドロップしてください。")
        print("コマンドライン例: python hex_to_csv.py dump_data.txt")
        input("Enterキーを押して終了...")
        return

    input_file = sys.argv[1]
    
    if not os.path.exists(input_file):
        print(f"[ERROR] ファイルが見つかりません: {input_file}")
        return

    print(f"ファイルを読み込んでいます: {os.path.basename(input_file)}")
    
    # 1. データの抽出
    hex_list = parse_dump_file(input_file)
    if not hex_list:
        print("[ERROR] 有効なデータが見つかりませんでした。'|' 区切りのフォーマットか確認してください。")
        return
        
    print(f"{len(hex_list)} バイトのHexデータを抽出しました。")

    # 2. パケットの解析とデータ復元
    header_info, records = decode_packets(hex_list)
    
    if not records:
        print("[ERROR] レコードをデコードできませんでした。")
        return

    # 3. ファイル名の生成 (時間とサンプリングレートを使用)
    time_sec = header_info.get('time_sec', 0)
    rate_id = header_info.get('samp_rate', 0)
    rate_str = SAMP_RATE_MAP.get(rate_id, f"ID{rate_id}")
    
    # 例: Data_36sec_10ms.csv
    output_filename = f"Data_{time_sec}sec_{rate_str}.csv"
    
    # 入力ファイルと同じフォルダに保存
    output_dir = os.path.dirname(os.path.abspath(input_file))
    output_path = os.path.join(output_dir, output_filename)

    # 4. CSVファイルへの書き込み
    with open(output_path, 'w', newline='', encoding='utf-8') as csvfile:
        fieldnames = ['Step', 'Temperature_RAW', 'Strain1_RAW', 'Strain2_RAW']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        
        writer.writeheader()
        writer.writerows(records)

    print(f"\n[SUCCESS] 変換が完了しました！")
    print(f"-> 保存先: {output_path}")
    print(f"-> 抽出したデータ数: {len(records)}件 / (ヘッダ予定数: {header_info.get('data_count')})")

if __name__ == "__main__":
    main()