import os
import glob
import pandas as pd
from datetime import datetime

# ログインユーザー名を取得
username = os.environ.get("USERNAME")
# ベースパスの構築
base_path = fr"C:\Users\{username}\Chiba Institute of Technology\5号機 - ドキュメント\General\00_MMJ文書\ICD\MMJ_ICD_018_IICD"
# ファイル名キーワード
filename_keyword = "Uplink_command"
# ファイル検索パターン
search_pattern = os.path.join(base_path, f"*{filename_keyword}*.xlsx")
# シートの項目名と列番号のマッピング
SHEET_ITEM_MAPPING = {
    "Uplink_command": {
        "Category": 0,
        "Name": 1,
        "CmdDesc": 2,
        "CmdID": 3,
        "Param0": 4,
        "Param1": 5,
        "Param2": 6,
        "Param3": 7,
        "Param4": 8,
        "Param5": 9,
        "Param6": 10,
        "Prama7": 11
    }
}


