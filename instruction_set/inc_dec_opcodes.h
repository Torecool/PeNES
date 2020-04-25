/**
 * @brief  Definitions for increment/decrement-related opcodes.
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
/** @brief Increment memory by 1. */
class OpcodeINC : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *increment_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Decrement memory by 1. */
class OpcodeDEC : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *decrement_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Increment register X by 1. */
class OpcodeINX : public IUpdateDataStatusOpcode, public IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Decrement register X by 1. */
class OpcodeDEX : public IUpdateDataStatusOpcode, public IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Increment register Y by 1. */
class OpcodeINY : public IUpdateDataStatusOpcode, public IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Decrement register Y by 1. */
class OpcodeDEY : public IUpdateDataStatusOpcode, public IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __INC_DEC_OPCODES_H__ */
