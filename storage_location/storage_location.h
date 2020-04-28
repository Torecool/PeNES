/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

#ifndef __STORAGE_LOCATION_H__
#define __STORAGE_LOCATION_H__

/** Headers ***************************************************************/
#include <cstddef>
#include <cstdlib>

#include "system.h"

#include "penes_status.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
class IStorageLocation {
public:
    inline explicit IStorageLocation(std::size_t num_storage_words = 0):
        storage_size(system_words_to_bytes(num_storage_words))
    {
        if (0 < this->storage_size) {
            this->storage_buffer = new native_word_t[this->storage_size];
        }
    }

    inline ~IStorageLocation()
    {
        if (0 < this->storage_size) {
            delete this->storage_buffer;
        }
    }

    virtual enum PeNESStatus read(
        native_word_t *read_buffer,
        std::size_t num_read_words,
        std::size_t read_word_offset
    );

    virtual enum PeNESStatus write(
        const native_word_t *write_buffer,
        std::size_t num_write_words,
        std::size_t write_word_offset
    );

    virtual enum PeNESStatus transfer(
        IStorageLocation *dest_storage_location,
        std::size_t num_transfer_words,
        std::size_t src_transfer_word_offset,
        std::size_t dest_transfer_word_offset
    );

    constexpr inline std::size_t get_storage_size() const
    {
        return this->storage_size;
    }

    constexpr inline std::size_t get_num_storage_words() const
    {
        return system_words_to_bytes(this->storage_size);
    }

protected:
    std::size_t storage_size = 0;
    native_word_t *storage_buffer = nullptr;
};


template<typename SizeType>
class ImmediateStorage : public IStorageLocation {
public:
    inline ImmediateStorage(): IStorageLocation(system_bytes_to_words(sizeof(SizeType)))
    {};

    inline enum PeNESStatus write(
        const native_word_t *write_buffer,
        std::size_t num_immediate_words,
        std::size_t immediate_word_offset
    ) override
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

        ASSERT(nullptr != write_buffer);

        status = PENES_STATUS_STORAGE_LOCATION_IMMEDIATE_STORAGE_WRITE_INVALID_OPERATION;
        DEBUG_PRINT_WITH_ARGS("Cannot write to immediate storage location. Status: %d\n", status);

        return status;
    }

    inline void set(SizeType immediate_data)
    {
        /* Copy memory from the input data buffer into the immediate storage buffer. */
        COPY_MEMORY(this->storage_buffer, &immediate_data,this->storage_size);
    }
};


template<typename SizeType>
class RegisterStorage : public IStorageLocation {
public:
    inline RegisterStorage(): IStorageLocation(system_bytes_to_words(sizeof(SizeType)))
    {};

    inline SizeType read()
    {
        SizeType read_buffer = 0;

        /* Copy memory from the register data buffer into the read buffer and return it. */
        COPY_MEMORY(&read_buffer, this->storage_buffer, this->storage_size);

        return read_buffer;
    }

    inline void write(SizeType register_data)
    {
        /* Copy memory from the input data buffer into the register storage buffer. */
        COPY_MEMORY(this->storage_buffer, &register_data,this->storage_size);
    }

    inline enum PeNESStatus transfer(RegisterStorage<SizeType> *dest_register)
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

        ASSERT(nullptr != dest_register);

        /* Write the data from the source register buffer directly into the destination register buffer. */
        status = dest_register->IStorageLocation::write(
            this->storage_buffer,
            system_bytes_to_words(sizeof(SizeType)),
            0
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ARGS("write failed. Status: %d\n", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }
};

#endif /* __STORAGE_LOCATION_H__ */
