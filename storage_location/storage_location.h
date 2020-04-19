/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

#ifndef __STORAGE_LOCATION_H__
#define __STORAGE_LOCATION_H__

/** Headers ***************************************************************/
#include <cstdlib>

#include "system.h"

#include "penes_status.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
class StorageLocation {
public:
    inline explicit StorageLocation(
        size_t storage_size,
        size_t storage_range_start = 0
    );

    inline ~StorageLocation();

    virtual inline enum PeNESStatus read(
        void *read_buffer,
        size_t read_size,
        size_t storage_range_offset = 0
    );

    virtual inline enum PeNESStatus write(
        void *write_buffer,
        size_t write_size,
        size_t storage_range_offset = 0
    );

    constexpr inline size_t get_storage_size() const
    {
        return this->storage_size;
    }

    constexpr inline size_t get_range_start() const
    {
        return this->storage_range_start;
    }

protected:
    const size_t storage_size = 0;
    const size_t storage_range_start = 0;
    void *storage_buffer = nullptr;
};


#endif /* __STORAGE_LOCATION_H__ */
