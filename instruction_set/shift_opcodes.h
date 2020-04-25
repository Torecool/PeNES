/**
 * @brief  Definitions for shift-related opcodes.
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
/** @brief Interface of an opcode performing a shift operation using the Accumulator or a memory location.
 *         Each subclass implements the operation method for performing various shift/rotate operations,
 *         which is then invoked by the interface's exec method.
 * */
class IShiftOpcode : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *shift_storage,
        std::size_t storage_offset
    ) override;

protected:
    /** @brief          Perform a shift operation on the given data, using flags from the status register.
     *                  The return value is the new value of the operand storage location.
     *
     *  @param[in]      status_register_data        The data of the Status register.
     *  @param[in]      storage_data                The data from the storage location to shift.
     *
     *  @return         The result of the shift operation, to be written back to the storage location.
     *
     *  @note           Although both operands are of size WORD, the return value is of size DWORD.
     *                  This is in order to retain data needed for the calculation of the updated status,
     *                  namely the value of the Carry flag.
     * */
    inline virtual native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) = 0;

    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE |
                                      REGISTER_STATUS_FLAG_MASK_CARRY |
                                      REGISTER_STATUS_FLAG_MASK_ZERO;
};

/** @brief Shift left one bit. */
class OpcodeASL : public IShiftOpcode {
protected:
    inline native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) override
    {
        UNREFERENCED_PARAMETER(status_register_data);

        return static_cast<native_dword_t>(storage_data) << 1;
    }
};

/** @brief Shift right one bit. */
class OpcodeLSR : public IShiftOpcode {
protected:
    inline native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) override
    {
        UNREFERENCED_PARAMETER(status_register_data);

        return static_cast<native_dword_t>(storage_data) >> 1;
    }
};

/** @brief Rotate left one bit. */
class OpcodeROL : public IShiftOpcode {
protected:
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

/** @brief Rotate right one bit. */
class OpcodeROR : public IShiftOpcode {
protected:
    inline native_dword_t operation(
        native_word_t status_register_data,
        native_word_t storage_data
    ) override
    {
        bool is_carry_set = (status_register_data & REGISTER_STATUS_FLAG_MASK_CARRY);
        native_dword_t shift_result = static_cast<native_dword_t>(storage_data) >> 1;

        shift_result |= (true == is_carry_set)? SYSTEM_NATIVE_WORD_SIGN_BIT_MASK: 0;

        return shift_result;
    }
};

} /* namespace instruction_set */

#endif /* __SHIFT_OPCODES_H__ */
