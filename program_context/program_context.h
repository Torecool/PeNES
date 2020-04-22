/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __PROGRAM_CONTEXT_H__
#define __PROGRAM_CONTEXT_H__

/** Headers ***************************************************************/
#include <cstdint>

#include "system.h"

#include "storage_location/storage_location.h"
#include "memory_manager/memory_manager.h"
#include "utils/object_pool.h"


/** Structs ***************************************************************/
class RegisterStorage : public IStorageLocation {
public:
   inline explicit RegisterStorage(std::size_t register_size) : IStorageLocation(register_size) {};
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
    RegisterFile(void);

    constexpr inline RegisterStorage *get_register_a(void) const;

    constexpr inline RegisterStorage * get_register_x(void) const;

    constexpr inline RegisterStorage *get_register_y(void) const;

    constexpr inline RegisterStorage *get_register_status(void) const;

    constexpr inline RegisterStorage *get_register_stack_pointer(void) const;

    constexpr inline RegisterStorage *get_register_program_counter(void) const;

private:
    RegisterStorage register_a = RegisterStorage(sizeof(native_word_t));
    RegisterStorage register_x = RegisterStorage(sizeof(native_word_t));
    RegisterStorage register_y = RegisterStorage(sizeof(native_word_t));
    RegisterStorage register_status = RegisterStorage(sizeof(native_word_t));
    RegisterStorage register_stack_pointer = RegisterStorage(sizeof(native_word_t));
    RegisterStorage register_program_counter = RegisterStorage(sizeof(native_dword_t));
};


struct ProgramContext {
    RegisterFile register_file;
    MemoryManager memory_manager;
    utils::ObjectPool<ImmediateStorage> immediate_storage_pool;

};

#endif /* __PROGRAM_CONTEXT_H__ */
