#include "cigs/system/son_tmp_main.h"  // ヘッダーファイルから自動的にインクルードされるため不要

void main()
{
    delay_ms(100); // 起動待ち
    fprintf(PC, "\r\n--- Echo Back Test Mode (Software UART) ---\r\n");

    // 割り込みは一切使わず、無限ループでひたすら受信を待つ
    while(TRUE)
    {
        // BOSS(TeraTerm)からデータが来るかを監視 (kbhit)
        if (kbhit(BOSS))
        {
            // データが来たら読み取る
            char c = fgetc(BOSS);

            // そのままBOSS(TeraTerm)に打ち返す
            fputc(c, BOSS);

            // PC側のモニターにも「何を受け取ったか」を表示する
            fprintf(PC, "Received: %c (HEX: %02X)\r\n", c, c);
        }
    }
}

/*
void main()
{
    delay_ms(100); // wait for power stable
    fprintf(PC,"\r\n\r\n\r\n============================================================\r\n");
    fprintf(PC,"This is SHION TMP PIC.\r\n");
    fprintf(PC,"Last updated on %s %s.\r\n\r\n", __DATE__, __TIME__);

    io_init();
    setup_uart_to_boss();
    setup_timer();
    misf_init();

    piclog_make(0x00, 0x00); // PICLOG_STARTUP

    int1 is_finished = FALSE;
    fprintf(PC,"____CIGS PIC Start Operation_____\r\n\r\n");

    fprintf(PC,"waiting for BOSS PIC command");

    //Start loop
    while(!is_finished)
    {
        // handle from boss commands
        if(boss_receive_buffer_size > 0)
        {
            // ===== ここからデバッグ出力を追加 =====
            fprintf(PC, "\r\n[DEBUG] Raw RX Data: ");
            for(int i = 0; i < boss_receive_buffer_size; i++)
            {
                // バイナリデータなので16進数表記(%02X)で出力する
                fprintf(PC, "%02X ", boss_receive_buffer[i]);

                // もし本当に「abc」のようなASCII文字列を送っていて、
                // それを見たい場合は上の行をコメントアウトして下を有効にしてください。
                // fprintf(PC, "%c", boss_receive_buffer[i]);
            }
            fprintf(PC, "\r\n");
            // ===== ここまで追加 =====

            volatile Command recieve_cmd = make_receive_command(boss_receive_buffer, boss_receive_buffer_size);

            // ここでバッファがクリア(サイズ0)になる
            clear_receive_signal(boss_receive_buffer, &boss_receive_buffer_size);

            if(recieve_cmd.is_exist)
            {
                // 変更: 古い関数ではなく、新しいディスパッチャにCommand構造体を渡す
                execute_mission_command((Command*)&recieve_cmd);

                fprintf(PC,"\r\nwaiting for BOSS PIC command");
            }
        }

        // check `is break while loop`
        if(is_finished == TRUE)
            break;

        delay_ms(400);
        fprintf(PC, ".");
    }

    fprintf(PC, "\r\n\r\n======\r\n\r\nFinished process.\r\nWait for BOSS PIC turn off me");

    while (TRUE)
    {
        fprintf(PC, ".");
        delay_ms(1000);
    }

    fprintf(PC, "End main\r\n");
}
*/