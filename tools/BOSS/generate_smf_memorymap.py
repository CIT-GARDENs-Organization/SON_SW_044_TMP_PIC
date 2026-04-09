"""
Memory Map Header Generator
Excelファイルからメモリマップヘッダーファイルを自動生成するメインスクリプト
"""

import os
import glob
from smf_memorymap_generator import generate_memory_map_header


def main():
    """メイン実行関数"""
    # ログインユーザー名を取得
    username = os.environ.get("USERNAME")
    
    # ベースパスの構築
    base_path = fr"C:\Users\{username}\Chiba Institute of Technology\5号機 - ドキュメント\General\00_MMJ文書\ICD\MMJ_ICD_018_IICD"
    
    # ファイル名キーワード
    filename_keyword = "Memory_map"
    
    # ファイル検索パターン
    search_pattern = os.path.join(base_path, f"*{filename_keyword}*.xlsx")
    
    print(f"Search Pattern: {search_pattern}")
    matches = glob.glob(search_pattern)
    
    if matches:
        excel_file = matches[0]
        print(f"✅ ファイル見つかった: {excel_file}")
        
        # ヘッダーファイル生成を試行
        header_content = generate_memory_map_header(excel_file)
        
        if header_content:
            # ファイルに保存（絶対パスを使用）
            script_dir = os.path.dirname(os.path.abspath(__file__))
            project_root = os.path.dirname(script_dir)
            output_file = os.path.join(project_root, "lib", "tool", "mmj_smf_memorymap.h")

            # ディレクトリが存在しない場合は作成
            output_dir = os.path.dirname(output_file)
            os.makedirs(output_dir, exist_ok=True)
            
            try:
                with open(output_file, 'w', encoding='utf-8') as f:
                    f.write(header_content)
                print(f"\n✅ ヘッダーファイルが生成されました: {output_file}")
            except Exception as e:
                print(f"❌ ファイル保存エラー: {e}")
        else:
            print("❌ ヘッダーファイルの生成に失敗しました")
            print("   処理を中断します")
            exit(1)
        
    else:
        print("❌ Excelファイルが見つかりませんでした")
        print(f"   検索パス: {search_pattern}")
        print("   以下を確認してください:")
        print("   - ファイルが正しい場所にあるか")
        print("   - ファイル名にMemory_mapが含まれているか")
        print("   - ネットワークドライブが接続されているか")
        exit(1)


if __name__ == "__main__":
    main()
