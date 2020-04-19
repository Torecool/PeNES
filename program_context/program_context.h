/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __PROGRAM_CONTEXT_H__
#define __PROGRAM_CONTEXT_H__

/** Headers ***************************************************************/
#include <cstdint>

#include "memory_manager/memory_manager.h"

typedef uint8_t native_word_t;
typedef uint16_t native_dword_t;
typedef native_dword_t native_address_t;

/** Structs ***************************************************************/

struct RegisterFile {
    native_word_t register_a;
    native_word_t register_x;
    native_word_t register_y;
    native_word_t register_status;
    native_word_t register_stack_pointer;
    native_dword_t register_program_counter;
};

struct ProgramContext {
    RegisterFile register_file;
    MemoryManager memory_manager;
};

#endif /* __PROGRAM_CONTEXT_H__ */
