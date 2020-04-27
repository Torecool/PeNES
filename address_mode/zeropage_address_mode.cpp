/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "memory_map/memory_map.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/zeropage_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_mode;

/** Functions *************************************************************/
inline enum PeNESStatus ZeropageAddressMode::get_storage(
    ProgramContext *program_ctx,
    native_dword_t zeropage_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    native_word_t address_data = 0;
    size_t data_storage_offset = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Retrieve storage at absolute zeropage address. */
    status = program_ctx->memory_map.get_memory_storage(
        zeropage_address,
        &data_storage,
        &data_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = data_storage_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus ZeropageXIndexedAddressMode::get_storage(
    ProgramContext *program_context,
    native_dword_t zeropage_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    RegisterStorage<native_word_t> *register_x = nullptr;
    size_t data_storage_offset = 0;
    native_address_t indexed_zeropage_address = 0;
    native_word_t register_index = 0;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Add index offset from register X.
     * Note: We don't want carry behavior. If the summation overflows the address should wrap around.
     * */
    register_x = program_context->register_file.get_register_x();
    register_index = register_x->read();

    indexed_zeropage_address = static_cast<native_address_t>(
        static_cast<native_word_t>(zeropage_address) + register_index
    );

    /* Retrieve storage at absolute indexed zeropage address. */
    status = program_context->memory_map.get_memory_storage(
        indexed_zeropage_address,
        &data_storage,
        &data_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = data_storage_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus ZeropageYIndexedAddressMode::get_storage(
    ProgramContext *program_context,
    native_dword_t zeropage_address,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    MemoryStorage *data_storage = nullptr;
    RegisterStorage<native_word_t> *register_y = nullptr;
    size_t data_storage_offset = 0;
    native_address_t indexed_zeropage_address = 0;
    native_word_t register_index = 0;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Add index offset from register Y.
     * Note: We don't want carry behavior. If the summation overflows the address should wrap around.
     * */
    register_y = program_context->register_file.get_register_y();
    register_index = register_y->read();

    indexed_zeropage_address = static_cast<native_address_t>(
        static_cast<native_word_t>(zeropage_address) + register_index
    );

    /* Retrieve storage at absolute indexed zeropage address. */
    status = program_context->memory_map.get_memory_storage(
        indexed_zeropage_address,
        &data_storage,
        &data_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("data_at_address failed. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = data_storage;
    *output_storage_offset = data_storage_offset;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}