/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

/** Headers ***************************************************************/
#include <cstdint>
#include <vector>

#include "penes_status.h"

#include "program_context/program_context.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/

/** Structs ***************************************************************/

/** Functions *************************************************************/



class MemoryStorage : public StorageLocation {};
class ImmediateStorage : public StorageLocation {};


class MemoryManager {
public:
    inline enum PeNESStatus data_at_address(
        native_address_t address,
        StorageLocation **output_storage_location
    ) const;
};


#endif /* __MEMORY_MANAGER_H__ */
