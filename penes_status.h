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

    /* Error statuses for the module address_mode. */
    PENES_STATUS_ADDRESS_MODE_TABLE_GET_ADDRESS_MODE_NO_MODE,

    /* Error statuses for the module absolute_address_mode. */
    PENES_STATUS_ABSOLUTE_ADDRESS_MODE_GET_DATA_END_OF_MEMORY,
    PENES_STATUS_ABSOLUTE_X_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY,
    PENES_STATUS_ABSOLUTE_Y_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY,

    /* Error statuses for the module indirect_address_mode. */
    PENES_STATUS_INDIRECT_ADDRESS_MODE_GET_DATA_INDIRECT_END_OF_MEMORY,
    PENES_STATUS_INDIRECT_ADDRESS_MODE_GET_DATA_DIRECT_END_OF_MEMORY,
    PENES_STATUS_X_INDEXED_INDIRECT_ADDRESS_MODE_GET_DATA_INDIRECT_END_OF_MEMORY,
    PENES_STATUS_X_INDEXED_INDIRECT_ADDRESS_MODE_GET_DATA_DIRECT_END_OF_MEMORY,
    PENES_STATUS_INDIRECT_Y_INDEXED_ADDRESS_MODE_GET_DATA_INDIRECT_END_OF_MEMORY,
    PENES_STATUS_INDIRECT_Y_INDEXED_ADDRESS_MODE_GET_DATA_DIRECT_END_OF_MEMORY,

    PENES_STATUS_ZEROPAGE_ADDRESS_MODE_GET_DATA_END_OF_MEMORY,
    PENES_STATUS_ZEROPAGE_X_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY,
    PENES_STATUS_ZEROPAGE_Y_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY,
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
