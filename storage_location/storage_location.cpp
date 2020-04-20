/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

/** Headers ***************************************************************/
#include <new>

#include "common.h"

#include "storage_location/storage_location.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/


StorageLocation::StorageLocation(
    std::size_t storage_size,
    std::size_t storage_range_start
) : storage_size(storage_size), storage_range_start(storage_range_start)
{
    this->storage_buffer = HEAPALLOCZ(storage_size);
    if (nullptr == this->storage_buffer) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Failed to allocate a buffer of size %zu for storage location",
            storage_size
        );
        throw std::bad_alloc();
    }
}


StorageLocation::~StorageLocation()
{
    HEAPFREE(this->storage_buffer);
    this->storage_buffer = nullptr;
}
