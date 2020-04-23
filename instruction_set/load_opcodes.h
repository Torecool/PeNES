/**
 * @brief  
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __LOAD_OPCODES_H__
#define __LOAD_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
class ILoadOpcode : public IUpdateDataStatusOpcode {
public:
    enum PeNESStatus load(
        ProgramContext *program_ctx,
        RegisterStorage<native_word_t> *load_register,
        IStorageLocation *load_storage,
        std::size_t storage_offset
    );
};

/* Load Accumulator with Data. */
class OpcodeLDA : public ILoadOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Load register X with Data. */
class OpcodeLDX : public ILoadOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        /* If the address mode was predicted as zp,X, this should actually be zp,Y.
         * Also, if the address mode was predicted as abs,X, this should actually be abs,Y.
         * The reason for this is that the opcode's address mode bit pattern fits the otherwise zp,X/abs,X templates.
         * */
        if (address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED == default_address_mode) {
            return address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_Y_INDEXED;
        } else if (address_mode::AddressModeType::ADDRESS_MODE_TYPE_ABSOLUTE_X_INDEXED == default_address_mode) {
            return address_mode::AddressModeType::ADDRESS_MODE_TYPE_ABSOLUTE_Y_INDEXED;
        } else {
            return default_address_mode;
        }
    }

    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Load register Y with Data. */
class OpcodeLDY : public ILoadOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

}

#endif /* __LOAD_OPCODES_H__ */
