/**
 * @brief  Definitions for jump-related opcodes.
 * @author TBK
 * @date   24/04/2020
 * */

#ifndef __JUMP_OPCODES_H__
#define __JUMP_OPCODES_H__

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
/** @brief Jump to a new location.
 *
 *  @note  Although the address mode of this opcode is "Absolute",
 *         we actually set the Program counter to be the absolute address itself,
 *         and not the data pointed to by the absolute address (as opposed to any other absolute-addressed opcode).
 *         This effectively makes the address mode more like an immediate address mode of size double.
 * */
class OpcodeJMP : public IOpcode, public IJumpOperation {
public:
    inline address_mode::AddressModeType resolve_address_mode(
        address_mode::AddressModeType default_address_mode
    ) override
    {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_IMMEDIATE_DOUBLE;
    }

    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *jump_address_storage,
        std::size_t address_storage_offset
    ) override;
};

/** @brief Jump to a new location, in indirect address mode.
 *
 *  @note  The two address modes of the JMP opcode, absolute and indirect,
 *         must be separated because they feature the same address mode encoding.
 *  @note  Although the address mode of this opcode is "Indirect",
 *         we actually set the Program counter to be the direct address itself (meaning the address pointed to by the indirect address),
 *         and not the data pointed to by the direct address (meaning the data pointed to by the direct address which is pointed to by the indirect address).
 *         This effectively makes the address mode more like an absolute address mode.
 * */
class OpcodeIndirectJMP : public OpcodeJMP {
public:
    inline address_mode::AddressModeType resolve_address_mode(
        address_mode::AddressModeType default_address_mode
    ) override
    {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_ABSOLUTE;
    }
};

/** @brief Jump and enter a subroutine, saving the return address.
 *
 *  @note  Although the address mode of this opcode is "Absolute",
 *         we actually set the Program counter to be the absolute address itself,
 *         and not the data pointed to by the absolute address (as opposed to any other absolute-addressed opcode).
 *         This effectively makes the address mode more like an immediate address mode of size double.
 * */
class OpcodeJSR : public IOpcode, public IJumpOperation, public IStackOperation {
public:
    inline address_mode::AddressModeType resolve_address_mode(
        address_mode::AddressModeType default_address_mode
    ) override
    {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_IMMEDIATE_DOUBLE;
    }

    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *jump_address_storage,
        std::size_t address_storage_offset
    ) override;
};

/** @brief Software interrupt. Save Stack pointer and Status, jump to interrupt handling routine. */
class OpcodeBRK :
    public IImpliedOperandOpcode,
    public IInterruptOperation {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Return from interrupt, restoring Status register and Program counter. */
class OpcodeRTI :
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

/** @brief Return from subroutine, restoring Program counter. */
class OpcodeRTS : public IImpliedOperandOpcode, public IStackOperation {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __JUMP_OPCODES_H__ */
