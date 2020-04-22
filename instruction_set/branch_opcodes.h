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

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
namespace instruction_set {

class IBranchOpcode : public IOpcode {
public:
    inline virtual enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset = 0
    ) override {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;
        native_word_t register_status_data = 0;

        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != operand_storage);

        /* Retrieve the status register. */
        register_status = program_ctx->register_file.get_register_status();
        register_status_data = register_status->get();

        if (branch_on_set == (register_status_data & branch_condition_mask)) {
            status = IBranchOpcode::branch(program_ctx, operand_storage, operand_storage_offset);
            if (PENES_STATUS_SUCCESS != status) {
                DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("branch failed. Status: %d\n", status);
                goto l_cleanup;
            }
        }

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    };


    inline enum PeNESStatus branch(
        ProgramContext *program_ctx,
        IStorageLocation *branch_operand,
        std::size_t operand_storage_offset = 0
    ) {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_address_t> *register_program_counter = nullptr;
        native_word_t register_program_counter_data = 0;
        native_address_t relative_branch_address = 0;

        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != branch_operand);

        /* Retrieve the current program counter. */
        register_program_counter = program_ctx->register_file.get_register_program_counter();
        register_program_counter_data = register_program_counter->get();

        /* Read relative branch address from operand. */
        status = branch_operand->read(
            &relative_branch_address,
            sizeof(relative_branch_address),
            operand_storage_offset
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read relative branch address failed. Status: %d\n", status);
            goto l_cleanup;
        }

        /* Add the relative offset to the program counter. */
        register_program_counter->set(register_program_counter_data + relative_branch_address);

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

private:
    const enum FlagRegisterBitMask branch_condition_mask = FLAG_REGISTER_BIT_MASK_NONE;
    const bool branch_on_set = false;
};


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
