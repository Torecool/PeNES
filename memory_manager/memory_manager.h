/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

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



class MemoryStorage : public IStorageLocation {};


class MemoryManager {
public:
    inline enum PeNESStatus get_memory_storage(
        native_address_t address,
        MemoryStorage **output_storage,
        std::size_t *output_storage_offset
    ) const;
};


#endif /* __MEMORY_MANAGER_H__ */
