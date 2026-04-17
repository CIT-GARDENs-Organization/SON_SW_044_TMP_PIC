#ifndef MT25Q_H
#define MT25Q_H

// =========
//#define MT25Q_DEBUG
// =========


#define FLASH_STREAM0 MIS_FM_STREAM     // <- Align the names to `config.h` Stream name
#define FLASH_STREAM1 SMF_STREAM
#define FLASH_STREAM2 SMF_STREAM // dont use
#define FLASH_STREAM3 SMF_STREAM // dont use

typedef enum spi_stream{
    SPI_0,
    SPI_1,
    SPI_2,
    SPI_3,
}SpiStreamId;

#define MT25QL128ABA 0x00     //Mission Flash (16MB)
#define MT25QL01GBBB 0x01     //SMF,CF (128MB)
typedef struct select_stream_to_flash{
    SpiStreamId spi_stream_id;
    unsigned int8 flash_model;
    int16 cs_pin;
}Flash;


Flash mis_fm = {SPI_0, MT25QL128ABA, MIS_FM_CS};
Flash smf = {SPI_1, MT25QL01GBBB, SMF_CS};



// ===================== Function List =====================
// Public Functions

int1 is_connect(Flash flash_stream);

void write_data_bytes(Flash flash_stream, unsigned int32 write_start_address, int8 *write_data, unsigned int16 write_amount);

void read_data_bytes(Flash flash_stream, unsigned int32 read_start_address, int8 *read_data, unsigned int32 read_amount);

void sector_erase(Flash flash_stream, unsigned int32 sector_address);

void subsector_32kByte_erase(Flash flash_stream, unsigned int32 subsector_address);

void subsector_4kByte_erase(Flash flash_stream, unsigned int32 subsector_address);

//Private Functions

void flash_setting(Flash flash_stream);

int8 status_register(Flash flash_stream);

int8 read_id(Flash flash_stream);

int8 read_data_byte(Flash flash_stream, unsigned int32 read_address);

void write_data_byte(Flash flash_stream, unsigned int32 write_address,int8 write_data);
        //


// ===================== Flash Commands =====================
#define CMD_READ_ID                     0x9F
#define CMD_READ_STATUS_REGISTER        0x05
#define CMD_READ                        0x03//for MT25QL128ABA
#define CMD_4BYTE_READ                  0x13//for MT25QL01GBBB
#define CMD_WRITE_ENABLE                0x06
#define CMD_PAGE_PROGRAM                0x02//for MT25QL128ABA
#define CMD_4BYTE_PAGE_PROGRAM          0x12//for MT25QL01GBBB
#define CMD_SUBSECTOR_4KB_ERASE         0x20//for MT25QL128ABA
#define CMD_4BYTE_SUBSECTOR_4KB_ERASE   0x21//for MT25QL01GBBB
#define CMD_SUBSECTOR_32KB_ERASE        0x52//for MT25QL128ABA
#define CMD_4BYTE_SUBSECTOR_32KB_ERASE  0x5C//for MT25QL01GBBB
#define CMD_SECTOR_ERASE                0xD8//for MT25QL128ABA
#define CMD_4BYTE_SECTOR_ERASE          0xDC//for MT25QL01GBBB

// ====================== Value List =====================
#define MANUFACTURER_ID_MICRON 0x20 // Manufacturer ID for Micron flash
#define CAPACITY_ID_16MB       0x18 // Capacity ID for MT25QL128ABA (128Mbit = 16MB)
#define CAPACITY_ID_128MB      0x21 // Capacity ID for MT25QL01GBBB (1Gbit = 128MB)
#define READ_ID_DATASIZE 20 // 20 bytes for read ID data


// ====================== Data Structures =====================
typedef union
{
    unsigned int8 bytes[READ_ID_DATASIZE]; // 20 bytes for read ID
    struct {
        unsigned int8 manufacturer_id; // 1 byte
        unsigned int8 memory_type;     // 1 byte
        unsigned int8 capacity;        // 1 byte
        unsigned int8 reserved;       // 1 byte
    } fields;
}READ_ID_DATA;


#endif