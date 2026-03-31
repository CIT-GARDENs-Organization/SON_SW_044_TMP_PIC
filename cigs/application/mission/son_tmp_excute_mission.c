#include "son_tmp_excute_mission.h"
#include "../../hardware/mcu/uart.h"
#include "../../hardware/mcu/timer.h"
#include "../../core/logging/son_tmp_piclog.h"
#include "son_tmp_mode_mission.h"
#include "son_tmp_mode_flash.h"
#include "../../../lib/communication/communication.h"

// ============================================================================
// グローバル変数の実体定義
// ============================================================================
unsigned int8 status = 0;
int1 is_use_smf_req_in_mission = 0;

int1 execute_command(Command* cmd)
{
    if (!cmd->is_exist) return false;

    uint8_t cmd_id = cmd->frame_id;
    fprintf(PC, "Received Command: 0x%02X\r\n", cmd_id);

    switch (cmd_id)
    {
        // ----------------------------------------------------
        // 計測コマンド
        // ----------------------------------------------------
        case CMD_STR:
        {
            fprintf(PC, "[CMD] STR (0xA0)\r\n");
            status = EXECUTING_MISSION;
            // 第3引数に mode = 0x01 を渡す
            execute_mission_sequence((uint8_t)cmd->content[1], (uint8_t)cmd->content[2], 0x01);
            break;
        }
        case CMD_STR_DEBUG:
        {
            fprintf(PC, "[CMD] STR_DEBUG (0xA1)\r\n");
            status = EXECUTING_MISSION;
            // 第3引数に mode = 0x02 を渡す
            execute_mission_sequence((uint8_t)cmd->content[1], (uint8_t)cmd->content[2], 0x02);
            break;
        }
        case CMD_STR_PRINT:
        {
            fprintf(PC, "[CMD] STR_PRINT (0xA2)\r\n");
            status = EXECUTING_MISSION;
            // 第3引数に mode = 0x03 を渡す
            execute_mission_sequence((uint8_t)cmd->content[1], (uint8_t)cmd->content[2], 0x03);
            break;
        }

        // ----------------------------------------------------
        // PICF (Mission Flash) コマンド
        // ----------------------------------------------------
        case CMD_PICF_READ:
        {
            fprintf(PC, "[CMD] PICF_READ (0x86)\r\n");
            execute_flash_dump();
            break;
        }
        case CMD_PICF_ERASE_ALL:
        {
            fprintf(PC, "[CMD] PICF_ERASE_ALL (0x80)\r\n");
            break;
        }
        case CMD_PICF_ERASE_1SECTOR:
        {
            fprintf(PC, "[CMD] PICF_ERASE_1SECTOR (0x81)\r\n");
            break;
        }
        case CMD_PICF_ERASE_4K_SUBSECTOR:
        {
            fprintf(PC, "[CMD] PICF_ERASE_4K_SUBSECTOR (0x82)\r\n");
            break;
        }
        case CMD_PICF_ERASE_64K_SUBSECTOR:
        {
            fprintf(PC, "[CMD] PICF_ERASE_64K_SUBSECTOR (0x83)\r\n");
            break;
        }
        case CMD_PICF_WRITE_DEMO:
        {
            fprintf(PC, "[CMD] PICF_WRITE_DEMO (0x84)\r\n");
            break;
        }
        case CMD_PICF_WRITE_4K_SUBSECTOR:
        {
            fprintf(PC, "[CMD] PICF_WRITE_4K_SUBSECTOR (0x85)\r\n");
            break;
        }
        case CMD_PICF_READ_ADDRESS:
        {
            fprintf(PC, "[CMD] PICF_READ_ADDRESS (0x87)\r\n");
            print_flash_status();
            break;
        }
        case CMD_PICF_ERASE_AND_RESET:
        {
            fprintf(PC, "[CMD] PICF_ERASE_AND_RESET (0x88)\r\n");
            break;
        }
        case CMD_PICF_READ_AREA:
        {
            fprintf(PC, "[CMD] PICF_READ_AREA (0x89)\r\n");
            break;
        }
        case CMD_PICF_RESET_ADDRESS:
        {
            fprintf(PC, "[CMD] PICF_RESET_ADDRESS (0x8F)\r\n");
            break;
        }

        // ----------------------------------------------------
        // SMF (CPLD Flash) コマンド
        // ----------------------------------------------------
        case CMD_SMF_COPY:
        {
            fprintf(PC, "[CMD] SMF_COPY (0x90)\r\n");
            status = COPYING;
            prepare_smf_transfer();
            break;
        }
        case CMD_SMF_READ:
        {
            fprintf(PC, "[CMD] SMF_READ (0x91)\r\n");
            break;
        }
        case CMD_SMF_ERASE:
        {
            fprintf(PC, "[CMD] SMF_ERASE (0x92)\r\n");
            break;
        }
        case CMD_SMF_COPY_FORCE:
        {
            fprintf(PC, "[CMD] SMF_COPY_FORCE (0x93)\r\n");
            break;
        }
        case CMD_SMF_READ_FORCE:
        {
            fprintf(PC, "[CMD] SMF_READ_FORCE (0x94)\r\n");
            break;
        }
        case CMD_SMF_ERASE_FORCE:
        {
            fprintf(PC, "[CMD] SMF_ERASE_FORCE (0x95)\r\n");
            break;
        }

        // ----------------------------------------------------
        // その他・システム制御コマンド
        // ----------------------------------------------------
        case CMD_RETURN_TIME:
        {
            fprintf(PC, "[CMD] RETURN_TIME (0xB0)\r\n");
            break;
        }
        case REQ_POWER_OFF:
        {
            fprintf(PC, "[CMD] POWER_OFF (0x30)\r\n");
            status = IDLE;
            break;
        }
        default:
        {
            fprintf(PC, "Command received but not implemented yet: 0x%02X\r\n", cmd_id);
            break;
        }
    }

    return true;
}