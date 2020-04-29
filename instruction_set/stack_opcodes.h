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
/** @brief Push Accumulator on Stack. */
class OpcodePHA : public IImpliedOperandOpcode, public IStackOperation {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Push Status register on Stack. */
class OpcodePHP : public IImpliedOperandOpcode, public IStackOperation {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Pull Accumulator from Stack. */
class OpcodePLA :
    public IImpliedOperandOpcode,
    public IStackOperation,
    public IUpdateDataStatusOperation {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Pull Status register from Stack. */
class OpcodePLP :
    public IImpliedOperandOpcode,
    public IStackOperation,
    public IUpdateStatusOperation {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register.
     *  @note  We will never set the Break flag in the status written to the register,
     *         because it is only relevant to the status that is pushed onto the stack.
     * */
    inline native_word_t get_update_mask() const override
    {
        return ~REGISTER_STATUS_FLAG_MASK_BREAK;
    }

};

} /* namespace instruction_set */

#endif /* __STACK_OPCODES_H__ */
