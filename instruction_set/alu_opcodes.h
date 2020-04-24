/**
 * @brief  
 * @author TBK
 * @date   22/04/2020
 * */

#ifndef __ALU_OPCODES_H__
#define __ALU_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
class OpcodeADC : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE |
                                REGISTER_STATUS_FLAG_MASK_ZERO |
                                REGISTER_STATUS_FLAG_MASK_CARRY |
                                REGISTER_STATUS_FLAG_MASK_OVERFLOW;
};


class OpcodeSBC : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE |
                                REGISTER_STATUS_FLAG_MASK_ZERO |
                                REGISTER_STATUS_FLAG_MASK_CARRY |
                                REGISTER_STATUS_FLAG_MASK_OVERFLOW;
};

}

#endif /* __ALU_OPCODES_H__ */
