/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>
#include <new>

#include "common.h"

#include "storage_location/storage_location.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
IStorageLocation::IStorageLocation(std::size_t num_storage_words): storage_size(system_words_to_bytes(num_storage_words))
{
    this->storage_buffer = new native_word_t[system_words_to_bytes(num_storage_words)];
}


IStorageLocation::~IStorageLocation()
{
    delete this->storage_buffer;
}


enum PeNESStatus IStorageLocation::read(
    native_word_t *read_buffer,
    std::size_t num_read_words,
    std::size_t read_word_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != read_buffer);

    /* Verify that the area to read is within the bounds of the storage location. */
    if (this->storage_size < system_words_to_bytes(read_word_offset + num_read_words)) {
        status = PENES_STATUS_STORAGE_LOCATION_READ_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Requested read area exceeds the bounds of the storage location. Status: %d. Read size: %zu, read offset: %zu\n",
            status,
            num_read_words,
            read_word_offset
        );
        goto l_cleanup;
    }

    /* Copy memory from the storage location buffer into the read buffer. */
    COPY_MEMORY(
        read_buffer,
        this->storage_buffer + read_word_offset,
        system_words_to_bytes(num_read_words)
    );

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStorageLocation::write(
    const native_word_t *write_buffer,
    std::size_t num_write_words,
    std::size_t write_word_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != write_buffer);

    /* Verify that the area to write is within the bounds of the storage location. */
    if (this->storage_size < system_words_to_bytes(write_word_offset + num_write_words)) {
        status = PENES_STATUS_STORAGE_LOCATION_WRITE_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Requested write area exceeds the bounds of the storage location. Status: %d. Write size: %zu, write offset: %zu\n",
            status,
            num_write_words,
            write_word_offset
        );
        goto l_cleanup;
    }

    /* Copy memory from the write buffer into the storage location buffer. */
    COPY_MEMORY(
        this->storage_buffer + write_word_offset,
        write_buffer,
        system_words_to_bytes(num_write_words)
    );

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStorageLocation::transfer(
    IStorageLocation *dest_storage_location,
    std::size_t num_transfer_words,
    std::size_t src_transfer_word_offset,
    std::size_t dest_transfer_word_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t *transfer_buffer = nullptr;

    ASSERT(nullptr != dest_storage_location);

    /* Allocate a buffer to contain the words to transfer. */
    transfer_buffer = new native_word_t[num_transfer_words];

    /* Read the words to transfer from the source storage location (this). */
    status = this->read(transfer_buffer, num_transfer_words, src_transfer_word_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Source read failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Write the words to transfer into the destination storage location. */
    status = dest_storage_location->write(
        transfer_buffer,
        num_transfer_words,
        dest_transfer_word_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Destination write failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    delete transfer_buffer;
    return status;
}


template<typename SizeType>
enum PeNESStatus ImmediateStorage<SizeType>::write(
    const native_word_t *write_buffer,
    std::size_t num_immediate_words,
    std::size_t immediate_word_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != write_buffer);

    status = PENES_STATUS_STORAGE_LOCATION_IMMEDIATE_STORAGE_WRITE_INVALID_OPERATION;
    DEBUG_PRINT_WITH_ARGS("Cannot write to immediate storage location. Status: %d\n", status);
    goto l_cleanup;

l_cleanup:
    return status;
}


template<typename SizeType>
void ImmediateStorage<SizeType>::set(const native_word_t *immediate_data)
{
    ASSERT(nullptr != immediate_data);

    /* Copy memory from the input data buffer into the immediate storage buffer. */
    COPY_MEMORY(this->storage_buffer, immediate_data,this->storage_size);
}


template<typename SizeType>
SizeType RegisterStorage<SizeType>::read()
{
    SizeType read_buffer = 0;

    /* Copy memory from the register data buffer into the read buffer and return it. */
    COPY_MEMORY(&read_buffer, this->storage_buffer, this->storage_size);

    return read_buffer;
}


template<typename SizeType>
void RegisterStorage<SizeType>::write(SizeType register_data)
{
    ASSERT(nullptr != register_data);

    /* Copy memory from the input data buffer into the register storage buffer. */
    COPY_MEMORY(this->storage_buffer, register_data,this->storage_size);
}


template<typename SizeType>
void RegisterStorage<SizeType>::transfer(RegisterStorage<SizeType> *dest_register)
{
    ASSERT(nullptr != dest_register);

    /* Write the data from the source register buffer directly into the destination register buffer. */
    dest_register->write(this->storage_buffer);
}