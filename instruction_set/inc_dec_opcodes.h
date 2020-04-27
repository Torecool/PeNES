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
#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode that performs an increment/decrement operation on memory. */
class IMemoryIncrementingOpcode : public IOpcode, public IUpdateDataStatusOperation {};

/** @brief Interface of an opcode that performs an increment/decrement operation on a register. */
class IRegisterIncrementingOpcode : public IImpliedOperandOpcode, public IUpdateDataStatusOperation {};

/** @brief Increment memory by 1. */
class OpcodeINC : public IMemoryIncrementingOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *increment_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Decrement memory by 1. */
class OpcodeDEC : public IMemoryIncrementingOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *decrement_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Increment register X by 1. */
class OpcodeINX : public IRegisterIncrementingOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Decrement register X by 1. */
class OpcodeDEX : public IRegisterIncrementingOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Increment register Y by 1. */
class OpcodeINY : public IRegisterIncrementingOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Decrement register Y by 1. */
class OpcodeDEY : public IRegisterIncrementingOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __INC_DEC_OPCODES_H__ */
