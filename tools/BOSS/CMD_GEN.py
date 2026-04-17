import sys

def print_header():
    print("==================================================")
    print("  TMP Command Payload Generator v2.3")
    print("==================================================")
    print("BOSS PIC Simulator に貼り付けるための 9Byte の")
    print("UPLINK COMMAND ペイロードを自動生成します。")
    print("--------------------------------------------------")

def get_int_input(prompt, min_val, max_val, is_hex=False):
    """範囲指定付きで整数入力を受け付ける（16進数入力もサポート）"""
    while True:
        try:
            val_str = input(prompt).strip()
            if not val_str:
                print("値を入力してください。")
                continue

            if is_hex or val_str.startswith('0x') or val_str.startswith('0X'):
                val = int(val_str, 16)
            else:
                val = int(val_str, 10)

            if min_val <= val <= max_val:
                return val
            else:
                print(f"[エラー] 値は {min_val} から {max_val} の範囲で入力してください。")
        except ValueError:
            print("[エラー] 正しい数値を入力してください。")

def get_hex_choice(prompt, valid_choices):
    """16進数の文字列（例: 'A0', '8F'）または1桁の文字での入力を受け付ける"""
    while True:
        choice = input(prompt).strip().upper()
        # プレフィックス(0x)がついていれば外す
        if choice.startswith('0X'):
            choice = choice[2:]

        # 入力が有効なリストに含まれているか確認 (80 や 0 などの両方に対応)
        if choice in valid_choices:
            return choice
        # 1文字入力の場合、対応するキーを推測
        matching = [k for k in valid_choices if k.endswith(choice) and len(k) == 2]
        if matching:
            return matching[0]

        print(f"[エラー] 有効なコマンドIDを入力してください: {', '.join(valid_choices)}")

def format_payload(cmd_id, args):
    """1つのCMD_IDと最大8つの引数から9ByteのHEX文字列を生成する"""
    payload = [cmd_id] + args
    # 9バイトに満たない場合は 0x00 で埋める
    while len(payload) < 9:
        payload.append(0x00)

    hex_str = " ".join(f"{b:02X}" for b in payload)
    return hex_str

def input_address_and_packets(needs_sectors=False, is_smf=False):
    """アドレス(4Byte)と、パケット数(2Byte)またはセクタ数(1Byte)の入力を求める共通処理"""

    if is_smf:
        print("\n--- [参考] SMF (CPLD) 最新メモリマップ ---")
        print(" 0x04DA1000 : STR_DATA (128KB)")
        print(" 0x04DC2000 : PICLOG_DATA (64KB)")
        print("------------------------------------------")
    else:
        print("\n--- [参考] PICF (Local) 最新メモリマップ ---")
        print(" 0x00000000 : DATA_TABLE (メイン)")
        print(" 0x00001000 : DATA_TABLE (バックアップ)")
        print(" 0x00010000 : PICLOG_DATA (64KB)")
        print(" 0x00020000 : STR_DATA (残り約15.8MB)")
        print("--------------------------------------------")

    print("※ アドレスは16進数(例: 0x10000) または 10進数 で入力できます。")
    addr = get_int_input("> 開始アドレス (0x00000000 - 0xFFFFFFFF): ", 0, 0xFFFFFFFF, is_hex=True)

    a3 = (addr >> 24) & 0xFF
    a2 = (addr >> 16) & 0xFF
    a1 = (addr >> 8)  & 0xFF
    a0 = addr & 0xFF

    if needs_sectors:
        sector_num = get_int_input("> 処理するセクタ数 (1-255): ", 1, 255)
        return [a3, a2, a1, a0, 0x00, 0x00, sector_num]
    else:
        packet_num = get_int_input("> 処理するパケット数 (1-65535): ", 1, 65535)
        p1 = (packet_num >> 8) & 0xFF
        p0 = packet_num & 0xFF
        return [a3, a2, a1, a0, 0x00, p1, p0]

