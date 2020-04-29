/**
 * @brief  
 * @author TBK
 * @date   27/04/2020
 * */

/** Headers ***************************************************************/
#include <string>
#include <iostream>
#include <fstream>

#include "penes_status.h"
#include "common.h"

#include "rom_loader/rom_loader.h"

/** Constants *************************************************************/
#define ROM_LOADER_NES_FILE_MAGIC ("NES\x1A")
#define ROM_LOADER_NES_FILE_MAGIC_SIZE (4)
#define ROM_LOADER_NES_FILE_ROM_SIZE_MSB_OFFSET (9)
#define ROM_LOADER_NES_FILE_PRG_ROM_OFFSET (16)
#define ROM_LOADER_NIBBLE_BIT_MASK (0b1111)
#define ROM_LOADER_NIBBLE_SIZE_BITS (4)
#define ROM_LOADER_PRG_ROM_BANK_SIZE (0x4000)

/** Functions *************************************************************/
enum PeNESStatus ROMLoader::open(const std::string& input_file)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    char file_magic_buffer[ROM_LOADER_NES_FILE_MAGIC_SIZE] = {0};
    std::size_t temp_prg_rom_banks = 0;
    std::size_t temp_chr_rom_banks = 0;
    std::uint8_t combined_rom_size_msb = 0;

    /* Close the previous input file in case it was open. */
    this->input_file_stream.close();

    /* Open the source file and check if the operation has succeeded. */
    this->input_file_stream.open(input_file, std::ifstream::in | std::ifstream::binary);
    if (true == this->input_file_stream.fail()) {
        status = PENES_STATUS_ROM_LOADER_OPEN_OPEN_FAILED;
        DEBUG_PRINT_WITH_ARGS("Failed to open the input file. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Read the magic of the file to verify that it is of type iNES. */
    this->input_file_stream.read(file_magic_buffer, sizeof(file_magic_buffer));
    if (true == this->input_file_stream.fail()) {
        status = PENES_STATUS_ROM_LOADER_OPEN_READ_MAGIC_FAILED;
        DEBUG_PRINT_WITH_ARGS("Failed to read file magic. Status: %d.\n", status);
        goto l_cleanup;
    }

    if (CMP_EQUAL != strcmp(ROM_LOADER_NES_FILE_MAGIC, file_magic_buffer)) {
        status = PENES_STATUS_ROM_LOADER_OPEN_INVALID_FILE;
        DEBUG_PRINT_WITH_ARGS("Invalid input file. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Read the LSB of the PRG-ROM and CHR-ROM bank counts, respectively. */
    temp_prg_rom_banks = this->input_file_stream.get();
    if (true == this->input_file_stream.fail()) {
        status = PENES_STATUS_ROM_LOADER_OPEN_GET_PRG_ROM_BANKS_LSB_FAILED;
        DEBUG_PRINT_WITH_ARGS("Failed to read PRG-ROM bank count LSB. Status: %d.\n", status);
        goto l_cleanup;
    }

    temp_chr_rom_banks = this->input_file_stream.get();
    if (true == this->input_file_stream.fail()) {
        status = PENES_STATUS_ROM_LOADER_OPEN_GET_CHR_ROM_BANKS_LSB_FAILED;
        DEBUG_PRINT_WITH_ARGS("Failed to read CHR-ROM bank count LSB. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Seek to the location specifying the combined MSB of the PRG-ROM and the CHR-ROM bank count. */
    this->input_file_stream.seekg(ROM_LOADER_NES_FILE_ROM_SIZE_MSB_OFFSET);
    if (true == input_file_stream.fail()) {
        status = PENES_STATUS_ROM_LOADER_OPEN_SEEK_BANK_COUNT_FAILED;
        DEBUG_PRINT_WITH_ARGS("seekg to the MSB failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Read the combined MSB and parse it into the PRG/CHR nibbles. */
    combined_rom_size_msb = this->input_file_stream.get();
    if (true == this->input_file_stream.fail()) {
        status = PENES_STATUS_ROM_LOADER_OPEN_READ_ROM_BANK_MSB_FAILED;
        DEBUG_PRINT_WITH_ARGS("Failed to read the PRG/CHR bank count MSB. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* TODO Ugh */
    /* Add the respective nibbles to the size totals. */
    temp_prg_rom_banks += (combined_rom_size_msb & (ROM_LOADER_NIBBLE_BIT_MASK << ROM_LOADER_NIBBLE_SIZE_BITS) << ROM_LOADER_NIBBLE_SIZE_BITS);
    temp_chr_rom_banks += (combined_rom_size_msb & (ROM_LOADER_NIBBLE_SIZE_BITS) << (ROM_LOADER_NIBBLE_SIZE_BITS * 2));

    this->num_prg_rom_banks = temp_prg_rom_banks;
    this->num_chr_rom_banks = temp_chr_rom_banks;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus ROMLoader::get_prg_rom_bank(std::size_t bank_index, char *bank_buffer)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != bank_buffer);

    /* Verify the bank index is within bounds of the open file. */
    if (this->num_prg_rom_banks <= bank_index) {
        status = PENES_STATUS_LOADER_GET_PRG_ROM_BANK_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ARGS("PRG-ROM index out of bounds. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Seek to the PRG-ROM location. */
    input_file_stream.seekg(ROM_LOADER_NES_FILE_PRG_ROM_OFFSET + (ROM_LOADER_PRG_ROM_BANK_SIZE * bank_index));
    if (true == input_file_stream.fail()) {
        status = PENES_STATUS_LOADER_GET_PRG_ROM_BANK_SEEKG_FAILED;
        DEBUG_PRINT_WITH_ARGS("seekg failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Read the PRG-ROM bank into a buffer. */
    input_file_stream.read(bank_buffer, ROM_LOADER_PRG_ROM_BANK_SIZE);
    if (true == input_file_stream.fail()) {
        status = PENES_STATUS_LOADER_GET_PRG_ROM_BANK_READ_FAILED;
        DEBUG_PRINT_WITH_ARGS("read failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
