/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <byteswap.h>

#include "penes_status.h"
#include "common.h"

#include "memory_manager/memory_manager.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/absolute_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_modes;

/** Functions *************************************************************/
inline enum PeNESStatus AbsoluteAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t absolute_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    size_t num_bytes_read = sizeof(address_data);
    native_address_t hardware_address = bswap_16(absolute_address);

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Retrieve byte at absolute absolute_address. */
    status = program_context->memory_manager.data_at_address(
        hardware_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_ABSOLUTE_ADDRESS_MODE_GET_DATA_END_OF_MEMORY;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(,
            "Not enough memory to retrieve full data size. Status: %d. Retrieved: %zu\n",
            status,
            num_bytes_read
        );
        goto l_cleanup;
    }

    *output_data = address_data;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


inline enum PeNESStatus AbsoluteXIndexedAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t absolute_indexed_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    native_word_t register_x = 0;
    size_t num_bytes_read = sizeof(address_data);
    native_address_t hardware_address = bswap_16(absolute_indexed_address);

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Add index offset from register X. */
    register_x = program_context->register_file.register_x;
    hardware_address += register_x;

    /* Retrieve byte at absolute absolute_address. */
    status = program_context->memory_manager.data_at_address(
        hardware_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_ABSOLUTE_X_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(,
            "Not enough memory to retrieve full data size. Status: %d. Retrieved: %zu\n",
            status,
            num_bytes_read
        );
        goto l_cleanup;
    }

    *output_data = address_data;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


inline enum PeNESStatus AbsoluteYIndexedAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t absolute_indexed_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    native_word_t register_y = 0;
    size_t num_bytes_read = sizeof(address_data);
    native_address_t hardware_address = bswap_16(absolute_indexed_address);

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Add index offset from register Y. */
    register_y = program_context->register_file.register_y;
    hardware_address += register_y;

    /* Retrieve byte at absolute absolute_address. */
    status = program_context->memory_manager.data_at_address(
        hardware_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_ABSOLUTE_Y_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(,
            "Not enough memory to retrieve full data size. Status: %d. Retrieved: %zu\n",
            status,
            num_bytes_read
        );
        goto l_cleanup;
    }

    *output_data = address_data;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
