import sys
import os
import csv

# サンプリングレートIDと文字列のマッピング (最新仕様に対応)
SAMP_RATE_MAP = {
    0x01: "10ms",
    0x02: "50ms",
    0x03: "100ms",
    0x04: "500ms",
    0x05: "1sec",
    0x06: "5sec",
    0x07: "2432ms",
    0x08: "4865ms",
    0x09: "9730ms"
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

def is_header_packet(packet):
    """パケットが計測のヘッダー(先頭)であるかを特徴から判定する"""
    if len(packet) < 64: return False

    mode_ch_rate = packet[4]
    mode = (mode_ch_rate >> 5) & 0x07
    hw_ch = (mode_ch_rate >> 4) & 0x01
    samp_rate = mode_ch_rate & 0x0F

    # モードが1〜4、CHが0固定、サンプリングレートが1〜9の範囲に収まっていればヘッダーとみなす
    # (FF FF FF... などのゴミデータはここで弾かれる)
    if not (1 <= mode <= 4): return False
    if hw_ch != 0: return False
    if not (1 <= samp_rate <= 9): return False

    return True

def parse_blocks(packet, start_idx, num_blocks, session):
    """パケットの中から6Byteのデータブロックを抽出してセッションに追加する"""
    for b in range(num_blocks):
        if session['current_step'] >= session['header']['data_count']:
            break # 予定された総データ数に達したら終了

        idx = start_idx + b * 6

        # パケットの末尾を超えて読まないようにガード
        if idx + 5 >= len(packet):
            break

        # 温度(12bit): DataNumの4bitは無視して下位12bitのみ抽出
        temp_val = ((packet[idx] & 0x0F) << 8) | packet[idx+1]

        # ひずみ1(16bit): 2Byteを結合
        str1_val = (packet[idx+2] << 8) | packet[idx+3]

        # ひずみ2(16bit): 2Byteを結合
        str2_val = (packet[idx+4] << 8) | packet[idx+5]

        session['records'].append({
            'Step': session['current_step'],
            'Temperature_RAW': temp_val,
            'Strain1_RAW': str1_val,
            'Strain2_RAW': str2_val
        })

        session['current_step'] += 1

def extract_sessions(hex_list):
    """Hexデータの塊から、複数の計測セッション(ヘッダーとデータのペア)を抽出する"""
    packets = [hex_list[i:i+64] for i in range(0, len(hex_list), 64)]
    sessions = []

    in_session = False
    current_session = None

    for packet in packets:
        if len(packet) < 64:
            continue

        # ヘッダーの特徴を持つパケットを見つけたら、新しい計測の開始とみなす
        if is_header_packet(packet):
            # もし前のセッションが保存されずに残っていれば保存する (中断された場合など)
            if in_session and current_session['current_step'] > 0:
                sessions.append(current_session)

            time_sec = (packet[0] << 24) | (packet[1] << 16) | (packet[2] << 8) | packet[3]
            mode_ch_rate = packet[4]
            mode = (mode_ch_rate >> 5) & 0x07
            samp_rate = mode_ch_rate & 0x0F
            data_count = (packet[5] << 4) | ((packet[6] >> 4) & 0x0F)

            current_session = {
                'header': {
                    'time_sec': time_sec,
                    'mode': mode,
                    'samp_rate': samp_rate,
                    'data_count': data_count
                },
                'records': [],
                'current_step': 0
            }

            # ヘッダーパケット内のデータブロック (9個) を処理
            parse_blocks(packet, 7, 9, current_session)

            # もしデータが少ない場合、この1パケットで終了の可能性がある
            if current_session['current_step'] >= data_count:
                sessions.append(current_session)
                current_session = None
                in_session = False
            else:
                in_session = True
        else:
            # データパケットの処理
            if in_session:
                parse_blocks(packet, 1, 10, current_session)

                # 予定されたデータ数を取り終えたらセッション終了
                if current_session['current_step'] >= current_session['header']['data_count']:
                    sessions.append(current_session)
                    current_session = None
                    in_session = False

    # ファイルの末尾でセッションが終了していない場合（途切れた場合）も保存
    if in_session and current_session['current_step'] > 0:
        sessions.append(current_session)

    return sessions

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
        input("Enterキーを押して終了...")
        return

    print(f"ファイルを読み込んでいます: {os.path.basename(input_file)}")

    # 1. データの抽出
    hex_list = parse_dump_file(input_file)
    if not hex_list:
        print("[ERROR] 有効なデータが見つかりませんでした。'|' 区切りのフォーマットか確認してください。")
        input("Enterキーを押して終了...")
        return

    print(f"{len(hex_list)} バイトのHexデータを抽出しました。")

    # 2. パケットの解析とセッション分割 (自動スキャン)
    sessions = extract_sessions(hex_list)

    if not sessions:
        print("[ERROR] 解析可能な計測データ(ヘッダー)が見つかりませんでした。")
        input("Enterキーを押して終了...")
        return

    print(f"\n[SUCCESS] {len(sessions)} 回分の計測データを発見しました！")

    output_dir = os.path.dirname(os.path.abspath(input_file))

    # 3. CSVファイルへの書き込み
    for i, session in enumerate(sessions):
        time_sec = session['header']['time_sec']
        rate_id = session['header']['samp_rate']
        rate_str = SAMP_RATE_MAP.get(rate_id, f"ID{rate_id}")

        # 例: Data_36sec_10ms.csv
        output_filename = f"Data_{time_sec}sec_{rate_str}.csv"

        # もし同じ時刻・同じレートのデータが複数見つかった場合は連番を付ける
        if i > 0 and any(s['header']['time_sec'] == time_sec for s in sessions[:i]):
            output_filename = f"Data_{time_sec}sec_{rate_str}_{i}.csv"

        output_path = os.path.join(output_dir, output_filename)

        with open(output_path, 'w', newline='', encoding='utf-8') as csvfile:
            fieldnames = ['Step', 'Temperature_RAW', 'Strain1_RAW', 'Strain2_RAW']
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

            writer.writeheader()
            writer.writerows(session['records'])

        print(f"-> 保存: {output_filename} (抽出したデータ数: {len(session['records'])}件 / ヘッダ予定数: {session['header']['data_count']})")

    print("\nすべての変換が完了しました。")

if __name__ == "__main__":
    main()