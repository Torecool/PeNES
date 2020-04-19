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

#include "address_mode/indirect_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_modes;

/** Functions *************************************************************/
inline enum PeNESStatus IndirectAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t indirect_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    native_address_t direct_address = 0;
    size_t num_bytes_read = 0;
    native_address_t hardware_indirect_address = bswap_16(indirect_address);

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Retrieve direct address using indirect address. */
    num_bytes_read = sizeof(direct_address);
    status = program_context->memory_manager.data_at_address(
        hardware_indirect_address,
        &direct_address,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(direct_address) != num_bytes_read) {
        status = PENES_STATUS_INDIRECT_ADDRESS_MODE_GET_DATA_INDIRECT_END_OF_MEMORY;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(,
            "Not enough memory to retrieve full data size. Status: %d. Retrieved: %zu\n",
            status,
            num_bytes_read
        );
        goto l_cleanup;
    }

    /* Retrieve data at direct address. */
    num_bytes_read = sizeof(address_data);
    status = program_context->memory_manager.data_at_address(
        direct_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for direct. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_INDIRECT_ADDRESS_MODE_GET_DATA_DIRECT_END_OF_MEMORY;
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


inline enum PeNESStatus XIndexedIndirectAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t indexed_indirect_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    native_address_t direct_address = 0;
    size_t num_bytes_read = 0;
    native_word_t register_x = 0;
    native_dword_t indirect_address = 0;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    /* Add index offset from register X.
     * Note: We don't want carry behavior. If the summation overflows the address should wrap around.
     * */
    register_x = program_context->register_file.register_x;
    indirect_address = static_cast<native_address_t>(static_cast<native_word_t>(indexed_indirect_address) + register_x);

    /* Retrieve direct address using indirect address. */
    num_bytes_read = sizeof(direct_address);
    status = program_context->memory_manager.data_at_address(
        indirect_address,
        &direct_address,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(direct_address) != num_bytes_read) {
        status = PENES_STATUS_X_INDEXED_INDIRECT_ADDRESS_MODE_GET_DATA_INDIRECT_END_OF_MEMORY;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(,
            "Not enough memory to retrieve full data size. Status: %d. Retrieved: %zu\n",
            status,
            num_bytes_read
        );
        goto l_cleanup;
    }

    /* Retrieve data at direct address. */
    num_bytes_read = sizeof(address_data);
    status = program_context->memory_manager.data_at_address(
        direct_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for direct. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_X_INDEXED_INDIRECT_ADDRESS_MODE_GET_DATA_DIRECT_END_OF_MEMORY;
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


inline enum PeNESStatus IndirectYIndexedAddressMode::get_data(
    const ProgramContext *program_context,
    native_dword_t indirect_indexed_address,
    native_word_t *output_data
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t address_data = 0;
    native_address_t indexed_direct_address = 0;
    native_address_t direct_address = 0;
    size_t num_bytes_read = 0;
    native_word_t register_y = 0;
    native_address_t indirect_address = indirect_indexed_address;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_data);

    register_y = program_context->register_file.register_y;

    /* Retrieve direct address using indirect address. */
    num_bytes_read = sizeof(indexed_direct_address);
    status = program_context->memory_manager.data_at_address(
        indirect_address,
        &indexed_direct_address,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(indexed_direct_address) != num_bytes_read) {
        status = PENES_STATUS_INDIRECT_Y_INDEXED_ADDRESS_MODE_GET_DATA_INDIRECT_END_OF_MEMORY;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(,
            "Not enough memory to retrieve full data size. Status: %d. Retrieved: %zu\n",
            status,
            num_bytes_read
        );
        goto l_cleanup;
    }

    /* Add index offset from register X.
     * Note: Here, we do want carry behavior.
     * Note: Addresses are saved in little endianness,
     * so we need to swap it back to machine endianness, add the index, and swap it back.
     * */
    direct_address = bswap_16(bswap_16(indexed_direct_address) + register_y);

    /* Retrieve data at direct address. */
    num_bytes_read = sizeof(address_data);
    status = program_context->memory_manager.data_at_address(
        direct_address,
        &address_data,
        &num_bytes_read
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for direct. Status: %d\n", status);
        goto l_cleanup;
    } else if (sizeof(address_data) != num_bytes_read) {
        status = PENES_STATUS_INDIRECT_Y_INDEXED_ADDRESS_MODE_GET_DATA_DIRECT_END_OF_MEMORY;
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