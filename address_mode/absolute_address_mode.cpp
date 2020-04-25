/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"
#include "system.h"

#include "program_context/program_context.h"
#include "memory_manager/memory_manager.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/absolute_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_mode;

/** Functions *************************************************************/
enum PeNESStatus AbsoluteAddressMode::get_storage(
    ProgramContext *program_ctx,
    native_dword_t absolute_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    std::size_t memory_offset = 0;
    native_address_t converted_address = system_native_to_big_endianness(absolute_address);

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Retrieve storage at absolute address. */
    status = program_ctx->memory_manager.get_memory_storage(
        converted_address,
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


enum PeNESStatus AbsoluteXIndexedAddressMode::get_storage(
    ProgramContext *program_ctx,
    native_dword_t absolute_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    RegisterStorage<native_word_t> *register_x = nullptr;
    std::size_t memory_offset = 0;
    native_word_t register_index = 0;
    native_address_t hardware_address = system_native_to_big_endianness(absolute_address);

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Add index offset from register X. */
    register_x = program_ctx->register_file.get_register_x();
    register_index = register_x->read();

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


enum PeNESStatus AbsoluteYIndexedAddressMode::get_storage(
    ProgramContext *program_ctx,
    native_dword_t absolute_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    RegisterStorage<native_word_t> *register_y = nullptr;
    native_word_t register_data = 0;
    std::size_t memory_offset = 0;
    native_address_t converted_address = system_native_to_big_endianness(absolute_address);

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Add index offset from register Y. */
    register_y = program_ctx->register_file.get_register_y();
    register_data = register_y->read();

    converted_address += register_data;

    /* Retrieve storage at absolute indexed address. */
    status = program_ctx->memory_manager.get_memory_storage(
        converted_address,
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
