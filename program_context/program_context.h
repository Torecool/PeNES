/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __PROGRAM_CONTEXT_H__
#define __PROGRAM_CONTEXT_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "system.h"

#include "storage_location/storage_location.h"
#include "memory_map/memory_map.h"
#include "utils/utils.h"
#include "rom_loader/rom_loader.h"

/** Constants *************************************************************/
#define PROGRAM_CONTEXT_REGISTER_A_INITIAL_VALUE (0)
#define PROGRAM_CONTEXT_REGISTER_X_INITIAL_VALUE (0)
#define PROGRAM_CONTEXT_REGISTER_Y_INITIAL_VALUE (0)
#define PROGRAM_CONTEXT_REGISTER_STATUS_INITIAL_VALUE (0)
#define PROGRAM_CONTEXT_REGISTER_STACK_POINTER_INITIAL_VALUE (0xFF)
#define PROGRAM_CONTEXT_REGISTER_PROGRAM_COUNTER_INITIAL_VALUE (0)

/** Enums *****************************************************************/
enum RegisterStatusFlagMask {
    REGISTER_STATUS_FLAG_MASK_NONE = 0,
    REGISTER_STATUS_FLAG_MASK_CARRY = 1 << 0,
    REGISTER_STATUS_FLAG_MASK_ZERO = 1 << 1,
    REGISTER_STATUS_FLAG_MASK_INTERRUPT = 1 << 2,
    REGISTER_STATUS_FLAG_MASK_DECIMAL = 1 << 3,
    REGISTER_STATUS_FLAG_MASK_BREAK = 1 << 4,
    REGISTER_STATUS_FLAG_MASK_UNUSED = 1 << 5,
    REGISTER_STATUS_FLAG_MASK_OVERFLOW = 1 << 6,
    REGISTER_STATUS_FLAG_MASK_NEGATIVE = 1 << 7
};

/** Classes ***************************************************************/
class RegisterFile {
public:
    explicit RegisterFile(
        native_word_t register_a_data = PROGRAM_CONTEXT_REGISTER_A_INITIAL_VALUE,
        native_word_t register_x_data = PROGRAM_CONTEXT_REGISTER_X_INITIAL_VALUE,
        native_word_t register_y_data = PROGRAM_CONTEXT_REGISTER_Y_INITIAL_VALUE,
        native_word_t register_status_data = PROGRAM_CONTEXT_REGISTER_STATUS_INITIAL_VALUE,
        native_word_t register_stack_pointer_data = PROGRAM_CONTEXT_REGISTER_STACK_POINTER_INITIAL_VALUE,
        native_address_t register_program_data = PROGRAM_CONTEXT_REGISTER_PROGRAM_COUNTER_INITIAL_VALUE
    )
    {
        /* Initialize each register with its initial value. */
        register_a.write(register_a_data);
        register_x.write(register_x_data);
        register_y.write(register_y_data);
        register_status.write(register_status_data);
        register_stack_pointer.write(register_stack_pointer_data);
        register_program_counter.write(register_program_data);
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_a() {
        return &this->register_a;
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_x()
    {
        return &this->register_x;
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_y() {
        return &this->register_y;
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_status() {
        return &this->register_status;
    };

    constexpr inline RegisterStorage<native_word_t> *get_register_stack_pointer() {
        return &this->register_stack_pointer;
    };

    constexpr inline RegisterStorage<native_address_t> *get_register_program_counter() {
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

/** Structs ***************************************************************/
class ProgramContext {
public:
    inline explicit ProgramContext(ROMLoader *file_loader): memory_map(file_loader)
    {
        ASSERT(nullptr != file_loader);
    }

    RegisterFile register_file;
    MemoryMap memory_map;
};

#endif /* __PROGRAM_CONTEXT_H__ */
