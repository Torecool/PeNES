/**
 * @brief  
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __STORE_OPCODES_H__
#define __STORE_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/* Store Accumulator in memory. */
class OpcodeSTA : public IOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *store_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Store Index X in memory. */
class OpcodeSTX : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        /* If the address mode was predicted as zp,X, this should actually be zp,Y.
         * The reason for this is that the opcode's address mode bit pattern fits the otherwise zp,X template.
         * */
        if (address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED == default_address_mode) {
            return address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_Y_INDEXED;
        } else {
            return default_address_mode;
        }
    }

    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *store_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Store Index Y in memory. */
class OpcodeSTY : public IOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *store_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

}

#endif /* __STORE_OPCODES_H__ */
