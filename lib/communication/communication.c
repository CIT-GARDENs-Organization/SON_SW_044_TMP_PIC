#include "mission_tools.h"
#include "../tool/calc_tools.h"

Command make_receive_command(unsigned int8 receive_signal[], int8 receive_signal_size)
{
   Command command = {0, 0, FALSE,{0x00}};
   fprintf(PC, "\r\nStart make_receive_command\r\n");
   fprintf(PC, "\t[BOSS] >>> ");
   for (int8 i = 0; i < receive_signal_size; i++)
      fprintf(PC, "%02X ", receive_signal[i]);
   fprintf(PC, "\r\n");

   int8 frame_start_position = trim_receive_signal_header(receive_signal, receive_signal_size);
   if (frame_start_position == -1)
      return command;

   unsigned int8 *frame = &receive_signal[frame_start_position];

   int8 frame_id = frame[0] & 0x0F;
   int8 content_size = get_content_size(frame_id);
   if (content_size == -1)
      return command;

   unsigned int8 receive_frame_size = receive_signal_size - frame_start_position;

   if (!check_crc(frame, receive_frame_size))
      return command;

   if(!check_device_id((frame[0] & 0xF0) >> 4))
      return command;

   command.frame_id = frame_id;
   memcpy(command.content, &frame[1], receive_frame_size-2); // '2' is for Decive ID, Frame ID and CRC
   command.size = receive_frame_size-2;
   command.is_exist = TRUE;

   // スペルミスも直しています
   fprintf(PC, "End make_receive_command\r\n\r\n");
   return command;
}

static int8 trim_receive_signal_header(unsigned int8 receive_signal[], int8 receive_signal_size)
{
   int8 i = 0;
   for(i = 0; i < receive_signal_size-1; i++) // if SFD find
      if(receive_signal[i] == SFD)
         return i+1;
   if (receive_signal[i] == SFD)
   {
      fprintf(PC, "signal end with SFD\r\n");
      return -1;
   }
   else
   {
      fprintf(PC, "don't find SFD\r\n");
      return -1;
   }

}

static int8 get_content_size(unsigned int8 frame_id)
{
   // ★追加: 新しいFrame ID (0x03) はここで直接サイズ4を返すようにします
   if (frame_id == 0x03) {
       return 4;
   }

   for (int i = 0; i < (sizeof(frame_ids) / sizeof(frame_ids[0])); i++)
      if (frame_id == frame_ids[i].id)
         return frame_ids[i].length;

   fprintf(PC, "\t-> Invalid Frame ID received: %02X\r\n", frame_id);
   return -1;
}

static int1 check_crc(unsigned int8 frame[], int8 receive_frame_size)
{
   unsigned int8 received_crc = frame[receive_frame_size-1]; // '1' is for crc
   unsigned int8 collect_crc = calc_crc8(frame, receive_frame_size-1);
   if (received_crc == collect_crc)
      return TRUE;
   else
   {
      fprintf(PC, "\t-> CRC error!\r\n");
      fprintf(PC, "\t\t-> received CRC: %02X\r\n", received_crc);
      fprintf(PC, "\t\t   collect CRC : %02X\r\n", collect_crc);
      return FALSE;
   }
}

static int1 check_device_id(unsigned int8 device_id)
{
   if (SELF_DEVICE_ID == device_id)
      return TRUE;
   else
   {
      fprintf(PC, "\t-> Invalid device ID received\r\n");
      fprintf(PC, "\t\t-> received:     %02X\r\n", device_id);
      fprintf(PC, "\t\t   My device ID: %02X\r\n", SELF_DEVICE_ID);
      return FALSE;
   }
}

// _______ Transmit _______

void transmit_command(TransmitFrameId frame_id, unsigned int8 content[], int8 size)
{
   unsigned int8 data[16];
   data[0] = SFD;
   data[1] = (BOSS_PIC << 4) | frame_id;
   memcpy(&data[2], content, size);
   int8 payload_size = 2 + size; // '2' is for Device ID and Frame ID plus CRC
   data[payload_size] = calc_crc8(&data[1], payload_size-1); // '1' is for CRC
   int8 data_size = payload_size + 1; // '1' is for CRC

   transmit(data, data_size);
}

static void transmit(unsigned int8 data[], int8 data_size)
{
   for(int i = 0; i < data_size; i++)
      fputc(data[i], BOSS);

   fprintf(PC, "\t[BOSS] <<< ");
   for(int i = 0; i < data_size; i++)
      fprintf(PC, "%02X ", data[i]);
   fprintf(PC, "\r\n");
}