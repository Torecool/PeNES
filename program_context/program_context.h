/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __PROGRAM_CONTEXT_H__
#define __PROGRAM_CONTEXT_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "system.h"

#include "storage_location/storage_location.h"
#include "memory_manager/memory_manager.h"
#include "utils/object_pool.h"


/** Structs ***************************************************************/
template<typename T>
class RegisterStorage : public IStorageLocation {
public:
   inline explicit RegisterStorage() : IStorageLocation(sizeof(T)) {};

   inline T get() {
       return *static_cast<T *>(this->storage_buffer);
   }

   inline void set(T register_data) {
        return *static_cast<T *>(this->storage_buffer) = register_data;
   }
};


class ImmediateStorage : public IStorageLocation {
public:
    inline explicit ImmediateStorage(std::size_t immediate_size) : IStorageLocation(immediate_size) {};

    inline enum PeNESStatus write(
        void *write_buffer,
        std::size_t write_size,
        std::size_t immediate_absolute_offset = 0
    ) override;

    inline enum PeNESStatus set(
        void *write_buffer,
        std::size_t write_size,
        std::size_t immediate_absolute_offset = 0
    );

    inline enum PeNESStatus reset();
};


class RegisterFile {
public:

    constexpr inline RegisterStorage<native_word_t> *get_register_a(void) {
        return &this->register_a;
    };

    constexpr inline RegisterStorage<native_word_t> * get_register_x(void) {
        return &this->register_x;
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_y(void) {
        return &this->register_y;
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_status(void) {
        return &this->register_status;
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_stack_pointer(void) {
        return &this->register_stack_pointer;
    };

    constexpr inline RegisterStorage<native_address_t> *get_register_program_counter(void) {
        return &this->register_program_counter;
    };

private:
    RegisterStorage<native_word_t> register_a;
    RegisterStorage<native_word_t> register_x;
    RegisterStorage<native_word_t> register_y;
    RegisterStorage<native_word_t> register_status;
    RegisterStorage<native_word_t> register_stack_pointer;
    RegisterStorage<native_address_t> register_program_counter;
};


struct ProgramContext {
    RegisterFile register_file;
    MemoryManager memory_manager;
    utils::ObjectPool<ImmediateStorage> immediate_storage_pool;

};

#endif /* __PROGRAM_CONTEXT_H__ */
