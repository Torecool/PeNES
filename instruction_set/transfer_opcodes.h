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
class ITransferOpcode : public IUpdateDataStatusOpcode, IImpliedOperandOpcode {
public:
    inline enum PeNESStatus transfer(
        ProgramContext *program_ctx,
        RegisterStorage<native_word_t> *src_register,
        RegisterStorage<native_word_t> *dest_register
    );
};

/* Transfer Accumulator to register X. */
class OpcodeTAX : public ITransferOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Transfer Accumulator to register Y. */
class OpcodeTAY : public ITransferOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Transfer register X to Accumulator. */
class OpcodeTXA : public ITransferOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Transfer register X to Stack pointer. */
class OpcodeTXS : public ITransferOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NONE;
};

/* Transfer Stack pointer to register X. */
class OpcodeTSX : public ITransferOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/* Transfer register Y to Accumulator. */
class OpcodeTYA : public ITransferOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

}

#endif /* __TRANSFER_OPCODES_H__ */
