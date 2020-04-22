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

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/

namespace instruction_set {

class ISetFlagOpcode : public IOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;
        native_word_t register_status_data = 0;

        ASSERT(nullptr != program_ctx);

        UNREFERENCED_PARAMETER(operand_storage);
        UNREFERENCED_PARAMETER(operand_storage_offset);

        /* Read contents of status register. */
        register_status = program_ctx->register_file.get_register_status();
        register_status_data = register_status->get();

        /* Write the status register back with the turned on bits in the mask set to 1. */
        register_status->set(register_status_data | set_flag_mask);

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    };

private:
    const enum FlagRegisterBitMask set_flag_mask = FLAG_REGISTER_BIT_MASK_NONE;
};


class IClearFlagOpcode : public IOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;
        native_word_t register_status_data = 0;

        ASSERT(nullptr != program_ctx);

        UNREFERENCED_PARAMETER(operand_storage);
        UNREFERENCED_PARAMETER(operand_storage_offset);

        /* Read contents of status register. */
        register_status = program_ctx->register_file.get_register_status();
        register_status_data = register_status->get();

        /* Write the status register back with the turned on bits in the mask set to 0. */
        register_status->set(register_status_data & ~clear_flag_mask);

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    };

private:
    const enum FlagRegisterBitMask clear_flag_mask = FLAG_REGISTER_BIT_MASK_NONE;
};


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
