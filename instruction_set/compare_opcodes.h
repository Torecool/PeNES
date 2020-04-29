/**
 * @brief  Definitions for comparison-related opcodes.
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
#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing a comparison operation.
 *         Extends the standard data-status-updating opcode interface by adding the compare method.
 * */
class ICompareOpcode : public IOpcode, public IUpdateArithmeticStatusOperation {
protected:
    /** @brief          Perform a compare operation between a register operand and a generic storage location operand.
     *                  Subtract the second operand from the first and update the Status register's data flags accordingly.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *  @param[in]      compare_register            The first, register operand.
     *  @param[in]      compare_storage             The second, generic storage location operand.
     *  @param[in]      storage_offset              The offset within the storage location to read the data from.
     *
     *  @return         Status indicating the success of the operation.
     * */
    enum PeNESStatus compare(
        ProgramContext *program_ctx,
        RegisterStorage<native_word_t> *compare_register,
        IStorageLocation *compare_storage,
        std::size_t storage_offset = 0
    );
};

/** @brief Compare Accumulator with data (A - M). */
class OpcodeCMP : public ICompareOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Compare register X with data (X - M). */
class OpcodeCPX : public ICompareOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Compare register Y with data (Y - M). */
class OpcodeCPY : public ICompareOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __COMPARE_OPCODES_H__ */
