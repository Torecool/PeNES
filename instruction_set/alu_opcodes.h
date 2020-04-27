/**
 * @brief  Definitions for ALU-related opcodes.
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
#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing an addition operation using the Accumulator.
 *         Extends the standard data-status-updating opcode interface by adding the add method.
 */
class IAddOpcode : public IOpcode, public IUpdateDataStatusOperation {
protected:
    /** @brief          Add (with carry) a given WORD of data to the Accumulator and update the Status register accordingly.
     *
     *  @param[in]      program_ctx                 The program context containing the Accumulator to add to.
     *  @param[in]      add_operand                 The WORD of data to add.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus add(
        ProgramContext *program_ctx,
        native_word_t add_operand
    );

    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE |
                                      REGISTER_STATUS_FLAG_MASK_ZERO |
                                      REGISTER_STATUS_FLAG_MASK_CARRY |
                                      REGISTER_STATUS_FLAG_MASK_OVERFLOW;
};

/** @brief Add data to Accumulator with Carry. */
class OpcodeADC : public IAddOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Subtract data from Accumulator with Borrow (== ~Carry). */
class OpcodeSBC : public IAddOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __ALU_OPCODES_H__ */
