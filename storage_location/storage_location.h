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
        std::size_t storage_size
    );

    inline ~StorageLocation();

    virtual inline enum PeNESStatus read(
        void *read_buffer,
        std::size_t read_size,
        std::size_t read_offset = 0
    );

    virtual inline enum PeNESStatus write(
        void *write_buffer,
        std::size_t write_size,
        std::size_t write_offset = 0
    );

    constexpr inline std::size_t get_storage_size() const
    {
        return this->storage_size;
    }

protected:
    const std::size_t storage_size = 0;
    void *storage_buffer = nullptr;
};


#endif /* __STORAGE_LOCATION_H__ */