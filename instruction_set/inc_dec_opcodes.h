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
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *increment_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Decrement memory by 1. */
class OpcodeDEC : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *decrement_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Increment register X by 1. */
class OpcodeINX : public IUpdateDataStatusOpcode, IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Decrement register X by 1. */
class OpcodeDEX : public IUpdateDataStatusOpcode, IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Increment register Y by 1. */
class OpcodeINY : public IUpdateDataStatusOpcode, IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Decrement register Y by 1. */
class OpcodeDEY : public IUpdateDataStatusOpcode, IImpliedOperandOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

}

#endif /* __INC_DEC_OPCODES_H__ */
