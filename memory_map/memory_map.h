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

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/

/** Structs ***************************************************************/

/** Functions *************************************************************/



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
        std::size_t *output_storage_offset
    ) const;

    MemoryStorage *get_stack_storage() const;

    inline MemoryStorage *get_nmi_interrupt_vector() const;

    inline MemoryStorage *get_reset_interrupt_vector() const;

    inline MemoryStorage *get_irq_interrupt_vector() const;

private:
    static const std::vector<native_address_t> address_keys;
    std::vector<MemoryStorage *> storage_table;
};


#endif /* __MEMORY_MAP_H__ */
