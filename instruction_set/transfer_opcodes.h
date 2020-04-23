/**
 * @brief  
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __TRANSFER_OPCODES_H__
#define __TRANSFER_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/* Transfer Accumulator to register X. */
class OpcodeTAX : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Transfer Accumulator to register Y. */
class OpcodeTAY : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Transfer register X to Accumulator. */
class OpcodeTXA : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Transfer register X to Stack pointer. */
class OpcodeTXS : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NONE;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Transfer Stack pointer to register X. */
class OpcodeTSX : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Transfer register Y to Accumulator. */
class OpcodeTYA : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

}

#endif /* __TRANSFER_OPCODES_H__ */
