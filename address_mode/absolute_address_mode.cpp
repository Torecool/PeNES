/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>
#include <byteswap.h>

#include "penes_status.h"
#include "common.h"

#include "memory_manager/memory_manager.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/absolute_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_mode;

/** Functions *************************************************************/
inline enum PeNESStatus AbsoluteAddressMode::get_storage(
    const ProgramContext *program_ctx,
    native_dword_t absolute_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    std::size_t memory_offset = 0;
    native_address_t hardware_address = bswap_16(absolute_address);

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Retrieve storage at absolute address. */
    status = program_ctx->memory_manager.get_memory_storage(
        hardware_address,
        &data_storage,
        &memory_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = memory_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


inline enum PeNESStatus AbsoluteXIndexedAddressMode::get_storage(
    const ProgramContext *program_ctx,
    native_dword_t absolute_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    RegisterStorage *register_x = nullptr;
    std::size_t memory_offset = 0;
    native_word_t register_index = 0;
    native_address_t hardware_address = bswap_16(absolute_address);

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Add index offset from register X. */
    register_x = program_ctx->register_file.get_register_x();
    status = register_x->read(&register_index, sizeof(register_index));
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read failed. Status: %d\n", status);
        goto l_cleanup;
    }

    hardware_address += register_index;

    /* Retrieve storage at absolute indexed address. */
    status = program_ctx->memory_manager.get_memory_storage(
        hardware_address,
        &data_storage,
        &memory_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = memory_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


inline enum PeNESStatus AbsoluteYIndexedAddressMode::get_storage(
    const ProgramContext *program_ctx,
    native_dword_t absolute_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    RegisterStorage *register_y = nullptr;
    native_word_t register_data = 0;
    std::size_t memory_offset = 0;
    native_address_t hardware_address = bswap_16(absolute_address);

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Add index offset from register Y. */
    register_y = program_ctx->register_file.get_register_y();
    status = register_y->read(&register_data, sizeof(register_data));
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read failed. Status: %d\n", status);
        goto l_cleanup;
    }

    hardware_address += register_data;

    /* Retrieve storage at absolute indexed address. */
    status = program_ctx->memory_manager.get_memory_storage(
        hardware_address,
        &data_storage,
        &memory_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = memory_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
