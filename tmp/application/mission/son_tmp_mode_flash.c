#include "son_tmp_mode_flash.h"
#include "son_tmp_excute_mission.h"
#include "../../core/logging/son_tmp_piclog.h"
#include "../../core/storage/son_tmp_flash.h"
#include "../../core/storage/son_tmp_smf.h"
#include "../../../lib/tool/mmj_smf_memorymap.h"
#include "../../../lib/device/mt25q.h"

extern Flash mis_fm;
extern Flash smf;

extern void check_boss_status_polling(void);
extern void delay_ms_with_polling(uint16_t ms);
extern bool is_mission_aborted;

// ============================================================================
// SMF（CPLD経由）転送シーケンス
// ============================================================================

void prepare_smf_transfer(void)
{
    fprintf(PC, "--- SMF Transfer Prepare ---\r\n");
    piclog_make(0x20, 0x00);
    cigs_smf_prepare();
}

void execute_smf_transfer(void)
{
    fprintf(PC, "--- SMF Transfer Execute ---\r\n");
    piclog_make(0x21, 0x00);
    cigs_smf_copy();
}

void permit_smf_transfer(void)
{
    fprintf(PC, "--- SMF Transfer Permit ---\r\n");
    piclog_make(0x22, 0x00);
    cigs_smf_permit();
    fprintf(PC, "--- SMF Transfer Sequence Complete ---\r\n");
}

