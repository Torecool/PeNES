/**
 * @brief  Definitions for transfer-related opcodes.
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
#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing a transfer operation.
 *         Extends the standard implied-data-status-updating opcode interface by adding the transfer method.
 * */
class ITransferOpcode : public IImpliedOperandOpcode, public IUpdateDataStatusOperation {
protected:
    /** @brief          Transfer a WORD of data from a source register to a destination register,
     *                  and update the Status register accordingly.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *  @param[in]      src_register                The source register containing the data to transfer.
     *  @param[in]      dest_register               The destination register to transfer the data to.
     *
     *  @return         Status indicating the success of the operation.
     * */
    enum PeNESStatus transfer(
        ProgramContext *program_ctx,
        RegisterStorage<native_word_t> *src_register,
        RegisterStorage<native_word_t> *dest_register
    );
};

/** @brief Transfer Accumulator to register X. */
class OpcodeTAX : public ITransferOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Transfer Accumulator to register Y. */
class OpcodeTAY : public ITransferOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Transfer register X to Accumulator. */
class OpcodeTXA : public ITransferOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Transfer register X to Stack pointer. */
class OpcodeTXS : public ITransferOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NONE;
};

/** @brief Transfer Stack pointer to register X. */
class OpcodeTSX : public ITransferOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Transfer register Y to Accumulator. */
class OpcodeTYA : public ITransferOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __TRANSFER_OPCODES_H__ */
