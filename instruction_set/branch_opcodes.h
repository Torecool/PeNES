/**
 * @brief  Definitions for branch-related opcodes.
 * @author TBK
 * @date   22/04/2020
 * */

#ifndef __BRANCH_OPCODES_H__
#define __BRANCH_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Interfaces ************************************************************/
/** @brief Interface of an opcode performing a branch operation using the Status register.
 *         Each subclass declares the status flag that should be checked and the required value for the branch.
 *         The exec method checks if the branch condition is met, and performs the branch operation accordingly.
 * */
class IBranchOpcode : public IOpcode {
public:
    inline address_mode::AddressModeType resolve_address_mode(
        address_mode::AddressModeType default_address_mode
    ) override
    {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_RELATIVE;
    }

    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *branch_operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    /** @brief          Perform a branch operation to a relative offset specified by the storage operand.
     *
     *  @param[in]      program_ctx                 The program context containing the Program counter to modify.
     *  @param[in]      branch_operand_storage      The storage location specifying the relative branch location.
     *  @param[in]      storage_data                The offset within the storage location to read the data from.
     *
     *  @return         Status indicating the success of the operation.
     * */
    static inline enum PeNESStatus branch(
        ProgramContext *program_ctx,
        IStorageLocation *branch_operand_storage,
        std::size_t operand_storage_offset = 0
    );

    /* The status flag to check for a branch condition. */
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_NONE;
    /* The required value (set/clear) of the branch flag for the branch to occur. */
    const bool branch_on_set = false;
};

/** Classes ***************************************************************/
/** @brief Branch on Carry flag set. */
class OpcodeBCS: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
    const bool branch_on_set = true;
};


/** @brief Branch on Carry flag clear. */
class OpcodeBCC: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
    const bool branch_on_set = false;
};


/** @brief Branch on Zero flag set (ALU operation result is zero/equal). */
class OpcodeBEQ: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_ZERO;
    const bool branch_on_set = true;
};


/** @brief Branch on Zero flag clear (ALU operation result is nonzero/unequal). */
class OpcodeBNE: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_ZERO;
    const bool branch_on_set = false;
};


/** @brief Branch on Negative flag set (ALU operation result is negative). */
class OpcodeBMI: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE;
    const bool branch_on_set = true;
};


/** @brief Branch on Negative flag clear (ALU operation result is nonnegative). */
class OpcodeBPL: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE;
    const bool branch_on_set = false;
};


/** @brief Branch on Overflow flag set. */
class OpcodeBVS: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_OVERFLOW;
    const bool branch_on_set = true;
};


/** @brief Branch on Overflow flag clear. */
class OpcodeBVC: public IBranchOpcode {
protected:
    const native_word_t branch_condition_mask = REGISTER_STATUS_FLAG_MASK_OVERFLOW;
    const bool branch_on_set = false;
};

} /* namespace instruction_set */

#endif /* __BRANCH_OPCODES_H__ */
