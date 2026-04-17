#include "son_tmp_excute_mission.h"
#include "../../hardware/mcu/uart.h"
#include "../../hardware/mcu/timer.h"
#include "../../core/logging/son_tmp_piclog.h"
#include "son_tmp_mode_mission.h"
#include "son_tmp_mode_flash.h"
#include "../../../lib/communication/communication.h"
#include "../../../lib/communication/communication_driver.h"

// ============================================================================
// グローバル変数の実体定義
// ============================================================================
unsigned int8 status = 0; // 初期状態(IDLE等)
int1 is_use_smf_req_in_mission = 0;
bool is_mission_aborted = false;

// ============================================================================
// グローバルなポーリング監視用関数 (割り込みバッファ対応版)
// ============================================================================
void check_boss_status_polling(void)
{
    static uint8_t rx_state = 0;
    static uint8_t expected_len = 0;
    static uint8_t current_frame_id = 0;

    // ハードウェア直接(kbhit)ではなく、割り込みで溜まったリングバッファを確認
    while (uart_has_data())
    {
        uint8_t c = uart_getc();

        if (rx_state == 0 && c == 0xAA)
        {
            rx_state = 1;
        }
        else if (rx_state == 1)
        {
            current_frame_id = c & 0x0F;
            if (c == 0xA1 || c == 0xC1)
            {
                expected_len = 3;
                rx_state = 2;
            }
            else if (c == 0xA3 || c == 0xC3)
            {
                expected_len = 7;
                rx_state = 2;
            }
            else if (c == 0xA0 || c == 0xC0)
            {
                expected_len = 12;
                rx_state = 2;
            }
            else
            {
                rx_state = (c == 0xAA) ? 1 : 0;
            }
        }
        else if (rx_state >= 2)
        {
            if (rx_state == 2 && current_frame_id == 0x00 && c == 0xAF)
            {
                is_mission_aborted = true;
            }

            rx_state++;

            if (rx_state == expected_len)
            {
                if (current_frame_id == 0x01)
                {
                    fprintf(PC, "\r\n[INFO] STATUS_CHECK(0x01) Received during operation. Replying status: %u\r\n", status);
                    transmit_status();
                }
                else if (current_frame_id == 0x03)
                {
                    fprintf(PC, "\r\n[INFO] TIME_SYNC(0x03) Received during operation. Replying status: %u\r\n", status);
                    transmit_status();
                }
                else if (current_frame_id == 0x00 && is_mission_aborted)
                {
                    fprintf(PC, "\r\n[WARN] ABORT Command Received during operation!\r\n");
                    transmit_ack();
                }
                rx_state = 0;
            }
        }
    }
}

