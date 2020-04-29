/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

/** Headers ***************************************************************/
#include <cstddef>
#include <cstdint>
#include <vector>

#include "penes_status.h"
#include "system.h"

#include "storage_location/storage_location.h"
#include "rom_loader/rom_loader.h"

/** Enums *****************************************************************/
enum MemoryMapAddress {
    MEMORY_MAP_ADDRESS_NONE = -1,
    MEMORY_MAP_ADDRESS_START_ZERO_PAGE = 0x0,
    MEMORY_MAP_ADDRESS_START_STACK = 0x100,
    MEMORY_MAP_ADDRESS_START_RAM = 0x200,
    MEMORY_MAP_ADDRESS_START_RAM_MIRROR = 0x800,
    MEMORY_MAP_ADDRESS_START_IO_REGISTERS = 0x2000,
    MEMORY_MAP_ADDRESS_START_IO_MIRROR = 0x2008,
    MEMORY_MAP_ADDRESS_START_IO_REGISTERS_2 = 0x4000,
    MEMORY_MAP_ADDRESS_START_EXPANSION_ROM = 0x4020,
    MEMORY_MAP_ADDRESS_START_SRAM = 0x6000,
    MEMORY_MAP_ADDRESS_START_PRG_ROM_LOWER = 0x8000,
    MEMORY_MAP_ADDRESS_START_PRG_ROM_UPPER = 0xC000,
    MEMORY_MAP_ADDRESS_START_NMI_JUMP_VECTOR = 0xFFFA,
    MEMORY_MAP_ADDRESS_START_RESET_JUMP_VECTOR = 0xFFFC,
    MEMORY_MAP_ADDRESS_START_IRQ_JUMP_VECTOR = 0xFFFE,
    MEMORY_MAP_ADDRESS_END = 0x10000
};

/** Classes ***************************************************************/
class MemoryStorage : public IStorageLocation {
public:
    inline explicit MemoryStorage(std::size_t num_storage_words = 0):
        IStorageLocation(num_storage_words),
        is_mirror(false)
    {};

    /* Constructor for creating a mirror memory storage object.
     * Both this instance's storage buffer as well as the original storage buffer point to the same memory.
     * */
    inline MemoryStorage(
        MemoryStorage *original_storage,
        std::size_t num_storage_words,
        std::size_t original_storage_offset = 0
    ): IStorageLocation(), is_mirror(true)
    {
        ASSERT(nullptr != original_storage);
        ASSERT(original_storage->get_num_storage_words() >= original_storage_offset + num_storage_words);

        this->storage_buffer = original_storage->storage_buffer + original_storage_offset;
        this->storage_size = system_words_to_bytes(num_storage_words);
    }

    inline ~MemoryStorage()
    {
        /* TODO double free error. */
        /* Only call the destructor on the original copy. */
        if (false == is_mirror) {
            this->IStorageLocation::~IStorageLocation();
        }
    }

private:
    /* TODO replace with shared_ptr? */
    const bool is_mirror;

    /* The memory map needs to be able to directly manage the internal storage buffer and so it is a friend. */
    friend class MemoryMap;
};


class MemoryMap {
public:
     MemoryMap();

     explicit MemoryMap(ROMLoader *rom_loader);

     inline ~MemoryMap()
     {
         for (MemoryStorage *memory_storage : this->storage_table) {
             delete memory_storage;
         }

         this->storage_table.clear();
     }

    enum PeNESStatus get_memory_storage(
        native_address_t address,
        MemoryStorage **output_storage,
        std::size_t *output_storage_offset = nullptr
    ) const;

    inline MemoryStorage *get_stack() const
    {
        ASSERT(nullptr != this->stack_storage);

        return this->stack_storage;
    }

    inline MemoryStorage *get_nmi_jump_vector() const
    {
        ASSERT(nullptr != this->nmi_jump_vector_storage);

        return this->nmi_jump_vector_storage;
    }

    inline MemoryStorage *get_reset_jump_vector() const
    {
        ASSERT(nullptr != this->reset_jump_vector_storage);

        return this->reset_jump_vector_storage;
    }

    inline MemoryStorage *get_irq_jump_vector() const
    {
        ASSERT(nullptr != this->irq_jump_vector_storage);

        return this->irq_jump_vector_storage;
    }

private:
    enum PeNESStatus setup_storage_shortcuts();

    static const std::vector<enum MemoryMapAddress> address_keys;
    std::vector<MemoryStorage *> storage_table;

    MemoryStorage *stack_storage = nullptr;
    MemoryStorage *nmi_jump_vector_storage = nullptr;
    MemoryStorage *reset_jump_vector_storage = nullptr;
    MemoryStorage *irq_jump_vector_storage = nullptr;
};


#endif /* __MEMORY_MAP_H__ */
