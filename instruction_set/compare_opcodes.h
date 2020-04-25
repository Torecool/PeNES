/**
 * @brief  
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __COMPARE_OPCODES_H__
#define __COMPARE_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
class ICompareOpcode : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus compare(
        ProgramContext *program_ctx,
        RegisterStorage<native_word_t> *compare_register,
        IStorageLocation *compare_storage,
        std::size_t storage_offset
    );

protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE |
                                      REGISTER_STATUS_FLAG_MASK_CARRY |
                                      REGISTER_STATUS_FLAG_MASK_ZERO;
};

/* Compare data and Accumulator. */
class OpcodeCMP : public ICompareOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Compare data and register X. */
class OpcodeCPX : public ICompareOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Compare data and register Y. */
class OpcodeCPY : public ICompareOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

}

#endif /* __COMPARE_OPCODES_H__ */
