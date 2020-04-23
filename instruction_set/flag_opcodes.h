/**
 * @brief  
 * @author TBK
 * @date   22/04/2020
 * */

#ifndef __FLAG_OPCODES_H__
#define __FLAG_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Interfaces ************************************************************/
class ISetFlagOpcode : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(
        AddressModeType default_address_mode = AddressModeType::ADDRESS_MODE_TYPE_NONE
    ) override {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_IMPLIED;
    }

    inline enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    const enum FlagRegisterBitMask set_flag_mask = FLAG_REGISTER_BIT_MASK_NONE;
};


class IClearFlagOpcode : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(
        AddressModeType default_address_mode = AddressModeType::ADDRESS_MODE_TYPE_NONE
    ) override {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_IMPLIED;
    }

    inline enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    const enum FlagRegisterBitMask clear_flag_mask = FLAG_REGISTER_BIT_MASK_NONE;
};

/** Classes ***************************************************************/
/* Set Carry flag. */
class OpcodeSEC : public ISetFlagOpcode {
private:
    const enum FlagRegisterBitMask set_flag_mask = FLAG_REGISTER_BIT_MASK_CARRY;
};


/* Clear Carry flag. */
class OpcodeCLC : public IClearFlagOpcode {
private:
    const enum FlagRegisterBitMask clear_flag_mask = FLAG_REGISTER_BIT_MASK_CARRY;
};


/* Set Decimal flag. */
class OpcodeSED : public ISetFlagOpcode {
private:
    const enum FlagRegisterBitMask set_flag_mask = FLAG_REGISTER_BIT_MASK_DECIMAL;
};


/* Clear Decimal flag. */
class OpcodeCLD : public IClearFlagOpcode {
private:
    const enum FlagRegisterBitMask clear_flag_mask = FLAG_REGISTER_BIT_MASK_DECIMAL;
};


/* Set Interrupt flag. */
class OpcodeSEI : public ISetFlagOpcode {
private:
    const enum FlagRegisterBitMask set_flag_mask = FLAG_REGISTER_BIT_MASK_INTERRUPT;
};


/* Clear Interrupt flag. */
class OpcodeCLI : public IClearFlagOpcode {
private:
    const enum FlagRegisterBitMask clear_flag_mask = FLAG_REGISTER_BIT_MASK_INTERRUPT;
};


/* Clear Overflow flag. */
class OpcodeCLV : public IClearFlagOpcode {
private:
    const enum FlagRegisterBitMask clear_flag_mask = FLAG_REGISTER_BIT_MASK_OVERFLOW;
};

}

#endif /* __FLAG_OPCODES_H__ */
