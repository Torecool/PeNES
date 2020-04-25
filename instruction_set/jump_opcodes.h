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

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing a jump operation.
 *         Extends the standard opcode interface by adding the jump method.
 * */
class IJumpOpcode : public IOpcode {
protected:
    /** @brief          Perform a jump operation to a location specified by the jump address storage.
     *
     *  @param[in,out]  register_program_counter    The program counter register to update.
     *  @param[in]      jump_address_storage        The storage location containing the address to jump to.
     *  @param[in]      address_storage_offset      The offset within the storage location to read the address from.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus jump(
        RegisterStorage<native_dword_t> *register_program_counter,
        IStorageLocation *jump_address_storage,
        std::size_t address_storage_offset = 0
    );
};

/** @brief Jump to a new location.
 *
 *  @note  Although the address mode of this opcode is "Absolute",
 *         we actually set the Program counter to be the absolute address itself,
 *         and not the data pointed to by the absolute address (as opposed to any other absolute-addressed opcode).
 *         This effectively makes the address mode more like an immediate address mode of size double.
 * */
class OpcodeJMP : public IJumpOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return AddressModeType::ADDRESS_MODE_TYPE_IMMEDIATE_DOUBLE;
    }

    inline enum PeNESStatus exec(
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
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return AddressModeType::ADDRESS_MODE_TYPE_ABSOLUTE;
    }
};

/** @brief Jump and enter a subroutine, saving the return address.
 *
 *  @note  Although the address mode of this opcode is "Absolute",
 *         we actually set the Program counter to be the absolute address itself,
 *         and not the data pointed to by the absolute address (as opposed to any other absolute-addressed opcode).
 *         This effectively makes the address mode more like an immediate address mode of size double.
 * */
class OpcodeJSR : public IJumpOpcode, public IStackOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return AddressModeType::ADDRESS_MODE_TYPE_IMMEDIATE_DOUBLE;
    }

    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *jump_address_storage,
        std::size_t address_storage_offset
    ) override;
};

/** @brief Software interrupt. Save Stack pointer and Status, jump to interrupt handling routine. */
class OpcodeBRK : public IJumpOpcode, public IStackOpcode, public IUpdateStatusOpcode {
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
    /* Set the Interrupt flag on the status written back to the register,
     * to disable all maskable interrupt handling during the handling of the current interrupt.
     * Since this flag is not written to the status pushed on the stack,
     * after RTI/PLP, the flag will be cleared.
     * */
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
};

/** @brief Return from interrupt, restoring Status register and Program counter. */
class OpcodeRTI : public IStackOpcode, public IUpdateStatusOpcode {
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

/** @brief Return from subroutine, restoring Program counter. */
class OpcodeRTS : public IStackOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __JUMP_OPCODES_H__ */