def generate_8x_command():
    print("\n--- 8x: PICF (Local Flash) 制御コマンド ---")
    print(" 80: PICF_ERASE_ALL       (全消去)")
    print(" 81: PICF_ERASE_1SECTOR   (64KB単位消去)")
    print(" 82: PICF_ERASE_4K        (4KB単位消去)")
    print(" 83: PICF_ERASE_32K       (32KB単位消去)")
    print(" 84: PICF_WRITE_DEMO      (デモ書込)")
    print(" 85: PICF_WRITE_4K        (4KBテスト書込)")
    print(" 86: PICF_READ            (物理アドレス読出)")
    print(" 87: PICF_READ_ADDRESS    (使用量カウンタ確認)")
    print(" 88: PICF_ERASE_RESET     (全消去＆リセット)")
    print(" 89: PICF_READ_AREA       (エリア指定読出 ★推奨)")
    print(" 8F: PICF_RESET_ADDR      (カウンタのみリセット)")

    valid_cmds = ['80', '81', '82', '83', '84', '85', '86', '87', '88', '89', '8F']
    cmd_str = get_hex_choice("\n> 実行するコマンドIDを下2桁(または1桁)で入力 (例: 89, 9): ", valid_cmds)
    cmd_id = int(cmd_str, 16)

    args = []

    if cmd_id in [0x80, 0x87, 0x88, 0x8F]:
        pass
    elif cmd_id in [0x81, 0x82, 0x83]:
        args = input_address_and_packets(needs_sectors=True, is_smf=False)
    elif cmd_id in [0x84, 0x86]:
        args = input_address_and_packets(needs_sectors=False, is_smf=False)
    elif cmd_id == 0x85:
        print("\n--- [参考] PICF (Local) 最新メモリマップ ---")
        print(" 0x00000000 : DATA_TABLE (メイン)")
        print(" 0x00001000 : DATA_TABLE (バックアップ)")
        print(" 0x00010000 : PICLOG_DATA (64KB)")
        print(" 0x00020000 : STR_DATA (残り約15.8MB)")
        print("--------------------------------------------")
        print("※ アドレスは16進数(例: 0x10000) または 10進数 で入力できます。")
        addr = get_int_input("> 開始アドレス (0x00000000 - 0xFFFFFFFF): ", 0, 0xFFFFFFFF, is_hex=True)
        args = [(addr >> 24) & 0xFF, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF]
    elif cmd_id == 0x89:
        print("\n0: DATA_TABLE (管理領域) | 1: PICLOG (ログ領域) | 2: STR_DATA (計測領域)")
        area = get_int_input("> エリアIDを選択 (0-2): ", 0, 2)

        if area == 2:
            print("\n--- 読み出し方法の選択 ---")
            print("1: 手動で開始パケットとパケット数を指定する")
            print("2: 「N回目」の測定データを丸ごと指定する (1回=32パケットとして自動計算)")
            read_mode = get_int_input("> 選択 (1-2): ", 1, 2)
        else:
            read_mode = 1

        if read_mode == 1:
            start_pkt = get_int_input("> 読み出し開始パケット (0-65535) [例: 0=最新から]: ", 0, 65535)
            req_pkts = get_int_input("> 読み出すパケット数 (1-65535) [例: 32]: ", 1, 65535)
        else:
            n_th = get_int_input("> 何回目の測定を読み出しますか？ (1-2048): ", 1, 2048)
            start_pkt = (n_th - 1) * 32
            req_pkts = 32
            print(f" [*] 自動計算: 開始パケット = {start_pkt}, 読み出しパケット数 = {req_pkts}")

        args = [
            area,
            (start_pkt >> 8) & 0xFF,
            start_pkt & 0xFF,
            (req_pkts >> 8) & 0xFF,
            req_pkts & 0xFF
        ]

    return format_payload(cmd_id, args)

