/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <algorithm>

#include "common.h"
#include "penes_status.h"

#include "memory_manager/memory_manager.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
static const std::vector<native_address_t> address_keys = {
    0x0,        /* Zero Page */
    0x100,      /* Stack */
    0x200,      /* RAM */
    0x800,      /* Mirrors 0x0000-0x7FF */
    0x2000,     /* I/O Registers */
    0x2008,     /* Mirrors 0x2000-0x2007 */
    0x4000,     /* I/O Registers */
    0x4020,     /* Expansion ROM */
    0x6000,     /* SRAM */
    0x8000,     /* PRG-ROM Lower Bank */
    0xC000      /* PRG-ROM Upper Bank */
};

static const std::vector<MemoryStorage *> memory_map;

/** Functions *************************************************************/
enum PeNESStatus MemoryManager::get_memory_storage(
    native_address_t address,
    MemoryStorage **output_storage,
    std::size_t *output_storage_offset
) const
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    std::vector<native_address_t>::const_iterator address_key_iter;
    std::size_t memory_storage_index = 0;

    ASSERT(nullptr != output_storage);
    ASSERT(nullptr != output_storage_offset);

    /* Find the memory storage key that "contains" the address,
     * meaning the closest key that is a lower bound.
     * */
    address_key_iter = std::lower_bound(
        this->address_keys.begin(),
        this->address_keys.end(),
        address
    );
    if (this->address_keys.end() == address_key_iter) {
        status = PENES_STATUS_MEMORY_MANAGER_GET_MEMORY_STORAGE_NOT_FOUND;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "lower_bound failed: no matching address found. Status: %d. Search address: %x\n",
            status,
            address
        );
        goto l_cleanup;
    }

    /* Calculate the index of the found address key within the vector,
     * so that we can extract the storage object from the memory map.
     * */
    memory_storage_index = address_key_iter - this->address_keys.begin();

    /* Verify the index is within the bounds of the memory map and retrieve the memory storage object. */
    if (this->memory_map.size() <= memory_storage_index) {
        status = PENES_STATUS_MEMORY_MANAGER_GET_MEMORY_STORAGE_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Address key index exceeds bounds of memory map. Status: %d. Index: %zu\n",
            status,
            memory_storage_index
        );
        goto l_cleanup;
    }

    /* The offset within the storage object needed in order to reference the desired memory cell,
     * is the distance between the given address and the matched address key,
     * since the key represents the first cell of the storage object.
     * */
    *output_storage = this->memory_map.at(memory_storage_index);
    *output_storage_offset = address - *address_key_iter;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
