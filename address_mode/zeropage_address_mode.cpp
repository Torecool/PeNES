/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <cstdint>

#include "penes_status.h"
#include "common.h"

#include "address_mode/address_mode_interface.h"

#include "address_mode/zeropage_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_modes;

/** Functions *************************************************************/
inline enum PeNESStatus ZeropageAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t zeropage_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    size_t num_bytes_read = sizeof(address_data);

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Retrieve byte at absolute zeropage_address. */
    status = program_context->memory_manager.data_at_address(
        zeropage_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_ZEROPAGE_ADDRESS_MODE_GET_DATA_END_OF_MEMORY;
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


inline enum PeNESStatus ZeropageXIndexedAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t zeropage_indexed_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    native_word_t register_x = 0;
    size_t num_bytes_read = sizeof(address_data);
    native_address_t zeropage_address = 0;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Add index offset from register X.
     * Note: We don't want carry behavior. If the summation overflows the address should wrap around.
     * */
    register_x = program_context->register_file.register_x;
    zeropage_address = static_cast<native_address_t>(static_cast<native_word_t>(zeropage_indexed_address) + register_x);

    /* Retrieve byte at absolute absolute_address. */
    status = program_context->memory_manager.data_at_address(
        zeropage_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_ZEROPAGE_X_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY;
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


enum PeNESStatus ZeropageYIndexedAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t zeropage_indexed_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    native_word_t register_y = 0;
    size_t num_bytes_read = sizeof(address_data);
    native_address_t zeropage_address = 0;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Add index offset from register X.
     * Note: We don't want carry behavior. If the summation overflows the address should wrap around.
     * */
    register_y = program_context->register_file.register_y;
    zeropage_address = static_cast<native_address_t>(static_cast<native_word_t>(zeropage_indexed_address) + register_y);

    /* Retrieve byte at absolute absolute_address. */
    status = program_context->memory_manager.data_at_address(
        zeropage_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_ZEROPAGE_Y_INDEXED_ADDRESS_MODE_GET_DATA_END_OF_MEMORY;
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