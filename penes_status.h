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
    PENES_STATUS_STORAGE_LOCATION_HEAPALLOCZ_FAILED,
    PENES_STATUS_STORAGE_LOCATION_READ_OUT_OF_BOUNDS,
    PENES_STATUS_STORAGE_LOCATION_WRITE_OUT_OF_BOUNDS,
    PENES_STATUS_STORAGE_LOCATION_IMMEDIATE_STORAGE_WRITE_INVALID_OPERATION,

    /* Error statuses for the module address_mode. */
    PENES_STATUS_ADDRESS_MODE_TABLE_GET_ADDRESS_MODE_NO_MODE,

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
