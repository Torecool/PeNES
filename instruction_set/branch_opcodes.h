/**
 * @brief  
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
class IBranchOpcode : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(
        AddressModeType default_address_mode = AddressModeType::ADDRESS_MODE_TYPE_NONE
    ) override {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_RELATIVE;
    }

    inline enum PeNESStatus branch(
        ProgramContext *program_ctx,
        IStorageLocation *branch_operand_storage,
        std::size_t operand_storage_offset = 0
    );

    inline enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *branch_operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_NONE;
    const bool branch_on_set = false;
};

/** Classes ***************************************************************/
/* Branch on Carry flag set. */
class OpcodeBCS: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_CARRY;
    const bool branch_on_set = true;
};


/* Branch on Carry flag clear. */
class OpcodeBCC: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_CARRY;
    const bool branch_on_set = false;
};


/* Branch on Zero flag set (ALU operation result is zero/equal). */
class OpcodeBEQ: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_ZERO;
    const bool branch_on_set = true;
};


/* Branch on Zero flag clear (ALU operation result is nonzero/unequal). */
class OpcodeBNE: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_ZERO;
    const bool branch_on_set = false;
};


/* Branch on Negative flag set (ALU operation result is negative). */
class OpcodeBMI: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_NEGATIVE;
    const bool branch_on_set = true;
};


/* Branch on Negative flag clear (ALU operation result is nonnegative). */
class OpcodeBPL: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_NEGATIVE;
    const bool branch_on_set = false;
};


/* Branch on Overflow flag set. */
class OpcodeBVS: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_OVERFLOW;
    const bool branch_on_set = true;
};


/* Branch on Overflow flag clear. */
class OpcodeBVC: public IBranchOpcode {
private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_OVERFLOW;
    const bool branch_on_set = false;
};

}

#endif /* __BRANCH_OPCODES_H__ */
