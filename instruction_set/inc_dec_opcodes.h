/**
 * @brief  
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __INC_DEC_OPCODES_H__
#define __INC_DEC_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/* Increment memory by 1. */
class OpcodeINC : public IUpdateDataStatusOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return default_address_mode;
    }

    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *increment_operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Decrement memory by 1. */
class OpcodeDEC : public IUpdateDataStatusOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return default_address_mode;
    }

    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *decrement_operand_storage,
        std::size_t operand_storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Increment register X by 1. */
class OpcodeINX : public IUpdateDataStatusOpcode {
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

/* Decrement register X by 1. */
class OpcodeDEX : public IUpdateDataStatusOpcode {
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

/* Increment register Y by 1. */
class OpcodeINY : public IUpdateDataStatusOpcode {
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

/* Decrement register Y by 1. */
class OpcodeDEY : public IUpdateDataStatusOpcode {
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

#endif /* __INC_DEC_OPCODES_H__ */