// ============================================================================
// Flash保存データ全読み出し (デバッグ用)
// ============================================================================
void execute_flash_dump(void)
{
    unsigned int8 read_buffer[PACKET_SIZE];
    unsigned int32 flash_addr;
    unsigned int16 p_num;
    unsigned int16 i;
    unsigned int32 r_len;

    int8 *p_dst;
    p_dst = (int8 *)read_buffer;
    r_len = (unsigned int32)PACKET_SIZE;

    fprintf(PC, "\r\n--- Flash Data Dump Start ---\r\n");

    is_mission_aborted = false;

    for (p_num = 1; p_num <= 32; p_num++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Flash Dump Aborted by Boss.\r\n");
            break;
        }

        flash_addr = (unsigned int32)(p_num - 1);
        flash_addr *= r_len;

        read_data_bytes(mis_fm, flash_addr, p_dst, r_len);

        unsigned int8 a3 = (unsigned int8)((flash_addr >> 24) & 0xFF);
        unsigned int8 a2 = (unsigned int8)((flash_addr >> 16) & 0xFF);
        unsigned int8 a1 = (unsigned int8)((flash_addr >> 8)  & 0xFF);
        unsigned int8 a0 = (unsigned int8)(flash_addr         & 0xFF);

        unsigned int8 p_num_8 = (unsigned int8)p_num;

        fprintf(PC, "Packet [%u] ", p_num_8);
        fprintf(PC, "Addr: 0x%02X%02X%02X%02X | ", a3, a2, a1, a0);

        for (i = 0; i < PACKET_SIZE; i++)
        {
            fprintf(PC, "%02X ", read_buffer[i]);

            if (((i + 1) % 16 == 0) && (i < (PACKET_SIZE - 1)))
            {
                fprintf(PC, "\r\n                         | ");
            }
        }
        fprintf(PC, "\r\n------------------------------------------------------------\r\n");

        delay_ms_with_polling(10);
    }

    fprintf(PC, "--- Flash Data Dump Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: アドレス情報の出力 (0x87)
// ----------------------------------------------------
void execute_picf_read_address(void)
{
    fprintf(PC, "\r\n[PICF] Reading Address Info...\r\n");
    print_flash_status();
}

// ----------------------------------------------------
// PICF: アドレス情報のリセット (0x8F)
// ----------------------------------------------------
void execute_picf_reset_address(void)
{
    fprintf(PC, "\r\n[PICF] Resetting Address Info...\r\n");

    piclog_data.used_counter = 0;
    piclog_data.uncopied_counter = 0;
    str_data.used_counter = 0;
    str_data.uncopied_counter = 0;

    write_misf_address_area();

    fprintf(PC, "[PICF] Address Reset Complete.\r\n");
}

// ----------------------------------------------------
// PICF: 指定アドレスからの読み出し (0x86)
// ----------------------------------------------------
void execute_picf_read(uint32_t start_address, uint16_t packet_num)
{
    uint8_t read_buffer[PACKET_SIZE];
    uint32_t current_addr = start_address;

    fprintf(PC, "\r\n--- PICF Read Data Start ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX, Packets: %lu\r\n", start_address, (uint32_t)packet_num);

    is_mission_aborted = false;

    for (uint16_t p = 0; p < packet_num; p++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Read Aborted by Boss.\r\n");
            break;
        }

        read_data_bytes(mis_fm, current_addr, (int8*)read_buffer, PACKET_SIZE);

        fprintf(PC, "Packet [%lu] Addr: 0x%08LX | ", (uint32_t)(p+1), current_addr);
        for (uint8_t i = 0; i < PACKET_SIZE; i++)
        {
            fprintf(PC, "%02X ", read_buffer[i]);
        }
        fprintf(PC, "\r\n");

        current_addr += PACKET_SIZE;
        delay_ms_with_polling(5);
    }
    fprintf(PC, "--- PICF Read Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: 4KBサブセクター消去 (0x82)
// ----------------------------------------------------
void execute_picf_erase_4k(uint32_t start_address, uint8_t sector_num)
{
    fprintf(PC, "\r\n--- PICF Erase 4K Subsector ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX, Sectors: %u\r\n", start_address, sector_num);

    uint32_t current_addr = start_address;
    is_mission_aborted = false;

    for (uint8_t i = 0; i < sector_num; i++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Erase Aborted by Boss.\r\n");
            break;
        }

        subsector_4kByte_erase(mis_fm, current_addr);
        fprintf(PC, "Erased Addr: 0x%08LX\r\n", current_addr);
        current_addr += 4096;

        check_boss_status_polling();
    }
    fprintf(PC, "--- Erase Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: 1セクタ(64KB)消去 (0x81)
// ----------------------------------------------------
void execute_picf_erase_1sector(uint32_t start_address, uint8_t sector_num)
{
    fprintf(PC, "\r\n--- PICF Erase 1 Sector ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX, Sectors: %u\r\n", start_address, sector_num);

    uint32_t current_addr = start_address;
    is_mission_aborted = false;

    for (uint8_t i = 0; i < sector_num; i++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Erase Aborted by Boss.\r\n");
            break;
        }

        sector_erase(mis_fm, current_addr);
        fprintf(PC, "Erased Addr: 0x%08LX\r\n", current_addr);
        current_addr += 65536;

        check_boss_status_polling();
    }
    fprintf(PC, "--- Erase Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: 32KBセクター消去 (0x83)
// ----------------------------------------------------
void execute_picf_erase_32k(uint32_t start_address, uint8_t sector_num)
{
    fprintf(PC, "\r\n--- PICF Erase 32K Sector ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX, Sectors: %u\r\n", start_address, sector_num);

    uint32_t current_addr = start_address;
    is_mission_aborted = false;

    for (uint8_t i = 0; i < sector_num; i++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Erase Aborted by Boss.\r\n");
            break;
        }

        subsector_32kByte_erase(mis_fm, current_addr);
        fprintf(PC, "Erased Addr: 0x%08LX\r\n", current_addr);
        current_addr += 32768;

        check_boss_status_polling();
    }
    fprintf(PC, "--- Erase Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: デモ用データの書き込み (0x84)
// ----------------------------------------------------
void execute_picf_write_demo(uint32_t start_address, uint16_t packet_num)
{
    uint8_t write_buffer[PACKET_SIZE];
    uint32_t current_addr = start_address;

    fprintf(PC, "\r\n--- PICF Write Demo Data Start ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX, Packets: %lu\r\n", start_address, (uint32_t)packet_num);

    memset(write_buffer, 0xAA, PACKET_SIZE);
    is_mission_aborted = false;

    for (uint16_t p = 0; p < packet_num; p++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Write Aborted by Boss.\r\n");
            break;
        }

        write_buffer[0] = (uint8_t)(p & 0xFF);
        write_data_bytes(mis_fm, current_addr, (int8*)write_buffer, PACKET_SIZE);

        fprintf(PC, "Wrote Demo Packet [%lu] to Addr: 0x%08LX\r\n", (uint32_t)(p+1), current_addr);
        current_addr += PACKET_SIZE;

        delay_ms_with_polling(5);
    }
    fprintf(PC, "--- PICF Write Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: チップ全消去 (0x80)
// ----------------------------------------------------
void execute_picf_erase_all(void)
{
    fprintf(PC, "\r\n--- PICF Erase All (Full Chip 16MB) ---\r\n");

    uint32_t current_addr = 0x00000000;
    is_mission_aborted = false;

    for (uint16_t i = 0; i < 256; i++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Erase All Aborted by Boss.\r\n");
            break;
        }

        sector_erase(mis_fm, current_addr);

        if (i % 16 == 0) {
            fprintf(PC, "Erasing... %lu / 256 Sectors\r\n", i);
        }
        current_addr += 65536;

        check_boss_status_polling();
    }
    fprintf(PC, "--- Erase All Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: 4KBサブセクターへの書き込み (0x85)
// ----------------------------------------------------
void execute_picf_write_4k(uint32_t start_address)
{
    uint8_t write_buffer[PACKET_SIZE];
    uint32_t current_addr = start_address;

    fprintf(PC, "\r\n--- PICF Write 4K Subsector ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX\r\n", start_address);

    memset(write_buffer, 0x55, PACKET_SIZE);
    is_mission_aborted = false;

    for (uint16_t p = 0; p < 64; p++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Write Aborted by Boss.\r\n");
            break;
        }

        write_buffer[0] = (uint8_t)(p & 0xFF);
        write_data_bytes(mis_fm, current_addr, (int8*)write_buffer, PACKET_SIZE);
        current_addr += PACKET_SIZE;

        delay_us(500);
        check_boss_status_polling();
    }
    fprintf(PC, "--- Write 4K Complete ---\r\n");
}

// ----------------------------------------------------
// PICF: 全消去とアドレスリセット (0x88)
// ----------------------------------------------------
void execute_picf_erase_and_reset(void)
{
    fprintf(PC, "\r\n[PICF] Erase And Reset Sequence Start\r\n");

    execute_picf_erase_all();

    if (!is_mission_aborted) {
        execute_picf_reset_address();
        fprintf(PC, "[PICF] Erase And Reset Sequence Complete\r\n");
    }
}

// ----------------------------------------------------
// PICF: エリア指定での読み出し (0x89)
// ----------------------------------------------------
// ★修正: 引数を uint16_t に拡張しました (最大 65,535パケット指定可能)
void execute_picf_read_area(uint8_t area, uint16_t start_packet, uint16_t request_packet)
{
    fprintf(PC, "\r\n--- PICF Read Area ---\r\n");
    fprintf(PC, "Area ID: 0x%02X, Start Packet: %lu, Request Packets: %lu\r\n", area, (uint32_t)start_packet, (uint32_t)request_packet);

    MisfStatusStruct area_info = get_misf_status_struct(area);

    if (area_info.start_address == 0 && area_info.end_address == 0)
    {
        fprintf(PC, "[ERROR] Invalid Area ID.\r\n");
        return;
    }

    uint32_t start_address = area_info.start_address + ((uint32_t)start_packet * PACKET_SIZE);
    execute_picf_read(start_address, request_packet);
}

// ----------------------------------------------------
// (内部用) PICFのアドレスからSMFの書き込みアドレスを算出する
// ----------------------------------------------------
uint32_t calculate_smf_write_address(uint32_t picf_address)
{
    const uint32_t SMF_TMP_STR_DATA_START = 0x04DA1000;
    const uint32_t SMF_TMP_PICLOG_START   = 0x04DC2000;

    if (picf_address >= MISF_TMP_STR_DATA_START && picf_address <= MISF_TMP_STR_DATA_END)
    {
        uint32_t offset = picf_address - MISF_TMP_STR_DATA_START;
        return SMF_TMP_STR_DATA_START + offset;
    }
    else if (picf_address >= MISF_TMP_PICLOG_START && picf_address <= MISF_TMP_PICLOG_END)
    {
        uint32_t offset = picf_address - MISF_TMP_PICLOG_START;
        return SMF_TMP_PICLOG_START + offset;
    }

    return picf_address;
}

// ----------------------------------------------------
// SMF: ダイレクトコピー (PICF -> SMF) (0x90, 0x93)
// ----------------------------------------------------
void execute_smf_direct_copy(uint32_t address, uint16_t packet_num, bool is_force)
{
    uint8_t buffer[PACKET_SIZE];
    uint32_t read_addr = address;
    uint32_t write_addr = calculate_smf_write_address(address);

    fprintf(PC, "\r\n--- SMF Direct Copy (PICF -> SMF) ---\r\n");
    fprintf(PC, "Read Addr : 0x%08LX\r\n", read_addr);
    fprintf(PC, "Write Addr: 0x%08LX\r\n", write_addr);
    fprintf(PC, "Packets   : %lu, Force: %u\r\n", (uint32_t)packet_num, is_force);

    is_mission_aborted = false;

    for (uint16_t p = 0; p < packet_num; p++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] Copy Aborted by Boss.\r\n");
            break;
        }

        read_data_bytes(mis_fm, read_addr, (int8*)buffer, PACKET_SIZE);
        write_data_bytes(smf, write_addr, (int8*)buffer, PACKET_SIZE);

        fprintf(PC, "Copied Packet [%lu] 0x%08LX -> 0x%08LX\r\n", (uint32_t)(p+1), read_addr, write_addr);

        read_addr += PACKET_SIZE;
        write_addr += PACKET_SIZE;

        delay_ms_with_polling(5);
    }
    fprintf(PC, "--- SMF Copy Complete ---\r\n");
}

// ----------------------------------------------------
// SMF: 指定アドレスからの読み出し (0x91, 0x94)
// ----------------------------------------------------
void execute_smf_read(uint32_t address, uint16_t packet_num, bool is_force)
{
    uint8_t read_buffer[PACKET_SIZE];
    uint32_t current_addr = address;

    fprintf(PC, "\r\n--- SMF Read Data Start ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX, Packets: %lu, Force: %u\r\n", address, (uint32_t)packet_num, is_force);

    is_mission_aborted = false;

    for (uint16_t p = 0; p < packet_num; p++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] SMF Read Aborted by Boss.\r\n");
            break;
        }

        read_data_bytes(smf, current_addr, (int8*)read_buffer, PACKET_SIZE);

        fprintf(PC, "Packet [%lu] Addr: 0x%08LX | ", (uint32_t)(p+1), current_addr);
        for (uint8_t i = 0; i < PACKET_SIZE; i++)
        {
            fprintf(PC, "%02X ", read_buffer[i]);
        }
        fprintf(PC, "\r\n");

        current_addr += PACKET_SIZE;
        delay_ms_with_polling(5);
    }
    fprintf(PC, "--- SMF Read Complete ---\r\n");
}

// ----------------------------------------------------
// SMF: 64KBセクター消去 (0x92, 0x95)
// ----------------------------------------------------
void execute_smf_erase(uint32_t address, uint8_t sector_num, bool is_force)
{
    fprintf(PC, "\r\n--- SMF Erase 64K Sector ---\r\n");
    fprintf(PC, "Start Addr: 0x%08LX, Sectors: %u, Force: %u\r\n", address, sector_num, is_force);

    uint32_t current_addr = address;
    is_mission_aborted = false;

    for (uint8_t i = 0; i < sector_num; i++)
    {
        if (is_mission_aborted) {
            fprintf(PC, "\r\n[WARN] SMF Erase Aborted by Boss.\r\n");
            break;
        }

        sector_erase(smf, current_addr);
        fprintf(PC, "Erased Addr: 0x%08LX\r\n", current_addr);
        current_addr += 65536;

        check_boss_status_polling();
    }
    fprintf(PC, "--- Erase Complete ---\r\n");
}

// ----------------------------------------------------
// SMF: 強制一括消去 (0x95)
// ----------------------------------------------------
void execute_smf_erase_force(void)
{
    fprintf(PC, "\r\n--- SMF Erase Force (Fixed Area) ---\r\n");
    fprintf(PC, "\r\n--- This Command is in progress. ---\r\n");
}