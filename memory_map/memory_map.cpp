/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <algorithm>

#include "common.h"
#include "penes_status.h"

#include "memory_map/memory_map.h"

/** Constants *************************************************************/
#define MEMORY_MAP_ZERO_PAGE_START_ADDRESS (0x0)
#define MEMORY_MAP_STACK_START_ADDRESS (0x100)
#define MEMORY_MAP_RAM_START_ADDRESS (0x200)
#define MEMORY_MAP_RAM_MIRROR_START_ADDRESS (0x800)
#define MEMORY_MAP_IO_REGISTERS_START_ADDRESS (0x2000)
#define MEMORY_MAP_IO_MIRROR_START_ADDRESS (0x2008)
#define MEMORY_MAP_IO_REGISTERS_2_START_ADDRESS (0x4000)
#define MEMORY_MAP_EXPANSION_ROM_START_ADDRESS (0x4020)
#define MEMORY_MAP_SRAM_START_ADDRESS (0x6000)
#define MEMORY_MAP_PRG_ROM_LOWER_START_ADDRESS (0x8000)
#define MEMORY_MAP_PRG_ROM_UPPER_START_ADDRESS (0xC000)
#define MEMORY_MAP_END_ADDRESS (0x10000)

/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
static const std::vector<native_address_t> address_keys = {
    MEMORY_MAP_ZERO_PAGE_START_ADDRESS,         /* Zero Page */
    MEMORY_MAP_STACK_START_ADDRESS,             /* Stack */
    MEMORY_MAP_RAM_START_ADDRESS,               /* RAM */
    MEMORY_MAP_RAM_MIRROR_START_ADDRESS,        /* Mirrors 0x0000-0x7FF */
    MEMORY_MAP_IO_REGISTERS_START_ADDRESS,      /* I/O Registers */
    MEMORY_MAP_IO_MIRROR_START_ADDRESS,         /* Mirrors 0x2000-0x2007 */
    MEMORY_MAP_IO_REGISTERS_2_START_ADDRESS,    /* I/O Registers */
    MEMORY_MAP_EXPANSION_ROM_START_ADDRESS,     /* Expansion ROM */
    MEMORY_MAP_SRAM_START_ADDRESS,              /* SRAM */
    MEMORY_MAP_PRG_ROM_LOWER_START_ADDRESS,     /* PRG-ROM Lower Bank */
    MEMORY_MAP_PRG_ROM_UPPER_START_ADDRESS      /* PRG-ROM Upper Bank */
};

/** Functions *************************************************************/
MemoryMap::MemoryMap()
{
    this->storage_table = {
        new MemoryStorage(MEMORY_MAP_STACK_START_ADDRESS - MEMORY_MAP_ZERO_PAGE_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_RAM_START_ADDRESS - MEMORY_MAP_STACK_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_RAM_MIRROR_START_ADDRESS - MEMORY_MAP_RAM_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_IO_REGISTERS_START_ADDRESS - MEMORY_MAP_RAM_MIRROR_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_IO_MIRROR_START_ADDRESS - MEMORY_MAP_IO_REGISTERS_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_IO_REGISTERS_2_START_ADDRESS - MEMORY_MAP_IO_MIRROR_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_EXPANSION_ROM_START_ADDRESS - MEMORY_MAP_IO_REGISTERS_2_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_SRAM_START_ADDRESS - MEMORY_MAP_EXPANSION_ROM_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_PRG_ROM_LOWER_START_ADDRESS - MEMORY_MAP_SRAM_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_PRG_ROM_LOWER_START_ADDRESS - MEMORY_MAP_PRG_ROM_LOWER_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_PRG_ROM_UPPER_START_ADDRESS - MEMORY_MAP_PRG_ROM_LOWER_START_ADDRESS),
        new MemoryStorage(MEMORY_MAP_END_ADDRESS - MEMORY_MAP_PRG_ROM_UPPER_START_ADDRESS),
    };
}


enum PeNESStatus MemoryMap::get_memory_storage(
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
        MemoryMap::address_keys.begin(),
        MemoryMap::address_keys.end(),
        address
    );
    if (MemoryMap::address_keys.end() == address_key_iter) {
        status = PENES_STATUS_MEMORY_MAP_GET_MEMORY_STORAGE_NOT_FOUND;
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
    memory_storage_index = address_key_iter - MemoryMap::address_keys.begin();

    /* Verify the index is within the bounds of the memory map and retrieve the memory storage object. */
    if (this->storage_table.size() <= memory_storage_index) {
        status = PENES_STATUS_MEMORY_MAP_GET_MEMORY_STORAGE_OUT_OF_BOUNDS;
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
    *output_storage = this->storage_table.at(memory_storage_index);
    *output_storage_offset = address - *address_key_iter;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


MemoryStorage *MemoryMap::get_stack_storage() const
{
    return nullptr;
}
