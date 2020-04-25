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

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Set Carry flag. */
class OpcodeSEC : public IUpdateStatusOpcode, public IImpliedOperandOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_CARRY;
};


/** @brief Clear Carry flag. */
class OpcodeCLC : public IUpdateStatusOpcode, public IImpliedOperandOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


/** @brief Set Decimal flag. */
class OpcodeSED : public IUpdateStatusOpcode, public IImpliedOperandOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_DECIMAL;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_DECIMAL;
};


/** @brief Clear Decimal flag. */
class OpcodeCLD : public IUpdateStatusOpcode, public IImpliedOperandOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_DECIMAL;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


/** @brief Set Interrupt flag. */
class OpcodeSEI : public IUpdateStatusOpcode, public IImpliedOperandOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
};


/** @brief Clear Interrupt flag. */
class OpcodeCLI : public IUpdateStatusOpcode, public IImpliedOperandOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


/** @brief Clear Overflow flag. */
class OpcodeCLV : public IUpdateStatusOpcode, public IImpliedOperandOpcode {
protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_OVERFLOW;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

} /* namespace instruction_set */

#endif /* __FLAG_OPCODES_H__ */
