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

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/

/** Structs ***************************************************************/

/** Functions *************************************************************/
class StorageLocation {
public:
    StorageLocation(size_t storage_size, native_word_t *storage_buffer = nullptr);

    virtual inline enum PeNESStatus read(native_word_t *read_buffer, size_t *read_size);

    virtual inline enum PeNESStatus write(native_word_t *write_buffer, size_t *write_size);

protected:
    native_word_t *
};

class MemoryManager {
public:
    inline enum PeNESStatus data_at_address(
        native_dword_t address,
        void *data_buffer,
        size_t *data_size
    ) const;
};


#endif /* __MEMORY_MANAGER_H__ */
