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



/** Structs ***************************************************************/
class RegisterStorage : public StorageLocation {
public:
   inline explicit RegisterStorage(size_t register_size) : StorageLocation(register_size) {};
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
};

#endif /* __PROGRAM_CONTEXT_H__ */
