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
    explicit MemoryStorage(std::size_t num_storage_words): IStorageLocation(num_storage_words)
    {};
};


class MemoryMap {
public:
    MemoryMap();

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

    inline MemoryStorage *get_stack_storage() const
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
    static const std::vector<enum MemoryMapAddress> address_keys;
    std::vector<MemoryStorage *> storage_table;
    MemoryStorage *stack_storage = nullptr;
    MemoryStorage *nmi_jump_vector_storage = nullptr;
    MemoryStorage *reset_jump_vector_storage = nullptr;
    MemoryStorage *irq_jump_vector_storage = nullptr;
};


#endif /* __MEMORY_MAP_H__ */
