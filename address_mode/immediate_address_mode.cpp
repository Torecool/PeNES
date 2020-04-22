/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

/** Headers ***************************************************************/
#include "penes_status.h"
#include "common.h"

#include "memory_manager/memory_manager.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/immediate_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_modes;

/** Functions *************************************************************/
inline enum PeNESStatus ImmediateAddressMode::get_storage(
    const ProgramContext *program_context,
    native_dword_t immediate_value,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    ImmediateStorage *immediate_storage = nullptr;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_storage);

    status = program_context->immediate_storage_pool.retreive(&immediate_storage);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("retrieve failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* First, reset the contents of the immediate storage. */
    status = immediate_storage->reset();
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("reset failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Now, write the immediate value to the storage. */
    status = immediate_storage->set(&immediate_value, sizeof(immediate_value));
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("set failed. Status: %d\n", status);
        goto l_cleanup;
    }

    *output_storage = immediate_storage;
    if (nullptr != output_storage_offset) {
        *output_storage_offset = 0;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}

