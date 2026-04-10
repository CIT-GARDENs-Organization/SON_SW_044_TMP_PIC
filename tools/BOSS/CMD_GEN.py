import sys

def print_header():
    print("==================================================")
    print("TMP Command Payload Generator v1.0")
    print("==================================================")
    print("")
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

def format_payload(cmd_id, args):
    """1つのCMD_IDと最大8つの引数から9ByteのHEX文字列を生成する"""
    payload = [cmd_id] + args
    # 9バイトに満たない場合は 0x00 で埋める
    while len(payload) < 9:
        payload.append(0x00)

    hex_str = " ".join(f"{b:02X}" for b in payload)
    return hex_str

def generate_measurement_command():
    print("\n--- 計測コマンド (0xA0 - 0xA3) ---")
    print("1: A0 (STR - 保存のみ)")
    print("2: A1 (STR_DEBUG - ダンプのみ)")
    print("3: A2 (STR_PRINT - 生データリアルタイム出力)")
    print("4: A3 (STR_DEBUG_SAVE - 保存  +  ダンプ)")

    cmd_choice = get_int_input("> コマンドの種類を選択 (1-4): ", 1, 4)
    cmd_id = {1: 0xA0, 2: 0xA1, 3: 0xA2, 4: 0xA3}[cmd_choice]

    print("\n--- サンプリングレート ---")
    print("1: 10ms    2: 50ms    3: 100ms")
    print("4: 500ms   5: 1sec    6: 5sec")
    print("7: 2432ms  8: 4865ms  9: 9730ms")

    rate = get_int_input("> サンプリングレートを選択 (1-9): ", 1, 9)

    # Arg1(CH指定)は無視されるため 0x00、Arg2にレートを入れる
    args = [0x00, rate]

    return format_payload(cmd_id, args)

def generate_read_area_command():
    print("\n--- エリア指定読み出し (0x89) ---")
    print("0: DATA_TABLE (管理領域)")
    print("1: PICLOG (ログ領域)")
    print("2: STR_DATA (計測データ領域)")

    area = get_int_input("> エリアIDを選択 (0-2): ", 0, 2)
    start_pkt = get_int_input("> 読み出し開始パケット (0-255) [例: 0=最新から]: ", 0, 255)
    req_pkts = get_int_input("> 読み出すパケット数 (1-255) [例: 50]: ", 1, 255)

    args = [area, start_pkt, req_pkts]

    return format_payload(0x89, args)

def generate_direct_address_command():
    print("\n--- 直接アドレス指定コマンド (0x86, 0x81, etc...) ---")
    print("1: 0x86 (PICF_READ - 読み出し)")
    print("2: 0x81 (PICF_ERASE_1SECTOR - 64KB消去)")
    print("3: 0x84 (PICF_WRITE_DEMO - デモ書き込み)")
    print("4: 0x90 (SMF_COPY - CPLD転送)")

    cmd_choice = get_int_input("> コマンドを選択 (1-4): ", 1, 4)
    cmd_id = {1: 0x86, 2: 0x81, 3: 0x84, 4: 0x90}[cmd_choice]

    print("\n※ アドレスは16進数(例: 0x1000) または 10進数 で入力できます。")
    addr = get_int_input("> 開始アドレス (0x00000000 - 0x00FFFFFF): ", 0, 0x00FFFFFF, is_hex=True)

    # アドレスを 4Byte にビッグエンディアン分割
    a3 = (addr >> 24) & 0xFF
    a2 = (addr >> 16) & 0xFF
    a1 = (addr >> 8)  & 0xFF
    a0 = addr & 0xFF

    if cmd_id == 0x81:
        # 消去系は Arg7 にセクタ数
        sector_num = get_int_input("> 消去するセクタ数 (1-255): ", 1, 255)
        args = [a3, a2, a1, a0, 0x00, 0x00, sector_num]
    else:
        # 読み書き系は Arg6,7 にパケット数(16bit)
        packet_num = get_int_input("> 処理するパケット数 (1-65535): ", 1, 65535)
        p1 = (packet_num >> 8) & 0xFF
        p0 = packet_num & 0xFF
        args = [a3, a2, a1, a0, 0x00, p1, p0]

    return format_payload(cmd_id, args)

def generate_system_command():
    print("\n--- システム・単純コマンド ---")
    print("1: 0xAF (MISSION_ABORT - 強制停止)")
    print("2: 0xB0 (RETURN_TIME - 時刻確認)")
    print("3: 0x87 (PICF_READ_ADDRESS - カウンタ確認)")
    print("4: 0x8F (PICF_RESET_ADDR - カウンタリセット)")

    cmd_choice = get_int_input("> コマンドを選択 (1-4): ", 1, 4)
    cmd_id = {1: 0xAF, 2: 0xB0, 3: 0x87, 4: 0x8F}[cmd_choice]

    return format_payload(cmd_id, [])

def main():
    print_header()

    while True:
        print("\n==================================================")
        print(" 作成したいコマンドのカテゴリを選んでください")
        print("--------------------------------------------------")
        print(" 1: 計測コマンド (A0 - A3)")
        print(" 2: エリア指定読み出し (89)")
        print(" 3: 物理アドレス指定の処理 (86, 81, 84, 90 など)")
        print(" 4: システム・引数なしコマンド (AF, B0, 87, 8F)")
        print(" 0: 終了")
        print("==================================================")

        category = get_int_input("\n> カテゴリを選択 (0-4): ", 0, 4)

        if category == 0:
            print("終了します。")
            break

        payload_str = ""

        if category == 1:
            payload_str = generate_measurement_command()
        elif category == 2:
            payload_str = generate_read_area_command()
        elif category == 3:
            payload_str = generate_direct_address_command()
        elif category == 4:
            payload_str = generate_system_command()

        print("\n" + "="*50)
        print(" 生成されたコマンドペイロード (9 Byte) ")
        print("="*50)
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