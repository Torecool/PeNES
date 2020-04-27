/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "memory_map/memory_map.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/immediate_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_mode;

/** Functions *************************************************************/
template<typename SizeType>
enum PeNESStatus ImmediateAddressMode<SizeType>::get_storage(
    ProgramContext *program_ctx,
    native_dword_t immediate_value,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    ImmediateStorage<SizeType> *immediate_storage = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_storage);

    /* Create a new immediate storage object. */
    immediate_storage = new ImmediateStorage<SizeType>();

    /* Set the immediate value of the storage object.
     * This will be the return value when calling read.
     * */
    immediate_storage->set(immediate_value);

    *output_storage = immediate_storage;
    if (nullptr != output_storage_offset) {
        *output_storage_offset = 0;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