int1 execute_command(Command* cmd)
{
    if (!cmd->is_exist) return false;

    // ----------------------------------------------------
    // ① BOSSからの「ミッション指示 (UPLINK_COMMAND)」の場合
    // ----------------------------------------------------
    if (cmd->frame_id == 0x00)
    {
        uint8_t cmd_id = cmd->content[0];

        fprintf(PC, "Received Command: 0x%02X\r\n", cmd_id);

        transmit_ack();
        fprintf(PC, "-> Transmitted ACK to BOSS\r\n");

        switch (cmd_id)
        {
            // 計測コマンド
            case CMD_STR:
                fprintf(PC, "[CMD] STR (0xA0)\r\n");
                status = EXECUTING_MISSION;
                execute_mission_sequence((uint8_t)cmd->content[2], 0x01);
                break;
            case CMD_STR_DEBUG:
                fprintf(PC, "[CMD] STR_DEBUG (0xA1)\r\n");
                status = EXECUTING_MISSION;
                execute_mission_sequence((uint8_t)cmd->content[2], 0x02);
                break;
            case CMD_STR_PRINT:
                fprintf(PC, "[CMD] STR_PRINT (0xA2)\r\n");
                status = EXECUTING_MISSION;
                execute_mission_sequence((uint8_t)cmd->content[2], 0x03);
                break;
            case CMD_STR_DEBUG_SAVE:
                fprintf(PC, "[CMD] STR_DEBUG_SAVE (0xA3)\r\n");
                status = EXECUTING_MISSION;
                execute_mission_sequence((uint8_t)cmd->content[2], 0x04);
                break;
            case CMD_MISSION_ABORT:
                fprintf(PC, "[CMD] MISSION_ABORT (0xAF)\r\n");
                is_mission_aborted = true;
                fprintf(PC, "Mission Aborted by Boss.\r\n");
                break;

            // PICF コマンド
            case CMD_PICF_READ:
            {
                fprintf(PC, "[CMD] PICF_READ (0x86)\r\n");
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint16_t packet_num = ((uint16_t)cmd->content[6] << 8) | (uint16_t)cmd->content[7];
                execute_picf_read(address, packet_num);
                break;
            }
            case CMD_PICF_ERASE_ALL:
                fprintf(PC, "[CMD] PICF_ERASE_ALL (0x80)\r\n");
                execute_picf_erase_all();
                break;
            case CMD_PICF_ERASE_1SECTOR:
            {
                fprintf(PC, "[CMD] PICF_ERASE_1SECTOR (0x81)\r\n");
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint8_t sector_num = cmd->content[7];
                execute_picf_erase_1sector(address, sector_num);
                break;
            }
            case CMD_PICF_ERASE_4K_SUBSECTOR:
            {
                fprintf(PC, "[CMD] PICF_ERASE_4K_SUBSECTOR (0x82)\r\n");
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint8_t sector_num = cmd->content[7];
                execute_picf_erase_4k(address, sector_num);
                break;
            }
            case CMD_PICF_ERASE_32K_SUBSECTOR:
            {
                fprintf(PC, "[CMD] PICF_ERASE_32K_SUBSECTOR (0x83)\r\n");
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint8_t sector_num = cmd->content[7];
                execute_picf_erase_32k(address, sector_num);
                break;
            }
            case CMD_PICF_WRITE_DEMO:
            {
                fprintf(PC, "[CMD] PICF_WRITE_DEMO (0x84)\r\n");
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint16_t packet_num = ((uint16_t)cmd->content[6] << 8) | (uint16_t)cmd->content[7];
                execute_picf_write_demo(address, packet_num);
                break;
            }
            case CMD_PICF_WRITE_4K_SUBSECTOR:
            {
                fprintf(PC, "[CMD] PICF_WRITE_4K_SUBSECTOR (0x85)\r\n");
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                execute_picf_write_4k(address);
                break;
            }
            case CMD_PICF_READ_ADDRESS:
                fprintf(PC, "[CMD] PICF_READ_ADDRESS (0x87)\r\n");
                execute_picf_read_address();
                break;
            case CMD_PICF_ERASE_AND_RESET:
                fprintf(PC, "[CMD] PICF_ERASE_AND_RESET (0x88)\r\n");
                execute_picf_erase_and_reset();
                break;
            case CMD_PICF_READ_AREA:
            {
                uint16_t start_pkt = ((uint16_t)cmd->content[2] << 8) | cmd->content[3];
                uint16_t req_pkts  = ((uint16_t)cmd->content[4] << 8) | cmd->content[5];
                execute_picf_read_area(cmd->content[1], start_pkt, req_pkts);
                break;
            }
            case CMD_PICF_RESET_ADDRESS:
                fprintf(PC, "[CMD] PICF_RESET_ADDRESS (0x8F)\r\n");
                execute_picf_reset_address();
                break;

            // SMF コマンド
            case CMD_SMF_COPY:
            case CMD_SMF_COPY_FORCE:
            {
                bool is_force = (cmd_id == CMD_SMF_COPY_FORCE);
                fprintf(PC, "[CMD] SMF_COPY%s (0x%02X)\r\n", is_force ? "_FORCE" : "", cmd_id);
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint16_t packet_num = ((uint16_t)cmd->content[6] << 8) | (uint16_t)cmd->content[7];
                execute_smf_direct_copy(address, packet_num, is_force);
                break;
            }
            case CMD_SMF_READ:
            case CMD_SMF_READ_FORCE:
            {
                bool is_force = (cmd_id == CMD_SMF_READ_FORCE);
                fprintf(PC, "[CMD] SMF_READ%s (0x%02X)\r\n", is_force ? "_FORCE" : "", cmd_id);
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint16_t packet_num = ((uint16_t)cmd->content[6] << 8) | (uint16_t)cmd->content[7];
                execute_smf_read(address, packet_num, is_force);
                break;
            }
            case CMD_SMF_ERASE:
            {
                bool is_force = (cmd_id == CMD_SMF_ERASE_FORCE);
                fprintf(PC, "[CMD] SMF_ERASE%s (0x%02X)\r\n", is_force ? "_FORCE" : "", cmd_id);
                uint32_t address = ((uint32_t)cmd->content[1] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[3] << 8) | (uint32_t)cmd->content[4];
                uint8_t sector_num = cmd->content[7];
                execute_smf_erase(address, sector_num, is_force);
                break;
            }
            case CMD_SMF_ERASE_FORCE:
                fprintf(PC, "[CMD] SMF_ERASE_FORCE (0x%02X)\r\n", cmd_id);
                execute_smf_erase_force();
                break;

            // その他
            case CMD_RETURN_TIME:
                fprintf(PC, "[CMD] RETURN_TIME (0xB0)\r\n");
                uint32_t current_time = get_current_sec();
                fprintf(PC, "Current Time: %lu sec\r\n", current_time);
                break;
            default:
                fprintf(PC, "Command received but not implemented yet: 0x%02X\r\n", cmd_id);
                break;
        }

        status = FINISHED;
    }
    // ----------------------------------------------------
    // ② BOSSからの「状態確認 (STATUS_CHECK)」の場合
    // ----------------------------------------------------
    else if (cmd->frame_id == 0x01)
    {
        fprintf(PC, "Received STATUS_CHECK (0x01). Replying status: %u\r\n", status);
        transmit_status();
    }
    else if (cmd->frame_id == 0x03)
    {
        uint32_t received_time = ((uint32_t)cmd->content[3] << 24) | ((uint32_t)cmd->content[2] << 16) | ((uint32_t)cmd->content[1] << 8) | (uint32_t)cmd->content[0];
        set_current_sec(received_time);
        fprintf(PC, "Received TIME_SYNC (0x03). Time updated to: %lu sec. Replying status: %u\r\n", received_time, status);
        transmit_status();
    }
    else
    {
        fprintf(PC, "Received Unknown Frame ID: 0x%02X\r\n", cmd->frame_id);
    }

    return true;
}