def generate_9x_command():
    print("\n--- 9x: SMF (Shared Mission Flash) 制御コマンド ---")
    print(" 90: SMF_COPY             (PICF -> SMF コピー)")
    print(" 91: SMF_READ             (読出)")
    print(" 92: SMF_ERASE            (64KB単位消去)")
    print(" 93: SMF_COPY_FORCE       (強制コピー)")
    print(" 94: SMF_READ_FORCE       (強制読出)")
    print(" 95: SMF_ERASE_FORCE      (強制特定エリア消去)")

    valid_cmds = ['90', '91', '92', '93', '94', '95']
    cmd_str = get_hex_choice("\n> 実行するコマンドIDを下2桁(または1桁)で入力 (例: 90, 0): ", valid_cmds)
    cmd_id = int(cmd_str, 16)

    args = []
    if cmd_id in [0x90, 0x91, 0x93, 0x94]:
        args = input_address_and_packets(needs_sectors=False, is_smf=True)
    elif cmd_id == 0x92:
        args = input_address_and_packets(needs_sectors=True, is_smf=True)
    elif cmd_id == 0x95:
        pass

    return format_payload(cmd_id, args)

def generate_Ax_command():
    print("\n--- Ax: 計測系・ミッション制御コマンド ---")
    print(" A0: CMD_STR              (保存のみ [本番用])")
    print(" A1: CMD_STR_DEBUG        (ダンプのみ)")
    print(" A2: CMD_STR_PRINT        (生データリアルタイム出力)")
    print(" A3: CMD_STR_DEBUG_SAVE   (保存 ＋ ダンプ)")
    print(" AF: CMD_MISSION_ABORT    (強制停止)")

    valid_cmds = ['A0', 'A1', 'A2', 'A3', 'AF']
    cmd_str = get_hex_choice("\n> 実行するコマンドIDを下2桁(または1桁)で入力 (例: A0, 0, AF, F): ", valid_cmds)
    cmd_id = int(cmd_str, 16)

    if cmd_id == 0xAF:
        return format_payload(cmd_id, [])

    # ★修正: 10ms, 50ms の選択肢を削除し、ID 3〜9 に限定しました
    print("\n--- サンプリングレート ---")
    print(" 3: 100ms   4: 500ms   5: 1sec")
    print(" 6: 5sec    7: 2432ms  8: 4865ms")
    print(" 9: 9730ms")

    rate = get_int_input("> サンプリングレートを選択 (3-9): ", 3, 9)

    args = [0x00, rate]
    return format_payload(cmd_id, args)

def generate_Bx_command():
    print("\n--- Bx: システム制御コマンド ---")
    print(" B0: RETURN_TIME          (時刻確認)")

    valid_cmds = ['B0']
    cmd_str = get_hex_choice("\n> 実行するコマンドIDを下2桁(または1桁)で入力 (例: B0, 0): ", valid_cmds)
    cmd_id = int(cmd_str, 16)

    return format_payload(cmd_id, [])

def main():
    print_header()

    while True:
        print("\n==================================================")
        print(" 作成したいコマンドのカテゴリ(プレフィックス)を選んでください")
        print("--------------------------------------------------")
        print(" 8: PICFコマンド群")
        print(" 9: SMFコマンド群")
        print(" A: 計測・ミッション制御コマンド群")
        print(" B: システム制御コマンド群")
        print(" 0: 終了")
        print("==================================================")

        category_str = get_hex_choice("\n> カテゴリを選択 (8, 9, A, B, 0): ", ['8', '9', 'A', 'B', '0'])

        if category_str == '0':
            print("終了します。")
            break

        payload_str = ""

        if category_str == '8':
            payload_str = generate_8x_command()
        elif category_str == '9':
            payload_str = generate_9x_command()
        elif category_str == 'A':
            payload_str = generate_Ax_command()
        elif category_str == 'B':
            payload_str = generate_Bx_command()

        print("\n" + "="*50)
        print(" 生成されたコマンドペイロード (9 Byte) ")
        print("=" * 50)
        print(f"\n      {payload_str}\n")
        print("-" * 50)
        print(" ※ 上記の文字列を BOSS PIC Simulator の入力プロンプトに")
        print("    そのままコピー＆ペーストして Enter を押してください。")
        print("==================================================")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n終了します。")
        sys.exit(0)