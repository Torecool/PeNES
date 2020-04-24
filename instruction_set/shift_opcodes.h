/**
 * @brief  
 * @author TBK
 * @date   24/04/2020
 * */

#ifndef __SHIFT_OPCODES_H__
#define __SHIFT_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
class IShiftOpcode : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *shift_storage,
        std::size_t storage_offset
    ) override;

private:
    inline virtual native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) = 0;
};

/* Shift left one bit. */
class OpcodeASL : public IShiftOpcode {
private:
    inline native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) override
    {
        UNREFERENCED_PARAMETER(status_register_data);

        return static_cast<native_dword_t>(storage_data) << 1;
    }
};

/* Shift right one bit. */
class OpcodeLSR : public IShiftOpcode {
private:
    inline native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) override
    {
        UNREFERENCED_PARAMETER(status_register_data);

        return static_cast<native_dword_t>(storage_data) >> 1;
    }
};

/* Rotate left one bit. */
class OpcodeROL : public IShiftOpcode {
private:
    inline native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) override
    {
        bool is_carry_set = (status_register_data & REGISTER_STATUS_FLAG_MASK_CARRY);
        native_dword_t shift_result = static_cast<native_dword_t>(storage_data) << 1;

        shift_result |= (true == is_carry_set)? 1: 0;

        return shift_result;
    }
};

/* Rotate right one bit. */
class OpcodeROR : public IShiftOpcode {
private:
    inline native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) override
    {
        bool is_carry_set = (status_register_data & REGISTER_STATUS_FLAG_MASK_CARRY);
        native_dword_t shift_result = static_cast<native_dword_t>(storage_data) >> 1;
        native_word_t shifted_carry = 1 << (NATIVE_WORD_SIZE_BITS - 1);

        shift_result |= (true == is_carry_set)? shifted_carry: 0;

        return shift_result;
    }
};

}

#endif /* __SHIFT_OPCODES_H__ */
