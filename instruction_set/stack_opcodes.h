/**
 * @brief  Definitions for stack-related opcodes.
 * @author TBK
 * @date   24/04/2020
 * */

#ifndef __STACK_OPCODES_H__
#define __STACK_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"
#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing a stack operation. */
class IStackOpcode : public IImpliedOperandOpcode, public IStackOperation {};

/** @brief Push Accumulator on Stack. */
class OpcodePHA : public IStackOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Push Status register on Stack. */
class OpcodePHP : public IStackOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Pull Accumulator from Stack. */
class OpcodePLA : public IStackOpcode, public IUpdateDataStatusOperation {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Pull Status register from Stack. */
class OpcodePLP : public IStackOpcode, public IUpdateStatusOperation {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    /* We will never set the Break flag in the status written to the register,
     * because it is only relevant to the status that is pushed onto the stack.
     * */
    const native_word_t update_mask = ~REGISTER_STATUS_FLAG_MASK_BREAK;

};

} /* namespace instruction_set */

#endif /* __STACK_OPCODES_H__ */
