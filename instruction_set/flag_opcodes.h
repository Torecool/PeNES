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

/** Classes ***************************************************************/
/* Set Carry flag. */
class OpcodeSEC : public IUpdateStatusOpcode, IImpliedOperandOpcode {
private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_CARRY;
};


/* Clear Carry flag. */
class OpcodeCLC : public IUpdateStatusOpcode, IImpliedOperandOpcode {
private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_CARRY;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


/* Set Decimal flag. */
class OpcodeSED : public IUpdateStatusOpcode, IImpliedOperandOpcode {
private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_DECIMAL;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_DECIMAL;
};


/* Clear Decimal flag. */
class OpcodeCLD : public IUpdateStatusOpcode, IImpliedOperandOpcode {
private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_DECIMAL;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


/* Set Interrupt flag. */
class OpcodeSEI : public IUpdateStatusOpcode, IImpliedOperandOpcode {
private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
};


/* Clear Interrupt flag. */
class OpcodeCLI : public IUpdateStatusOpcode, IImpliedOperandOpcode {
private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


/* Clear Overflow flag. */
class OpcodeCLV : public IUpdateStatusOpcode, IImpliedOperandOpcode {
private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_OVERFLOW;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

}

#endif /* __FLAG_OPCODES_H__ */
