import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import pandas as pd
import os
import subprocess
import sys
from pathlib import Path
import serial.tools.list_ports
from datetime import datetime
import json

class CommandUI:
    def __init__(self, root):
        self.root = root
        self.root.title("MMJ CIGS Command Transmitter v2.0")
        self.root.geometry("900x700")
        self.root.minsize(800, 600)
        
        # ウィンドウスタイルの設定
        self.root.configure(bg='#f0f0f0')
        
        # データ保存用
        self.commands_df = None
        self.selected_command = None
        self.command_list = []  # 手動で追加されたコマンドのリスト
        self.boss_simulator_process = None  # BOSS PIC Simulatorプロセス
        
        # 設定ファイルのパス
        self.config_file = Path(__file__).parent / "command_ui_config.json"
        
        # 設定を読み込み
        self.load_settings()
        
        # UI要素の作成
        self.create_widgets()
        
        # 初期データ読み込み
        self.load_commands()
        
        # COMポートリストを初期化
        self.refresh_com_ports()
        
        # BOSS PIC Simulatorを起動
        self.start_boss_pic_simulator()
        
        # 終了時の設定保存
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
    
    def create_widgets(self):
        # メインフレーム
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # 上部フレーム（ファイル選択＋シリアルポート設定）
        top_frame = ttk.Frame(main_frame)
        top_frame.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 15))
        
        # ファイル選択フレーム（左側）
        file_frame = ttk.LabelFrame(top_frame, text="📁 コマンドファイル選択", padding="10")
        file_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 10))
        
        # ファイルパス選択
        self.file_path_var = tk.StringVar()
        ttk.Label(file_frame, text="ファイルパス:", font=('', 9, 'bold')).grid(row=0, column=0, sticky=tk.W, pady=(0, 5))
        file_path_frame = ttk.Frame(file_frame)
        file_path_frame.grid(row=1, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=(0, 10))
        ttk.Entry(file_path_frame, textvariable=self.file_path_var, width=50).grid(row=0, column=0, sticky=(tk.W, tk.E), padx=(0, 5))
        ttk.Button(file_path_frame, text="参照", command=self.browse_file).grid(row=0, column=1)
        file_path_frame.columnconfigure(0, weight=1)
        
        # シート選択
        sheet_frame = ttk.Frame(file_frame)
        sheet_frame.grid(row=2, column=0, columnspan=3, sticky=(tk.W, tk.E))
        ttk.Label(sheet_frame, text="シート名:", font=('', 9, 'bold')).grid(row=0, column=0, sticky=tk.W, padx=(0, 10))
        self.sheet_var = tk.StringVar(value="CIGS")
        self.sheet_combo = ttk.Combobox(sheet_frame, textvariable=self.sheet_var, width=20, state="readonly")
        self.sheet_combo.grid(row=0, column=1, padx=(0, 10))
        self.sheet_combo.bind('<<ComboboxSelected>>', self.on_sheet_change)
        ttk.Button(sheet_frame, text="📥 読み込み", command=self.load_commands).grid(row=0, column=2)
        
        file_frame.columnconfigure(0, weight=1)
        
        # シリアルポート設定フレーム（右側）
        serial_frame = ttk.LabelFrame(top_frame, text="📡 シリアルポート設定", padding="10")
        serial_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(10, 0))
        
        # COMポート設定
        com_frame = ttk.Frame(serial_frame)
        com_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=(0, 10))
        com_label_frame = ttk.Frame(com_frame)
        com_label_frame.grid(row=0, column=0, sticky=(tk.W, tk.E))
        ttk.Label(com_label_frame, text="COMポート:", font=('', 9, 'bold')).grid(row=0, column=0, sticky=tk.W, pady=(0, 3))
        ttk.Button(com_label_frame, text="🔄", command=self.refresh_com_ports, width=3).grid(row=0, column=1, sticky=tk.E, padx=(5, 0))
        self.com_port_var = tk.StringVar(value="COM3")
        self.com_combo = ttk.Combobox(com_frame, textvariable=self.com_port_var, width=25, state="readonly")
        self.com_combo.grid(row=1, column=0, sticky=(tk.W, tk.E))
        # COMポート変更時の保存処理を追加
        self.com_combo.bind('<<ComboboxSelected>>', self.on_com_port_change)
        com_label_frame.columnconfigure(0, weight=1)
        com_frame.columnconfigure(0, weight=1)
        
        # ボーレート設定
        baud_frame = ttk.Frame(serial_frame)
        baud_frame.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=(0, 10))
        ttk.Label(baud_frame, text="ボーレート:", font=('', 9, 'bold')).grid(row=0, column=0, sticky=tk.W, pady=(0, 3))
        self.baud_rate_var = tk.StringVar(value="9600")
        baud_entry = ttk.Entry(baud_frame, textvariable=self.baud_rate_var, width=25, justify='center')
        baud_entry.grid(row=1, column=0, sticky=(tk.W, tk.E))
        # ボーレート変更時の保存処理を追加
        self.baud_rate_var.trace_add('write', self.on_baud_rate_change)
        baud_frame.columnconfigure(0, weight=1)
        
        serial_frame.columnconfigure(0, weight=1)
        
        top_frame.columnconfigure(0, weight=1)
        top_frame.columnconfigure(1, weight=1)
        
        # コマンド設定フレーム（1行のみ）
        cmd_frame = ttk.LabelFrame(main_frame, text="🎯 コマンド設定", padding="10")
        cmd_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(15, 15))

        # 中段フレーム（保存されたコマンド＋通信ログを配置）
        middle_frame = ttk.Frame(main_frame)
        middle_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 15))
        middle_frame.columnconfigure(0, weight=1)  # 保存されたコマンド（1の比率）
        middle_frame.columnconfigure(1, weight=2)  # 通信ログ（2の比率）
        middle_frame.rowconfigure(0, weight=1)
        
        # フィルター設定
        filter_frame = ttk.Frame(cmd_frame)
        filter_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), padx=(0, 30))
        
        # カテゴリー選択
        cat_frame = ttk.Frame(filter_frame)
        cat_frame.grid(row=0, column=0, sticky=tk.W, padx=(0, 15))
        ttk.Label(cat_frame, text="カテゴリー:", font=('', 9, 'bold')).grid(row=0, column=0, sticky=tk.W, pady=(0, 3))
        self.category_var = tk.StringVar(value="全て")
        self.category_combo = ttk.Combobox(cat_frame, textvariable=self.category_var, width=15, state="readonly")
        self.category_combo.grid(row=1, column=0)
        self.category_combo.bind('<<ComboboxSelected>>', self.on_category_change)
        
        # コマンド名選択
        name_frame = ttk.Frame(filter_frame)
        name_frame.grid(row=0, column=1, sticky=tk.W, padx=(0, 15))
        ttk.Label(name_frame, text="コマンド名:", font=('', 9, 'bold')).grid(row=0, column=0, sticky=tk.W, pady=(0, 3))
        self.name_var = tk.StringVar(value="全て")
        self.name_combo = ttk.Combobox(name_frame, textvariable=self.name_var, width=20, state="readonly")
        self.name_combo.grid(row=1, column=0)
        self.name_combo.bind('<<ComboboxSelected>>', self.on_name_change)
        
        # コマンドバイト入力欄を格納するフレーム
        bytes_container = ttk.Frame(filter_frame)
        bytes_container.grid(row=0, column=2, sticky=(tk.W, tk.E), padx=(0, 15))
        
        # コマンドバイト入力欄（横一列）
        self.command_bytes = []
        self.param_labels = []
        self.entry_widgets = []  # Entry widgetを保存するリスト
        
        for i in range(9):
            # 個別のバイトフレーム
            byte_frame = ttk.Frame(bytes_container)
            byte_frame.grid(row=0, column=i, padx=2)
            
            # ラベル（初期値を設定）
            if i == 0:
                label_text = "CMD ID"
                label_color = "#2E86AB"  # 青色
            else:
                label_text = f"param{i}"
                label_color = "#A23B72"  # 紫色
            
            label = ttk.Label(byte_frame, text=label_text, font=('', 8, 'bold'), anchor='center')
            label.grid(row=0, column=0, pady=(0, 3), sticky=(tk.W, tk.E))
            self.param_labels.append(label)
            
            # 入力ボックス
            var = tk.StringVar(value="00")
            self.command_bytes.append(var)
            entry = ttk.Entry(byte_frame, textvariable=var, width=5, justify='center', font=('Courier', 9, 'bold'))
            entry.grid(row=1, column=0)
            self.entry_widgets.append(entry)  # Entry widgetを保存
            
            # CMD IDの場合は読み取り専用
            if i == 0:
                entry.configure(state='readonly')
            else:
                # 入力制限とイベントの設定（CMD ID以外）
                # 通常の編集を可能にする
                var.trace('w', lambda name, index, mode, idx=i: self.on_entry_change(idx))
                entry.bind('<FocusIn>', lambda event, idx=i: self.on_entry_focus(idx))
                entry.bind('<KeyPress>', lambda event, idx=i: self.on_key_press(event, idx))
                entry.bind('<KeyRelease>', lambda event, idx=i: self.on_key_release(event, idx))
                entry.bind('<Double-Button-1>', lambda event, idx=i: self.on_double_click(idx))
                entry.bind('<FocusOut>', lambda event, idx=i: self.on_focus_out(idx))
            
            # バイトフレーム内のカラム設定
            byte_frame.columnconfigure(0, weight=1)
        
        bytes_container.columnconfigure(tuple(range(9)), weight=1)
        
        # コマンド操作ボタン
        button_frame = ttk.Frame(filter_frame)
        button_frame.grid(row=0, column=3, sticky=(tk.W, tk.E))
        
        ttk.Button(button_frame, text="📝 追加", command=self.add_to_list).grid(row=0, column=0, padx=(0, 5))
        ttk.Button(button_frame, text="🔄 リセット", command=self.reset_command_bytes).grid(row=1, column=0, padx=(0, 5))
        ttk.Button(button_frame, text="🚀 送信", command=self.send_command).grid(row=0, column=1, rowspan=2, padx=(5, 0), sticky=(tk.N, tk.S))
        
        # コマンド名変数は保持（add_to_listメソッドで使用される可能性があるため）
        self.command_name_var = tk.StringVar()
        
        # 従来のパラメータ変数も維持（互換性のため）
        self.param_vars = self.command_bytes[1:]  # Byte1-8をparam_varsとして使用
        
        cmd_frame.columnconfigure(0, weight=1)
        
        # コマンドリストフレーム（左側）
        list_frame = ttk.LabelFrame(middle_frame, text="💾 保存されたコマンド", padding="10")
        list_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(0, 5))
        
        # コマンドリスト表示
        list_container = ttk.Frame(list_frame)
        list_container.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # リストボックスとスクロールバー
        self.command_listbox = tk.Listbox(list_container, height=6, font=('Courier', 9), 
                                         selectmode=tk.SINGLE, activestyle='dotbox',
                                         selectbackground='#007acc', selectforeground='white')
        list_scrollbar = ttk.Scrollbar(list_container, orient=tk.VERTICAL, command=self.command_listbox.yview)
        self.command_listbox.configure(yscrollcommand=list_scrollbar.set)
        
        # リストボックスの選択変更イベントを追加
        self.command_listbox.bind('<<ListboxSelect>>', self.on_listbox_select)
        # 追加のマウスイベントも追加
        self.command_listbox.bind('<Button-1>', self.on_listbox_click)
        
        self.command_listbox.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        list_scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        
        # リスト操作ボタン
        list_controls = ttk.Frame(list_frame)
        list_controls.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=(10, 0))
        
        ttk.Button(list_controls, text="🚀 送信", command=self.send_selected_command).grid(row=0, column=0, padx=(0, 10))
        ttk.Button(list_controls, text="� 読み込み", command=self.load_selected_command).grid(row=0, column=1, padx=(0, 10))
        ttk.Button(list_controls, text="�🗑️ 削除", command=self.delete_selected_command).grid(row=0, column=2, padx=(0, 10))
        ttk.Button(list_controls, text="💾 保存", command=self.save_command_list).grid(row=0, column=3, padx=(0, 10))
        ttk.Button(list_controls, text="📂 読み込み", command=self.load_command_list).grid(row=0, column=4)
        
        list_container.columnconfigure(0, weight=1)
        list_container.rowconfigure(0, weight=1)
        list_frame.columnconfigure(0, weight=1)
        list_frame.rowconfigure(0, weight=1)
        
        # ログフレーム（右側）
        log_frame = ttk.LabelFrame(middle_frame, text="📋 通信ログ", padding="10")
        log_frame.grid(row=0, column=1, sticky=(tk.W, tk.E, tk.N, tk.S), padx=(5, 0))
        
        # ログ表示エリア
        log_container = ttk.Frame(log_frame)
        log_container.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        self.log_text = tk.Text(log_container, height=10, wrap=tk.WORD, font=('Consolas', 9),
                               bg='#f8f9fa', fg='#333333', selectbackground='#007acc',
                               state='normal', cursor='xterm')
        log_scrollbar = ttk.Scrollbar(log_container, orient=tk.VERTICAL, command=self.log_text.yview)
        self.log_text.configure(yscrollcommand=log_scrollbar.set)
        
        # 右クリックメニューを追加
        self.create_log_context_menu()
        
        self.log_text.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        log_scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        
        # ログクリアボタン
        log_controls = ttk.Frame(log_frame)
        log_controls.grid(row=1, column=0, sticky=(tk.W, tk.E), pady=(10, 0))
        ttk.Button(log_controls, text="🗑️ ログクリア", command=self.clear_log).grid(row=0, column=0, sticky=tk.W)
        
        log_container.columnconfigure(0, weight=1)
        log_container.rowconfigure(0, weight=1)
        log_frame.columnconfigure(0, weight=1)
        log_frame.rowconfigure(0, weight=1)
        
        # グリッドの重み設定
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(2, weight=1)  # 中段（コマンド設定＋保存されたコマンド＋通信ログ）
        cmd_frame.columnconfigure(0, weight=1)
        
        # パラメータラベルを初期化
        self.update_parameter_labels()
    
    def create_log_context_menu(self):
        """ログテキストエリア用の右クリックメニューを作成"""
        self.log_context_menu = tk.Menu(self.root, tearoff=0)
        self.log_context_menu.add_command(label="全て選択", command=self.select_all_log)
        self.log_context_menu.add_command(label="コピー", command=self.copy_log_selection)
        self.log_context_menu.add_separator()
        self.log_context_menu.add_command(label="ログクリア", command=self.clear_log)
        
        # 右クリックイベントをバインド
        self.log_text.bind("<Button-3>", self.show_log_context_menu)
        
        # キーボードショートカットを追加
        self.log_text.bind("<Control-a>", lambda e: self.select_all_log())
        self.log_text.bind("<Control-c>", lambda e: self.copy_log_selection())
    
    def show_log_context_menu(self, event):
        """右クリックメニューを表示"""
        try:
            # 選択状態によってメニュー項目を有効/無効化
            if self.log_text.tag_ranges(tk.SEL):
                self.log_context_menu.entryconfig("コピー", state="normal")
            else:
                self.log_context_menu.entryconfig("コピー", state="disabled")
            
            self.log_context_menu.tk_popup(event.x_root, event.y_root)
        finally:
            self.log_context_menu.grab_release()
    
    def select_all_log(self):
        """ログテキスト全体を選択"""
        self.log_text.tag_add(tk.SEL, "1.0", tk.END)
        self.log_text.mark_set(tk.INSERT, "1.0")
        self.log_text.see(tk.INSERT)
    
    def copy_log_selection(self):
        """選択されたログテキストをクリップボードにコピー"""
        try:
            selected_text = self.log_text.selection_get()
            self.root.clipboard_clear()
            self.root.clipboard_append(selected_text)
        except tk.TclError:
            # 選択範囲がない場合は何もしない
            pass
    
    def clear_log(self):
        """ログをクリアする"""
        self.log_text.delete(1.0, tk.END)
    
    def refresh_com_ports(self):
        """利用可能なCOMポートを検索して更新"""
        try:
            # 利用可能なCOMポートを取得
            ports = serial.tools.list_ports.comports()
            port_list = []
            
            for port in ports:
                # ポート名と説明を組み合わせて表示
                if port.description and port.description != 'n/a':
                    port_display = f"{port.device} - {port.description}"
                else:
                    port_display = port.device
                port_list.append(port_display)
            
            # COMポートが見つからない場合のデフォルト値
            if not port_list:
                port_list = ["COM1", "COM2", "COM3", "COM4", "COM5"]
            
            # コンボボックスに設定
            self.com_combo['values'] = port_list
            
            # 保存されたCOMポートがあるかチェック（優先）
            saved_com_port = self.settings.get('last_com_port', '')
            current_value = self.com_port_var.get()
            
            # 保存されたCOMポートから探す
            found = False
            if saved_com_port:
                for port_display in port_list:
                    if saved_com_port in port_display or port_display.startswith(saved_com_port):
                        self.com_port_var.set(port_display)
                        found = True
                        break
            
            # 保存されたポートが見つからない場合、現在の値をリストから探す
            if not found:
                for port_display in port_list:
                    if current_value in port_display or port_display.startswith(current_value):
                        self.com_port_var.set(port_display)
                        found = True
                        break
            
            # どちらも見つからない場合は最初の項目を選択
            if not found and port_list:
                self.com_port_var.set(port_list[0])
                
        except Exception as e:
            # エラー時のデフォルト値
            default_ports = ["COM1", "COM2", "COM3", "COM4", "COM5"]
            self.com_combo['values'] = default_ports
            # 保存された値があればそれを使用、なければCOM3
            saved_com_port = self.settings.get('last_com_port', 'COM3')
            if not self.com_port_var.get():
                self.com_port_var.set(saved_com_port)
    
    def add_to_list(self):
        """現在のコマンド構成をリストに追加"""
        try:
            # カテゴリーとコマンド名を取得
            if self.selected_command is not None and self.commands_df is not None:
                # Excelから選択されたコマンドの場合
                category = self.category_var.get().strip()
                if category == "全て":
                    category = "その他"
                
                # B列からコマンド名を取得
                cmd_name_col = self.commands_df.columns[1] if len(self.commands_df.columns) > 1 else 'Name'
                command_name = self.selected_command.get(cmd_name_col, '').strip()
                
                if not command_name:
                    command_name = f"Command_{len(self.command_list)+1}"
            else:
                # 手動入力の場合
                command_name = self.command_name_var.get().strip()
                if not command_name:
                    command_name = f"Command_{len(self.command_list)+1}"
                
                category = self.category_var.get().strip() if hasattr(self, 'category_var') and self.category_var.get().strip() and self.category_var.get().strip() != "全て" else "手動入力"
            
            # カテゴリー_コマンド名の形式で名前を作成
            display_name = f"{category}_{command_name}"
            
            # 現在のコマンドバイトを取得
            command_bytes = []
            for var in self.command_bytes:
                value = var.get().strip().upper()
                if value:
                    try:
                        # 16進数として検証
                        int(value, 16)
                        # 2桁の16進数に正規化
                        if len(value) == 1:
                            value = "0" + value
                        elif len(value) > 2:
                            value = value[:2]
                        command_bytes.append(value)
                    except ValueError:
                        messagebox.showerror("エラー", f"無効な16進数値があります: {value}")
                        return
                else:
                    command_bytes.append("00")
            
            # コマンドをリストに追加
            command_data = {
                'name': display_name,
                'original_name': command_name,
                'category': category,
                'bytes': command_bytes.copy(),
                'command_string': ' '.join(command_bytes)
            }
            self.command_list.append(command_data)
            
            # リストボックスを更新
            self.update_command_listbox()
            
            # コマンド名をクリア
            self.command_name_var.set("")
            
        except Exception as e:
            messagebox.showerror("エラー", f"コマンドの追加に失敗しました: {str(e)}")
    
    def reset_command_bytes(self):
        """コマンドバイトをリセット"""
        for i, var in enumerate(self.command_bytes):
            if i == 0:
                var.set("00")  # CMD IDも00にリセット
            else:
                var.set("00")
    
    def update_command_listbox(self):
        """コマンドリストボックスを更新"""
        self.command_listbox.delete(0, tk.END)
        for i, cmd in enumerate(self.command_list):
            display_text = f"{i+1:2d}. {cmd['name']}"
            self.command_listbox.insert(tk.END, display_text)
    
    def send_selected_command(self):
        """選択されたコマンドを送信"""
        selection = self.command_listbox.curselection()
        if not selection:
            messagebox.showwarning("警告", "送信するコマンドを選択してください")
            return
        
        index = selection[0]
        if 0 <= index < len(self.command_list):
            command_data = self.command_list[index]
            self.send_command_direct(command_data['bytes'], command_data['name'])
    
    def load_selected_command(self):
        """選択されたコマンドを入力欄に読み込み"""
        selection = self.command_listbox.curselection()
        if not selection:
            messagebox.showwarning("警告", "読み込むコマンドを選択してください")
            return
        
        index = selection[0]
        if 0 <= index < len(self.command_list):
            command_data = self.command_list[index]
            
            # コマンドバイトを入力欄に設定
            for i, byte_value in enumerate(command_data['bytes']):
                if i < len(self.command_bytes):
                    self.command_bytes[i].set(byte_value)
    
    def delete_selected_command(self):
        """選択されたコマンドを削除"""
        selection = self.command_listbox.curselection()
        if not selection:
            messagebox.showwarning("警告", "削除するコマンドを選択してください")
            return
        
        index = selection[0]
        if 0 <= index < len(self.command_list):
            command_data = self.command_list[index]
            result = messagebox.askyesno("確認", f"コマンド '{command_data['name']}' を削除しますか？")
            if result:
                deleted_command = self.command_list.pop(index)
                self.update_command_listbox()
    
    def on_listbox_select(self, event):
        """リストボックスの選択変更時に自動的にコマンドを読み込む"""
        selection = self.command_listbox.curselection()
        if selection:
            index = selection[0]
            if 0 <= index < len(self.command_list):
                command_data = self.command_list[index]
                
                # コマンドバイトを入力欄に設定
                for i, byte_value in enumerate(command_data['bytes']):
                    if i < len(self.command_bytes):
                        self.command_bytes[i].set(byte_value)
    
    def on_listbox_click(self, event):
        """リストボックスのクリック時の処理"""
        # 少し遅延させてから選択処理を実行
        self.root.after(10, lambda: self.on_listbox_select(event))
    
    def on_entry_change(self, index):
        """入力値変更時の処理（2文字制限と自動移動）"""
        if index == 0:  # CMD IDは処理しない
            return
            
        var = self.command_bytes[index]
        value = var.get().upper()
        
        # 16進数以外の文字を除去
        filtered_value = ''.join(c for c in value if c in '0123456789ABCDEF')
        
        # 2文字制限
        if len(filtered_value) > 2:
            filtered_value = filtered_value[:2]
        
        # 値を更新（無限ループを防ぐため、変更があった場合のみ）
        if value != filtered_value:
            # カーソル位置を保存
            entry = self.entry_widgets[index]
            cursor_pos = entry.index(tk.INSERT)
            var.set(filtered_value)
            # カーソル位置を復元（適切な範囲内で）
            try:
                new_pos = min(cursor_pos, len(filtered_value))
                entry.icursor(new_pos)
            except:
                pass
            return
        
        # 2文字入力されたら次のボックスに移動（自動移動は任意にする）
        # この部分はコメントアウトして、手動でTabキーまたはクリックで移動するようにする
        # if len(filtered_value) == 2 and index < 8:
        #     self.entry_widgets[index + 1].focus()
    
    def on_entry_focus(self, index):
        """フォーカス取得時の処理（全選択は任意にする）"""
        if index == 0:  # CMD IDは処理しない
            return
            
        entry = self.entry_widgets[index]
        # ダブルクリックの場合のみ全選択する
        # 通常のクリック/Tabフォーカスでは全選択しない
        # self.root.after(10, lambda: entry.select_range(0, tk.END))
    
    def on_key_press(self, event, index):
        """キー押下時の処理（16進数文字のみ許可）"""
        if index == 0:  # CMD IDは処理しない
            return
            
        # 制御キー（Backspace、Delete、Tab、Arrow keys、Ctrl+A等）は許可
        if len(event.keysym) > 1:
            return
        
        # 16進数文字（0-9, A-F, a-f）のみ許可
        if event.char.upper() not in '0123456789ABCDEF':
            return 'break'  # イベントを停止
        
        # 2文字を超える場合は入力を制限
        current_value = self.command_bytes[index].get()
        if len(current_value) >= 2:
            # 選択範囲がある場合は置き換えを許可
            entry = self.entry_widgets[index]
            try:
                if entry.selection_present():
                    return  # 選択範囲がある場合は置き換えを許可
            except:
                pass
            return 'break'  # 選択範囲がない場合は入力を制限
    
    def on_key_release(self, event, index):
        """キー離した時の処理（自動フォーマット）"""
        if index == 0:  # CMD IDは処理しない
            return
            
        # 1文字だけの場合は先頭に0を付加
        var = self.command_bytes[index]
        value = var.get().strip().upper()
        
        # フォーカスが外れる時に1文字の場合は0埋めする
        if len(value) == 1 and event.keysym in ['Tab', 'Return', 'FocusOut']:
            var.set('0' + value)
    
    def on_double_click(self, index):
        """ダブルクリック時の処理（全選択）"""
        if index == 0:  # CMD IDは処理しない
            return
            
        entry = self.entry_widgets[index]
        entry.select_range(0, tk.END)
    
    def on_focus_out(self, index):
        """フォーカスが外れる時の処理（1文字の場合0埋め）"""
        if index == 0:  # CMD IDは処理しない
            return
            
        var = self.command_bytes[index]
        value = var.get().strip().upper()
        
        # 1文字だけの場合は先頭に0を付加
        if len(value) == 1:
            var.set('0' + value)
        elif len(value) == 0:
            var.set('00')
    
    def save_command_list(self):
        """コマンドリストをファイルに保存"""
        if not self.command_list:
            messagebox.showwarning("警告", "保存するコマンドがありません")
            return
        
        try:
            filename = filedialog.asksaveasfilename(
                title="コマンドリストを保存",
                defaultextension=".json",
                filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
            )
            
            if filename:
                import json
                with open(filename, 'w', encoding='utf-8') as f:
                    json.dump(self.command_list, f, ensure_ascii=False, indent=2)
                
                messagebox.showinfo("成功", "コマンドリストを保存しました")
        
        except Exception as e:
            messagebox.showerror("エラー", f"保存に失敗しました: {str(e)}")
    
    def load_command_list(self):
        """コマンドリストをファイルから読み込み"""
        try:
            filename = filedialog.askopenfilename(
                title="コマンドリストを読み込み",
                filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
            )
            
            if filename:
                import json
                with open(filename, 'r', encoding='utf-8') as f:
                    loaded_list = json.load(f)
                
                # 既存のリストに追加するか確認
                if self.command_list:
                    result = messagebox.askyesnocancel(
                        "確認", 
                        "既存のコマンドリストがあります。\n\n"
                        "はい: 既存リストに追加\n"
                        "いいえ: 既存リストを置き換え\n"
                        "キャンセル: 読み込みを中止"
                    )
                    if result is None:  # キャンセル
                        return
                    elif result:  # はい（追加）
                        self.command_list.extend(loaded_list)
                    else:  # いいえ（置き換え）
                        self.command_list = loaded_list
                else:
                    self.command_list = loaded_list
                
                self.update_command_listbox()
                messagebox.showinfo("成功", f"コマンドリストを読み込みました（{len(loaded_list)}個）")
        
        except Exception as e:
            messagebox.showerror("エラー", f"読み込みに失敗しました: {str(e)}")
    
    def browse_file(self):
        """ファイル選択ダイアログを開く"""
        filename = filedialog.askopenfilename(
            title="コマンドファイルを選択",
            filetypes=[("Excel files", "*.xlsx"), ("All files", "*.*")]
        )
        if filename:
            self.file_path_var.set(filename)
            self.update_sheet_list()
    
    def update_sheet_list(self):
        """Excelファイルのシート名リストを更新"""
        try:
            file_path = self.file_path_var.get()
            if file_path and os.path.exists(file_path):
                # Excelファイルからシート名を取得
                xls = pd.ExcelFile(file_path)
                sheet_names = xls.sheet_names
                self.sheet_combo['values'] = sheet_names
                
                # デフォルトでCIGSシートを選択、なければ最初のシートを選択
                if 'CIGS' in sheet_names:
                    self.sheet_var.set('CIGS')
                elif sheet_names:
                    self.sheet_var.set(sheet_names[0])
                
                # 設定から前回のシート名を復元
                last_sheet = self.settings.get('last_sheet', '')
                if last_sheet and last_sheet in sheet_names:
                    self.sheet_var.set(last_sheet)
                
        except Exception as e:
            pass
    
    def load_commands(self):
        """コマンドファイルを読み込む"""
        try:
            # デフォルトパスの設定
            if not self.file_path_var.get():
                username = os.environ.get("USERNAME")
                default_file = fr"C:\Users\{username}\Chiba Institute of Technology\5号機 - ドキュメント\General\00_MMJ文書\ICD\MMJ_ICD_018_IICD\MMJ_ICD_021_IICD_05_Uplink_command.xlsx"
                if os.path.exists(default_file):
                    self.file_path_var.set(default_file)
                    self.update_sheet_list()
            
            file_path = self.file_path_var.get()
            if not file_path or not os.path.exists(file_path):
                return
            
            # 選択されたシート名を取得
            sheet_name = self.sheet_var.get()
            if not sheet_name:
                return
            
            # Excelファイル読み込み
            self.commands_df = pd.read_excel(file_path, sheet_name=sheet_name)
            
            # フィルタリング用のデータを更新
            self.update_filter_options()
            
        except Exception as e:
            pass
    
    def update_filter_options(self):
        """フィルタ選択肢を更新"""
        if self.commands_df is None:
            return
        
        # カテゴリーの選択肢を更新
        categories = ["全て"] + sorted(self.commands_df['Category'].dropna().unique().tolist())
        self.category_combo['values'] = categories
        self.category_var.set("全て")
        
        # Nameの選択肢を更新（初期は全て表示）- B列のCMD Nameを使用
        cmd_name_col = self.commands_df.columns[1] if len(self.commands_df.columns) > 1 else 'Name'
        names = ["全て"] + sorted(self.commands_df[cmd_name_col].dropna().unique().tolist())
        self.name_combo['values'] = names
        self.name_var.set("全て")
    
    def on_category_change(self, event=None):
        """カテゴリー選択変更時の処理"""
        selected_category = self.category_var.get()
        
        # B列のCMD Name列を取得
        cmd_name_col = self.commands_df.columns[1] if len(self.commands_df.columns) > 1 else 'Name'
        
        if selected_category == "全て":
            # 全てのNameを表示
            names = ["全て"] + sorted(self.commands_df[cmd_name_col].dropna().unique().tolist())
        else:
            # 選択されたカテゴリーのNameのみ表示
            filtered_df = self.commands_df[self.commands_df['Category'] == selected_category]
            names = ["全て"] + sorted(filtered_df[cmd_name_col].dropna().unique().tolist())
        
        self.name_combo['values'] = names
        self.name_var.set("全て")
    
    def on_name_change(self, event=None):
        """Name選択変更時の処理"""
        # 選択されたコマンドをself.selected_commandに設定
        selected_category = self.category_var.get()
        selected_name = self.name_var.get()
        
        # B列のCMD Name列を取得
        cmd_name_col = self.commands_df.columns[1] if len(self.commands_df.columns) > 1 else 'Name'
        
        if selected_name != "全て":
            # 指定されたカテゴリーとNameでコマンドを特定
            filtered_df = self.commands_df.copy()
            
            if selected_category != "全て":
                filtered_df = filtered_df[filtered_df['Category'] == selected_category]
            
            filtered_df = filtered_df[filtered_df[cmd_name_col] == selected_name]
            
            if len(filtered_df) > 0:
                self.selected_command = filtered_df.iloc[0]
                self.update_command_bytes()
            else:
                self.selected_command = None
        else:
            self.selected_command = None
    
    def update_command_bytes(self):
        """選択されたコマンドの情報をバイト表示に反映"""
        if self.selected_command is None:
            return
        
        # CMD ID（Byte0）を設定 - D列（インデックス3）のCMD IDを使用
        cmd_id_col = self.commands_df.columns[3] if len(self.commands_df.columns) > 3 else 'CmdID'
        cmd_id = self.selected_command.get(cmd_id_col, '')
        if isinstance(cmd_id, (int, float)):
            cmd_id_hex = f"{int(cmd_id):02X}"
        elif isinstance(cmd_id, str):
            try:
                cmd_id_hex = f"{int(cmd_id, 16):02X}"
            except ValueError:
                try:
                    cmd_id_hex = f"{int(cmd_id):02X}"
                except ValueError:
                    cmd_id_hex = "00"
        else:
            cmd_id_hex = "00"
        
        self.command_bytes[0].set(cmd_id_hex)
        
        # パラメータ（Byte1-8）を設定 - 常に00で固定
        for i in range(8):
            self.command_bytes[i+1].set("00")
        
        # ラベルを更新してパラメータ名を表示
        self.update_parameter_labels()
    
    def update_parameter_labels(self):
        """パラメータラベルを更新"""
        if self.selected_command is None or self.commands_df is None:
            # コマンドが選択されていない場合またはExcelデータがない場合は固定ラベル
            self.param_labels[0].config(text="CMD ID")
            for i in range(8):
                self.param_labels[i+1].config(text=f"param{i+1}")
        else:
            # コマンドが選択されている場合はExcelからの情報を使用
            # CMD IDラベルは固定
            self.param_labels[0].config(text="CMD ID")
            
            # パラメータラベルを更新 - E列以降のカラム名または値を使用（E=インデックス4からL=インデックス11）
            for i in range(8):
                # E列以降のカラム（E, F, G, H, I, J, K, L列）
                col_index = i + 4  # E列は5番目（インデックス4）
                col_names = list(self.commands_df.columns)
                
                if col_index < len(col_names):
                    param_name = col_names[col_index]
                    # 選択された行のセル値を取得
                    param_value = self.selected_command.get(param_name, '')
                    
                    # セル値が存在し、空でない場合はそれをラベルに使用
                    if pd.notna(param_value) and str(param_value).strip() and str(param_value).strip() != '-':
                        label_text = str(param_value).strip()
                        self.param_labels[i+1].config(text=label_text)
                    else:
                        # セル値が空の場合はカラム名を使用、ただし"Unnamed"の場合は"-"を表示
                        if "Unnamed" in str(param_name):
                            self.param_labels[i+1].config(text="-")
                        else:
                            self.param_labels[i+1].config(text=param_name)
                else:
                    self.param_labels[i+1].config(text=f"param{i+1}")
        
        # UIの更新を強制
        self.root.update_idletasks()
    
    def reset_filters(self):
        """フィルタをリセット"""
        self.category_var.set("全て")
        self.name_var.set("全て")
        self.update_filter_options()
    
    def display_commands(self):
        """フィルタリングされたコマンドを表示"""
        pass  # テーブル表示は削除
    
    def on_command_select(self, event):
        """コマンド選択時の処理"""
        pass  # テーブル選択は削除
    
    def send_command(self):
        """BOSS_PIC_Simulater.pyを呼び出してコマンドを送信"""
        if self.selected_command is None:
            # 手動入力のコマンドを送信
            command_bytes = []
            for i, var in enumerate(self.command_bytes):
                value = var.get().strip().upper()
                if value:
                    try:
                        # 16進数として検証
                        int(value, 16)
                        # 2桁の16進数に正規化
                        if len(value) == 1:
                            value = "0" + value
                        elif len(value) > 2:
                            value = value[:2]
                        command_bytes.append(value)
                    except ValueError:
                        self.log_message(f"エラー: Byte{i}に無効な16進数値が入力されています: {value}")
                        return
                else:
                    command_bytes.append("00")
            
            self.send_command_direct(command_bytes, "手動入力")
        else:
            # Excelから選択されたコマンドを送信
            self.send_excel_command()
    
    def send_command_direct(self, command_bytes, command_name):
        """指定されたコマンドバイトを直接送信"""
        # BOSS PIC Simulatorが起動中の場合は自動入力を試行
        if self.boss_simulator_process and self.boss_simulator_process.poll() is None:
            self.send_to_boss_simulator(command_bytes, command_name)
        else:
            # BOSS PIC Simulatorが起動していない場合は従来の方法
            self.send_command_direct_legacy(command_bytes, command_name)
    
    def send_command_direct_legacy(self, command_bytes, command_name):
        """従来の方法でコマンドを送信（単発実行）"""
        try:
            # BOSS_PIC_Simulater.pyのパス
            simulator_path = Path(__file__).parent / "BOSS_PIC_Simulater.py"
            
            if not simulator_path.exists():
                self.log_message("エラー: BOSS_PIC_Simulater.pyが見つかりません")
                return
            
            # 完全なコマンド（9バイト）を構築
            full_command = " ".join(command_bytes)
            
            # COMポート名を抽出（表示形式から実際のポート名を取得）
            com_port_display = self.com_port_var.get()
            com_port = com_port_display.split(' - ')[0] if ' - ' in com_port_display else com_port_display
            
            # コマンドを16進数配列形式でログに表示
            hex_array = "[" + " ".join(command_bytes) + "]"
            self.log_message(f"MIS MCU >>> {hex_array} >>> BOSS")
            
            # コマンドライン引数を構築
            cmd_args = [
                sys.executable, str(simulator_path),
                "--port", com_port,
                "--baud", self.baud_rate_var.get(),
                "--command", full_command
            ]
            
            # BOSS_PIC_Simulater.pyを実行
            result = subprocess.run(cmd_args, capture_output=True, text=True, timeout=30)
            
            if result.returncode == 0:
                if result.stdout:
                    # BOSSからの応答をログに表示（重要な通信情報のみ抽出）
                    response_lines = result.stdout.strip().split('\n')
                    for line in response_lines:
                        if line.strip():
                            # 通信ログ行を抽出（BOSS > > > または BOSS < < < を含む行）
                            if '> > >' in line or '< < <' in line:
                                # ANSI エスケープシーケンスを除去
                                import re
                                clean_line = re.sub(r'\x1b\[[0-9;]*m', '', line.strip())
                                
                                # タイムスタンプと通信方向を含む行を抽出
                                if 'BOSS > > >' in clean_line:
                                    # BOSS送信: [AA C0 88 ...] 部分を抽出
                                    match = re.search(r'\[([A-F0-9 ]+)\]', clean_line)
                                    if match:
                                        hex_data = match.group(1)
                                        self.log_message(f"BOSS >>> [{hex_data}] >>> MIS MCU")
                                elif 'BOSS < < <' in clean_line:
                                    # BOSS受信: [AA 5F 5F] 部分を抽出
                                    match = re.search(r'\[([A-F0-9 ]+)\]', clean_line)
                                    if match:
                                        hex_data = match.group(1)
                                        self.log_message(f"MIS MCU >>> [{hex_data}] >>> BOSS")
                            # ステータス情報のみ表示
                            elif 'Finished mission' in line:
                                self.log_message(f"BOSS >>> [STATUS: Mission Finished] >>> MIS MCU")
                            elif 'power off' in line:
                                self.log_message(f"BOSS >>> [STATUS: Power Off] >>> MIS MCU")
                else:
                    self.log_message(f"BOSS >>> [OK] >>> MIS MCU")
            else:
                self.log_message(f"BOSS >>> [ERROR (コード: {result.returncode})] >>> MIS MCU")
                if result.stderr:
                    error_lines = result.stderr.strip().split('\n')
                    for line in error_lines:
                        if line.strip():
                            self.log_message(f"BOSS >>> [ERROR: {line.strip()}] >>> MIS MCU")
            
        except subprocess.TimeoutExpired:
            self.log_message(f"BOSS >> TIMEOUT >> {command_name}")
        except Exception as e:
            self.log_message(f"BOSS >> ERROR: {str(e)} >> {command_name}")
    
    def log_message(self, message):
        """ログメッセージをテキストエリアに追加"""
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        formatted_message = f"[{current_time}] {message}"
        
        # テキストエリアに追加（常に編集可能状態を維持）
        self.log_text.insert(tk.END, formatted_message + "\n")
        
        # 自動スクロール
        self.log_text.see(tk.END)
        
        # UIを更新
        self.root.update_idletasks()
    
    def send_excel_command(self):
        """Excelから選択されたコマンドを送信（従来の処理）"""
        try:
            # BOSS_PIC_Simulater.pyのパス
            simulator_path = Path(__file__).parent / "BOSS_PIC_Simulater.py"
            
            if not simulator_path.exists():
                self.log_message("エラー: BOSS_PIC_Simulater.pyが見つかりません")
                return
            
            # 9バイトのコマンドを構築
            command_bytes = []
            for i, var in enumerate(self.command_bytes):
                value = var.get().strip().upper()
                if value:
                    try:
                        # 16進数として検証
                        int(value, 16)
                        # 2桁の16進数に正規化
                        if len(value) == 1:
                            value = "0" + value
                        elif len(value) > 2:
                            value = value[:2]
                        command_bytes.append(value)
                    except ValueError:
                        self.log_message(f"エラー: Byte{i}に無効な16進数値が入力されています: {value}")
                        return
                else:
                    command_bytes.append("00")
            
            # 完全なコマンド（9バイト）を構築
            full_command = " ".join(command_bytes)
            
            # 通信ログとして表示
            sheet_name = self.sheet_var.get()
            cmd_name_col = self.commands_df.columns[1] if len(self.commands_df.columns) > 1 else 'Name'
            cmd_name = self.selected_command.get(cmd_name_col, '')
            
            # COMポート名を抽出（表示形式から実際のポート名を取得）
            com_port_display = self.com_port_var.get()
            com_port = com_port_display.split(' - ')[0] if ' - ' in com_port_display else com_port_display
            
            self.log_message(f"{sheet_name} >> {full_command} >> BOSS")
            
            # コマンドライン引数を構築
            cmd_args = [
                sys.executable, str(simulator_path),
                "--port", com_port,
                "--baud", self.baud_rate_var.get(),
                "--command", full_command
            ]
            
            # BOSS_PIC_Simulater.pyを実行
            result = subprocess.run(cmd_args, capture_output=True, text=True, timeout=30)
            
            if result.returncode == 0:
                if result.stdout:
                    # BOSSからの応答をログに表示
                    response_lines = result.stdout.strip().split('\n')
                    for line in response_lines:
                        if line.strip():
                            self.log_message(f"BOSS >> {line.strip()} >> {sheet_name}")
                else:
                    self.log_message(f"BOSS >> OK >> {sheet_name}")
            else:
                self.log_message(f"BOSS >> ERROR (コード: {result.returncode}) >> {sheet_name}")
                if result.stderr:
                    error_lines = result.stderr.strip().split('\n')
                    for line in error_lines:
                        if line.strip():
                            self.log_message(f"BOSS >> {line.strip()} >> {sheet_name}")
            
        except subprocess.TimeoutExpired:
            sheet_name = self.sheet_var.get()
            self.log_message(f"BOSS >> TIMEOUT >> {sheet_name}")
        except Exception as e:
            sheet_name = self.sheet_var.get()
            self.log_message(f"BOSS >> ERROR: {str(e)} >> {sheet_name}")
    
    def load_settings(self):
        """設定ファイルから設定を読み込み"""
        try:
            if self.config_file.exists():
                with open(self.config_file, 'r', encoding='utf-8') as f:
                    self.settings = json.load(f)
            else:
                self.settings = {}
        except Exception as e:
            self.settings = {}
        
        # 設定を各UIコントロールに適用
        self.apply_settings_to_ui()
    
    def save_settings(self):
        """設定をファイルに保存"""
        try:
            self.settings['last_sheet'] = self.sheet_var.get()
            self.settings['last_com_port'] = self.com_port_var.get()
            self.settings['last_baud_rate'] = self.baud_rate_var.get()
            with open(self.config_file, 'w', encoding='utf-8') as f:
                json.dump(self.settings, f, ensure_ascii=False, indent=2)
        except Exception as e:
            pass
    
    def apply_settings_to_ui(self):
        """設定をUIコントロールに適用"""
        try:
            # シートの設定
            if 'last_sheet' in self.settings:
                self.sheet_var.set(self.settings['last_sheet'])
            
            # COMポートの設定
            if 'last_com_port' in self.settings:
                self.com_port_var.set(self.settings['last_com_port'])
            
            # ボーレートの設定
            if 'last_baud_rate' in self.settings:
                self.baud_rate_var.set(self.settings['last_baud_rate'])
        except Exception as e:
            pass
    
    def on_com_port_change(self, event=None):
        """COMポート変更時の処理"""
        self.save_settings()
    
    def on_baud_rate_change(self, *args):
        """ボーレート変更時の処理"""
        self.save_settings()
    
    def on_sheet_change(self, event=None):
        """シート変更時の処理"""
        self.save_settings()
    
    def start_boss_pic_simulator(self):
        """BOSS PIC Simulatorをインタラクティブモードで起動"""
        try:
            # BOSS_PIC_Simulater.pyのパス
            simulator_path = Path(__file__).parent / "BOSS_PIC_Simulater.py"
            
            if not simulator_path.exists():
                self.log_message("警告: BOSS_PIC_Simulater.pyが見つかりません")
                return
            
            # デフォルトのCOMポートとボーレートを取得
            com_port_display = self.com_port_var.get()
            com_port = com_port_display.split(' - ')[0] if ' - ' in com_port_display else com_port_display
            baud_rate = self.baud_rate_var.get()
            
            # 新しいコマンドプロンプトウィンドウでBOSS PIC Simulatorをインタラクティブモードで起動
            cmd_args = [
                "cmd", "/c", "start", "cmd", "/k", 
                "python", str(simulator_path), "--port", com_port, "--baud", baud_rate
            ]
            
            # バックグラウンドで実行
            self.boss_simulator_process = subprocess.Popen(
                cmd_args,
                shell=False,
                creationflags=subprocess.CREATE_NO_WINDOW
            )
            
            self.log_message(f"BOSS PIC Simulator起動: {com_port} @ {baud_rate} baud")
            
        except Exception as e:
            self.log_message(f"BOSS PIC Simulator起動エラー: {str(e)}")
    
    def send_to_boss_simulator(self, command_bytes, command_name):
        """起動中のBOSS PIC Simulatorにコマンドを送信"""
        try:
            # コマンド文字列を構築
            command_string = " ".join(command_bytes)
            
            # ログに送信内容を表示
            hex_array = "[" + " ".join(command_bytes) + "]"
            self.log_message(f"MIS MCU >>> {hex_array} >>> BOSS")
            
            # 方法1: クリップボードにコマンドをコピー
            self.root.clipboard_clear()
            self.root.clipboard_append(command_string)
            self.log_message(f"コマンドをクリップボードにコピーしました: {command_string}")
            
            # 方法2: PowerShellスクリプトを生成してBOSS PIC Simulatorに送信
            try:
                # PowerShellスクリプトを一時的に作成
                ps_script = f'''
Add-Type -AssemblyName System.Windows.Forms
$processes = Get-Process | Where-Object {{$_.MainWindowTitle -like "*cmd*" -or $_.MainWindowTitle -like "*BOSS*"}}
if ($processes) {{
    $process = $processes[0]
    [System.Windows.Forms.SendKeys]::SendWait("{command_string}")
    [System.Windows.Forms.SendKeys]::SendWait("{{ENTER}}")
}}
'''
                
                # PowerShellスクリプトを実行
                subprocess.run(
                    ["powershell", "-Command", ps_script],
                    capture_output=True,
                    text=True,
                    timeout=5
                )
                
                self.log_message(f"PowerShell経由でコマンド送信試行: {command_string}")
                
            except Exception as ps_error:
                self.log_message(f"PowerShell送信エラー: {str(ps_error)}")
            
            # 使用方法をログに表示
            self.log_message("BOSS PIC Simulatorのターミナルで Ctrl+V を押してEnterキーを押すか、上記の自動送信をご確認ください")
            
        except Exception as e:
            self.log_message(f"コマンド送信エラー: {str(e)}")
            # エラーの場合は従来の方法で送信
            self.send_command_direct_legacy(command_bytes, command_name)
    
    def on_closing(self):
        """アプリケーション終了時の処理"""
        self.save_settings()
        
        # BOSS PIC Simulatorプロセスを終了
        if self.boss_simulator_process:
            try:
                self.boss_simulator_process.terminate()
                self.boss_simulator_process.wait(timeout=3)
            except Exception:
                # 強制終了
                try:
                    self.boss_simulator_process.kill()
                except Exception:
                    pass
        
        self.root.destroy()

def main():
    root = tk.Tk()
    app = CommandUI(root)
    root.mainloop()

if __name__ == "__main__":
    main()
