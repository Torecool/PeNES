/**
 * @brief  
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
class IJumpOpcode : public IOpcode {
protected:
    inline enum PeNESStatus jump(
        RegisterStorage<native_dword_t> *register_program_counter,
        IStorageLocation *jump_address_storage,
        std::size_t address_storage_offset = 0
    );
};

/* Jump to a new location.
 * Note: Although the address mode of this opcode is "Absolute",
 * we actually set the Program counter to be the absolute address itself,
 * and not the data pointed to by the absolute address (as opposed to any other absolute-addressed opcode).
 * This effectively makes the address mode more like an immediate address mode of size double.
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

/* Jump to a new location, in indirect address mode.
 * Note: The two address modes of the JMP opcode, absolute and indirect,
 * must be separated because they feature the same address mode encoding.
 * Note: Although the address mode of this opcode is "Indirect",
 * we actually set the Program counter to be the direct address itself (meaning the address pointed to by the indirect address),
 * and not the data pointed to by the direct address (meaning the data pointed to by the direct address which is pointed to by the indirect address).
 * This effectively makes the address mode more like an absolute address mode.
 * */
class OpcodeIndirectJMP : public OpcodeJMP {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return AddressModeType::ADDRESS_MODE_TYPE_ABSOLUTE;
    }
};

/* Jump and enter a subroutine, saving the return address.
 * Note: Although the address mode of this opcode is "Absolute",
 * we actually set the Program counter to be the absolute address itself,
 * and not the data pointed to by the absolute address (as opposed to any other absolute-addressed opcode).
 * This effectively makes the address mode more like an immediate address mode of size double.
 * */
class OpcodeJSR : public IJumpOpcode, IStackOpcode {
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

/* Software interrupt. Save Stack pointer and Status, jump to interrupt handling routine. */
class OpcodeBRK : public IJumpOpcode, IStackOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Return from interrupt, restoring Status register and Program counter. */
class OpcodeRTI : public IStackOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Return from subroutine, restoring Program counter. */
class OpcodeRTS : public IStackOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

}

#endif /* __JUMP_OPCODES_H__ */
