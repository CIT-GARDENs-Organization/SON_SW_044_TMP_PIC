#include "mission_tools.h"
#include "../../tmp/hardware/mcu/uart.h" // uart_fetch_to_buffer を使用するためインクルード

int1 req_use_smf()
{
   fprintf(PC, "Start SMF using reqest seaquence\r\n");
   status = SMF_USE_REQ;
   is_use_smf_req_in_mission = TRUE;

   while (TRUE)
   {
      for (int16 i = 0; i < 1200; i++) // 10 min
      {
         // ★修正: ループ毎に割り込みリングバッファからデータを吸い上げる処理を追加
         uart_fetch_to_buffer();

         if (boss_receive_buffer_size > 0)
         {
            Command command = make_receive_command(boss_receive_buffer, boss_receive_buffer_size);
            clear_receive_signal(boss_receive_buffer, &boss_receive_buffer_size);
            if (command.frame_id == STATUS_CHECK)
            {
               transmit_status();
               break;
            }
            else
            {
               fprintf(PC, "Error! Receiving command inconsistent with the design\r\n");
            }
         }
         delay_ms(500);
      }

      for (int16 i = 0; i < 1200; i++) // 10 min
      {
         // ★修正: 同様にデータを吸い上げる
         uart_fetch_to_buffer();

         if (boss_receive_buffer_size > 0)
         {
            Command command = make_receive_command(boss_receive_buffer, boss_receive_buffer_size);
            clear_receive_signal(boss_receive_buffer, &boss_receive_buffer_size);
            if (command.frame_id == IS_SMF_AVAILABLE)
            {
               if (command.content[0] == ALLOW)
               {
                  fprintf(PC, "SMF use request allowed\r\n");
                  transmit_ack();
                  goto NEXT;
               }
               else
               {
                  fprintf(PC, "SMF use request denyed\r\n");
                  fprintf(PC, "Retry request to BOSS PIC\r\n");
                  transmit_ack();
                  break;
               }
            }
            else
            {
               fprintf(PC, "Error! Receiving command inconsistent with the design\r\n");
            }
         }
         delay_ms(500);
      }
   }

NEXT:
   is_use_smf_req_in_mission = FALSE;
   status = COPYING;
   return TRUE;
   fprintf(PC, "End SMF using reqest seaquence\r\n");
}


void finished_use_smf()
{
   status = EXECUTING_MISSION;
}

void check_and_respond_to_boss()
{
   if (uart_has_data())
   {
      uart_getc();
      transmit_status();
   }
}