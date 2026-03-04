//#include "cigs/system/son_tmp_main.h"

#include "cigs/system/son_tmp_main.h"

void main()
{
    delay_ms(100); // wait for power stable
    fprintf(PC,"\r\n\r\n\r\n============================================================\r\n");
    fprintf(PC,"This is MOMIJI CIGS PIC BBM for MIS7_BBM4.\r\n");
    fprintf(PC,"Last updated on %s %s, by Inoue.\r\n\r\n", __DATE__, __TIME__);

    io_init();
    setup_uart_to_boss();
    setup_timer();
    misf_init();

    piclog_make(0x00, 0x00); // PICLOG_STARTUP

    int1 is_finished = FALSE;
    fprintf(PC,"____CIGS PIC Start Operation_____\r\n\r\n");
    fprintf(PC,"waiting for BOSS PIC command\r\n");

    // Start loop
    while(!is_finished)
    {
        // 1. 割り込みに頼らず、自力で受信データをバッファに吸い上げる (ポーリング受信)
        // ※ TeraTermのエコーバックテストで成功した手法を応用しています
        while(kbhit(BOSS))
        {
            if (boss_receive_buffer_size < RECEIVE_BUFFER_MAX) {
                boss_receive_buffer[boss_receive_buffer_size] = fgetc(BOSS);
                boss_receive_buffer_size++;
            } else {
                fgetc(BOSS); // バッファが満杯の時は空読みして捨てる
            }
        }

        // 2. バッファにデータがあればフレーム解析へ
        if(boss_receive_buffer_size > 0)
        {
            // バッファからフレームを解析し、Command構造体を生成
            Command recieve_cmd = make_receive_command((uint8_t*)boss_receive_buffer, boss_receive_buffer_size);

            // 解析が終わった分（または不要なゴミデータ）をバッファから消去
            clear_receive_signal((uint8_t*)boss_receive_buffer, &boss_receive_buffer_size);

            // 正しいコマンドフレーム(AA C0 10 ... D0)が抽出できていた場合のみ処理を実行
            if(recieve_cmd.is_exist)
            {
                fprintf(PC, "\r\n[INFO] Valid Command Frame Received! ID: %02X\r\n", recieve_cmd.frame_id);

                // ディスパッチャ（司令塔）へコマンドを渡す
                execute_mission_command(&recieve_cmd);

                fprintf(PC,"\r\nwaiting for BOSS PIC command\r\n");
            }
        }

        // 終了フラグのチェック
        if(is_finished == TRUE)
            break;

        // ★超重要: 以前ここにあった delay_ms(400); は、シミュレータからの連続データを取りこぼす原因になるため削除しました。
        // 代わりに、少しだけCPUを休ませる微小なウェイトを入れます。
        delay_us(10);
    }

    fprintf(PC, "\r\n\r\n======\r\n\r\nFinished process.\r\nWait for BOSS PIC turn off me\r\n");

    while (TRUE)
    {
        fprintf(PC, ".");
        delay_ms(1000);
    }

    fprintf(PC, "End main\r\n");
}

/*
void main()
{
    delay_ms(100); // wait for power stable
    fprintf(PC,"\r\n\r\n\r\n============================================================\r\n");
    fprintf(PC,"This is MOMIJI CIGS PIC BBM for MIS7_BBM4.\r\n");
    fprintf(PC,"Last updated on %s %s, by Inoue.\r\n\r\n", __DATE__, __TIME__);

    io_init();
    setup_uart_to_boss();
    setup_timer();
    misf_init();

    piclog_make(0x00, 0x00); // PICLOG_STARTUP

    int1 is_finished = FALSE;
    fprintf(PC,"____CIGS PIC Start Operation_____\r\n\r\n");

    fprintf(PC,"waiting for BOSS PIC command");

    // Start loop
    while(!is_finished)
    {
        // BOSSからの受信バッファにデータがあるかチェック
        if(boss_receive_buffer_size > 0)
        {
            // 1. バッファからフレームを解析し、Command構造体を生成
            Command recieve_cmd = make_receive_command((uint8_t*)boss_receive_buffer, boss_receive_buffer_size);

            // 2. 解析が終わった分（または不要なゴミデータ）をバッファから消去
            clear_receive_signal((uint8_t*)boss_receive_buffer, &boss_receive_buffer_size);

            // 3. 正しいコマンドフレームが抽出できていた場合のみ処理を実行
            if(recieve_cmd.is_exist)
            {
                fprintf(PC, "\r\n[INFO] Valid Command Frame Received! ID: %02X\r\n", recieve_cmd.frame_id);

                // 新しいディスパッチャ（司令塔）へコマンドを渡す
                execute_mission_command(&recieve_cmd);

                fprintf(PC,"\r\nwaiting for BOSS PIC command");
            }
        }

        // 終了フラグのチェック
        if(is_finished == TRUE)
            break;

        delay_ms(400);
        fprintf(PC, ".");
    }

    fprintf(PC, "\r\n\r\n======\r\n\r\nFinished process.\r\nWait for BOSS PIC turn off me\r\n");

    while (TRUE)
    {
        fprintf(PC, ".");
        delay_ms(1000);
    }

    fprintf(PC, "End main\r\n");
}
    */