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

/** Static Variables ******************************************************/
const std::vector<enum MemoryMapAddress> MemoryMap::address_keys = {
    MEMORY_MAP_ADDRESS_START_ZERO_PAGE,            /* Zero Page */
    MEMORY_MAP_ADDRESS_START_STACK,                /* Stack */
    MEMORY_MAP_ADDRESS_START_RAM,                  /* RAM */
    MEMORY_MAP_ADDRESS_START_RAM_MIRROR,           /* Mirrors 0x0000-0x7FF */
    MEMORY_MAP_ADDRESS_START_IO_REGISTERS,         /* I/O Registers */
    MEMORY_MAP_ADDRESS_START_IO_MIRROR,            /* Mirrors 0x2000-0x2007 */
    MEMORY_MAP_ADDRESS_START_IO_REGISTERS_2,       /* I/O Registers */
    MEMORY_MAP_ADDRESS_START_EXPANSION_ROM,        /* Expansion ROM */
    MEMORY_MAP_ADDRESS_START_SRAM,                 /* SRAM */
    MEMORY_MAP_ADDRESS_START_PRG_ROM_LOWER,        /* PRG-ROM Lower Bank */
    MEMORY_MAP_ADDRESS_START_PRG_ROM_UPPER,        /* PRG-ROM Upper Bank */
    MEMORY_MAP_ADDRESS_START_NMI_JUMP_VECTOR,      /* NMI Jump Vector */
    MEMORY_MAP_ADDRESS_START_RESET_JUMP_VECTOR,    /* RESET Jump Vector */
    MEMORY_MAP_ADDRESS_START_IRQ_JUMP_VECTOR       /* IRQ Jump Vector */
};

/** Functions *************************************************************/
MemoryMap::MemoryMap()
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    std::vector<enum MemoryMapAddress>::const_iterator address_key_iter;
    std::size_t memory_storage_size = 0;
    std::size_t next_address_start = 0;

    /* Iterate through the address key table.
     * Create a new storage object for each entry in the map,
     * with the size being the distance between the entry's start address and the following entry's start address.
     * */
    for (
        address_key_iter = MemoryMap::address_keys.begin();
        address_key_iter < MemoryMap::address_keys.end();
        address_key_iter++
    )
    {
        if (MemoryMap::address_keys.end() > address_key_iter + 1) {
            next_address_start = *(address_key_iter + 1);
        } else {
            next_address_start = MEMORY_MAP_ADDRESS_END;
        }

        memory_storage_size = next_address_start - *address_key_iter;
        this->storage_table.push_back(new MemoryStorage(memory_storage_size));
    }

    /* Search the table for "common" memory storage objects, in able to provide a "shortcut" retrieval method. */
    status = this->get_memory_storage(
        MEMORY_MAP_ADDRESS_START_STACK,
        &this->stack_storage
    );
    ASSERT(PENES_STATUS_SUCCESS == status);

    status = this->get_memory_storage(
        MEMORY_MAP_ADDRESS_START_NMI_JUMP_VECTOR,
        &this->nmi_jump_vector_storage
    );
    ASSERT(PENES_STATUS_SUCCESS == status);

    status = this->get_memory_storage(
        MEMORY_MAP_ADDRESS_START_RESET_JUMP_VECTOR,
        &this->reset_jump_vector_storage
    );
    ASSERT(PENES_STATUS_SUCCESS == status);

    status = this->get_memory_storage(
        MEMORY_MAP_ADDRESS_START_IRQ_JUMP_VECTOR,
        &this->irq_jump_vector_storage
    );
    ASSERT(PENES_STATUS_SUCCESS == status);
}


enum PeNESStatus MemoryMap::get_memory_storage(
    native_address_t address,
    MemoryStorage **output_storage,
    std::size_t *output_storage_offset
) const
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    std::vector<enum MemoryMapAddress>::const_iterator address_key_iter;
    std::size_t memory_storage_index = 0;

    ASSERT(nullptr != output_storage);

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

    if (nullptr != output_storage_offset) {
        *output_storage_offset = address - *address_key_iter;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}