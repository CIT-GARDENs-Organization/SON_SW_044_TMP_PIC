#include "mission_tools.h"
// uart.hの個別インクルードは不要です（son_tmp_main.hで解決されます）

int1 req_use_smf()
{
   fprintf(PC, "Start SMF using reqest seaquence\r\n");
   status = SMF_USE_REQ;
   is_use_smf_req_in_mission = TRUE;

   while (TRUE)
   {
      for (int16 i = 0; i < 1200; i++) // 10 min
      {
         // ★修正: uart_fetch_to_buffer() を削除し、安全なkbhitポーリングに置き換えました
         if (kbhit(BOSS))
         {
             uint8_t timeout_ms = 0;
             while(timeout_ms < 5)
             {
                 if (kbhit(BOSS)) {
                     if (boss_receive_buffer_size < RECEIVE_BUFFER_MAX) {
                         boss_receive_buffer[boss_receive_buffer_size++] = fgetc(BOSS);
                     } else {
                         fgetc(BOSS);
                     }
                     timeout_ms = 0;
                 } else {
                     delay_ms(1);
                     timeout_ms++;
                 }
             }
         }

         if (boss_receive_buffer_size > 0)
         {
            Command command = make_receive_command((uint8_t*)boss_receive_buffer, boss_receive_buffer_size);
            clear_receive_signal((uint8_t*)boss_receive_buffer, &boss_receive_buffer_size);
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
         // ★修正: 同様にkbhitポーリング処理に置き換え
         if (kbhit(BOSS))
         {
             uint8_t timeout_ms = 0;
             while(timeout_ms < 5)
             {
                 if (kbhit(BOSS)) {
                     if (boss_receive_buffer_size < RECEIVE_BUFFER_MAX) {
                         boss_receive_buffer[boss_receive_buffer_size++] = fgetc(BOSS);
                     } else {
                         fgetc(BOSS);
                     }
                     timeout_ms = 0;
                 } else {
                     delay_ms(1);
                     timeout_ms++;
                 }
             }
         }

         if (boss_receive_buffer_size > 0)
         {
            Command command = make_receive_command((uint8_t*)boss_receive_buffer, boss_receive_buffer_size);
            clear_receive_signal((uint8_t*)boss_receive_buffer, &boss_receive_buffer_size);
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
}


void finished_use_smf()
{
   status = EXECUTING_MISSION;
}

void check_and_respond_to_boss()
{
   if (kbhit(BOSS))
   {
      fgetc(BOSS);
      transmit_status();
   }
}