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
    inline explicit IStorageLocation(std::size_t num_storage_words);

    inline ~IStorageLocation();

    virtual inline enum PeNESStatus read(
        native_word_t *read_buffer,
        std::size_t num_read_words,
        std::size_t read_word_offset
    );

    virtual inline enum PeNESStatus write(
        const native_word_t *write_buffer,
        std::size_t num_write_words,
        std::size_t write_word_offset
    );

    virtual inline enum PeNESStatus transfer(
        IStorageLocation *dest_storage_location,
        std::size_t num_transfer_words,
        std::size_t src_transfer_word_offset,
        std::size_t dest_transfer_word_offset
    );

    constexpr inline std::size_t get_storage_size() const
    {
        return this->storage_size;
    }

protected:
    const std::size_t storage_size = 0;
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
    ) override;

    inline void set(const native_word_t *immediate_data);
};


template<typename SizeType>
class RegisterStorage : public IStorageLocation {
public:
    inline RegisterStorage(): IStorageLocation(system_bytes_to_words(sizeof(SizeType)))
    {};

    inline SizeType read();

    inline void write(SizeType register_data);

    inline void transfer(RegisterStorage<SizeType> *dest_register);
};

#endif /* __STORAGE_LOCATION_H__ */
