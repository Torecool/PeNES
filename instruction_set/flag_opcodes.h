/**
 * @brief  Definitions for status flag-related opcodes.
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
#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an implied operand flag-modification opcode. */
class IFlagOpcode : public IImpliedOperandOpcode, public IUpdateStatusOperation {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Set Carry flag. */
class OpcodeSEC : public IFlagOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
    const native_word_t base_values = REGISTER_STATUS_FLAG_MASK_CARRY;
};


/** @brief Clear Carry flag. */
class OpcodeCLC : public IFlagOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
};


/** @brief Set Decimal flag. */
class OpcodeSED : public IFlagOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_DECIMAL;
    const native_word_t base_values = REGISTER_STATUS_FLAG_MASK_DECIMAL;
};


/** @brief Clear Decimal flag. */
class OpcodeCLD : public IFlagOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_DECIMAL;
};


/** @brief Set Interrupt flag. */
class OpcodeSEI : public IFlagOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    const native_word_t base_values = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
};


/** @brief Clear Interrupt flag. */
class OpcodeCLI : public IFlagOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
};


/** @brief Clear Overflow flag. */
class OpcodeCLV : public IFlagOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_OVERFLOW;
};

} /* namespace instruction_set */

#endif /* __FLAG_OPCODES_H__ */
