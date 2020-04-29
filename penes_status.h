/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __PENES_STATUS_H__
#define __PENES_STATUS_H__

/** Enums *****************************************************************/
/** @brief Return statuses for the modules within the project Faraway. */
enum PeNESStatus {
    PENES_STATUS_UNINITIALIZED = -1,
    PENES_STATUS_SUCCESS = 0,

    /* Error statuses for the module utils. */
    PENES_STATUS_UTILS_OBJECT_TABLE_GET_TYPE_OUT_OF_BOUNDS,
    PENES_STATUS_UTILS_OBJECT_TABLE_GET_OBJECT_OUT_OF_BOUNDS,
    PENES_STATUS_UTILS_OBJECT_TABLE_GET_OBJECT_BY_TYPE_NOT_FOUND,

    /* Error statuses for the module decoder. */
    PENES_STATUS_DECODER_DECODE_OPCODE_ADDRESS_OUT_OF_BOUNDS,
    PENES_STATUS_DECODER_DECODE_OPCODE_GROUP_OUT_OF_BOUNDS,
    PENES_STATUS_DECODER_DECODE_OPERAND_ADDRESS_OUT_OF_BOUNDS,

    /* Error statuses for the module memory_map. */
    PENES_STATUS_MEMORY_MAP_GET_MEMORY_STORAGE_NOT_FOUND,
    PENES_STATUS_MEMORY_MAP_GET_MEMORY_STORAGE_OUT_OF_BOUNDS,

    /* Error statuses for the module storage_location. */
    PENES_STATUS_STORAGE_LOCATION_READ_OUT_OF_BOUNDS,
    PENES_STATUS_STORAGE_LOCATION_WRITE_OUT_OF_BOUNDS,
    PENES_STATUS_STORAGE_LOCATION_IMMEDIATE_STORAGE_WRITE_INVALID_OPERATION,

    /* Error statuses for the module rom_loader. */
    PENES_STATUS_ROM_LOADER_OPEN_OPEN_FAILED,
    PENES_STATUS_ROM_LOADER_OPEN_READ_MAGIC_FAILED,
    PENES_STATUS_ROM_LOADER_OPEN_INVALID_FILE,
    PENES_STATUS_ROM_LOADER_OPEN_GET_PRG_ROM_BANKS_LSB_FAILED,
    PENES_STATUS_ROM_LOADER_OPEN_GET_CHR_ROM_BANKS_LSB_FAILED,
    PENES_STATUS_ROM_LOADER_OPEN_SEEK_BANK_COUNT_FAILED,
    PENES_STATUS_ROM_LOADER_OPEN_READ_ROM_BANK_MSB_FAILED,
    PENES_STATUS_LOADER_SEEK_PRG_ROM_FAILED,
    PENES_STATUS_LOADER_READ_PRG_ROM_FAILED,
    PENES_STATUS_LOADER_GET_PRG_ROM_BANK_OUT_OF_BOUNDS,
    PENES_STATUS_LOADER_GET_PRG_ROM_BANK_SEEKG_FAILED,
    PENES_STATUS_LOADER_GET_PRG_ROM_BANK_READ_FAILED,

    /* Error statuses for the module address_mode_interface. */
    PENES_STATUS_IMPLIED_ADDRESS_MODE_GET_STORAGE_INVALID_OPERATION,

    /* Error statuses for the module instruction_set. */
    PENES_STATUS_INSTRUCTION_SET_OPCODE_TABLE_GET_OPCODE_OUT_OF_BOUNDS
};

/** Macros ****************************************************************/
/* Update a "real" status var with an internal status var,
 * only if it has not yet been updated by a previous error.
 * */
#define STATUS_LEGAL_UPDATE(status_var, update_status)            \
    do {                                                          \
        if ((PENES_STATUS_UNINITIALIZED == (status_var)) ||     \
            (PENES_STATUS_SUCCESS == (status_var))) {           \
            (status_var) = (update_status);                       \
        }                                                         \
    } while (0)

/* Convert an internal status variable into a standard exit value. */
#define EXIT_STATUS(status_var) ((PENES_STATUS_SUCCESS == (status_var))? EXIT_SUCCESS : EXIT_FAILURE)

#endif /*__PENES_STATUS_H__*/
