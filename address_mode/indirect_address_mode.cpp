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

#include "memory_manager/memory_manager.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/indirect_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_mode;

/** Functions *************************************************************/
enum PeNESStatus IndirectAddressMode::get_storage(
    ProgramContext *program_ctx,
    native_dword_t indirect_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    MemoryStorage *direct_address_storage = nullptr;
    size_t direct_storage_offset = 0;
    size_t data_storage_offset = 0;
    native_address_t direct_address = 0;
    native_address_t converted_direct_address = 0;
    native_address_t converted_indirect_address = system_native_to_big_endianess(indirect_address);

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Retrieve storage location containing the direct address using the indirect address. */
    status = program_ctx->memory_manager.get_memory_storage(
        converted_indirect_address,
        &direct_address_storage,
        &direct_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Read the direct address from the storage location. */
    status = direct_address_storage->read(
        &direct_address,
        sizeof(direct_address),
        direct_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Convert the direct address, stored in memory in native endianess, into big endianess. */
    converted_direct_address = system_native_to_big_endianess(direct_address);

    /* Retrieve data at absolute direct address. */
    status = program_ctx->memory_manager.get_memory_storage(
        converted_direct_address,
        &data_storage,
        &data_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for direct. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = data_storage_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus XIndexedIndirectAddressMode::get_storage(
    ProgramContext *program_ctx,
    native_dword_t indirect_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;
    MemoryStorage *data_storage = nullptr;
    MemoryStorage *direct_address_storage = nullptr;
    size_t direct_storage_offset = 0;
    size_t data_storage_offset = 0;
    native_word_t register_index = 0;
    native_address_t direct_address = 0;
    native_address_t converted_direct_address = 0;
    native_address_t indexed_indirect_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Add index offset from register X.
     * Note: We don't want carry behavior. If the summation overflows the address should wrap around.
     * */
    register_x = program_ctx->register_file.get_register_x();
    register_index = register_x->read();

    indexed_indirect_address = static_cast<native_address_t>(
        static_cast<native_word_t>(indirect_address) + register_index
    );

    /* Retrieve storage location containing the direct address using the indirect address. */
    status = program_ctx->memory_manager.get_memory_storage(
        indexed_indirect_address,
        &direct_address_storage,
        &direct_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Read the direct address from the storage location. */
    status = direct_address_storage->read(
        &direct_address,
        sizeof(direct_address),
        direct_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Convert the direct address, stored in memory in native endianess, into big endianess. */
    converted_direct_address = system_native_to_big_endianess(direct_address);

    /* Retrieve data at absolute indexed direct address. */
    status = program_ctx->memory_manager.get_memory_storage(
        converted_direct_address,
        &data_storage,
        &data_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for direct. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = data_storage_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IndirectYIndexedAddressMode::get_storage(
    ProgramContext *program_ctx,
    native_dword_t indirect_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_y = nullptr;
    MemoryStorage *data_storage = nullptr;
    MemoryStorage *direct_address_storage = nullptr;
    size_t direct_storage_offset = 0;
    size_t data_storage_offset = 0;
    native_word_t register_index = 0;
    native_address_t direct_address = 0;
    native_address_t indexed_direct_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Retrieve value of register Y. */
    register_y = program_ctx->register_file.get_register_y();
    register_index = register_y->read();

    /* Retrieve storage location containing the direct address using the indirect address. */
    status = program_ctx->memory_manager.get_memory_storage(
        indirect_address,
        &direct_address_storage,
        &direct_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Read the direct address from the storage location. */
    status = direct_address_storage->read(
        &direct_address,
        sizeof(direct_address),
        direct_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read failed for indirect. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Add index offset from register Y.
     * Note: Here, we do want carry behavior.
     * Note: Addresses are saved as little endian,
     * so we need to swap it to host machine endianness before adding the index.
     * */
    indexed_direct_address = system_native_to_big_endianess(direct_address) + register_index;

    /* Retrieve data at absolute indexed direct address. */
    status = program_ctx->memory_manager.get_memory_storage(
        indexed_direct_address,
        &data_storage,
        &data_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed for direct. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = data_storage_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}