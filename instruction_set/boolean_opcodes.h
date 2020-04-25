/**
 * @brief  Definitions for boolean-related opcodes.
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __BOOLEAN_OPCODES_H__
#define __BOOLEAN_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing a boolean operation using the Accumulator.
 *         Each subclass implements the operation method for performing various boolean operations (AND, OR, etc.),
 *         which is then invoked by the interface's exec method.
 * */
class IBooleanOpcode : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t storage_offset
    ) override;

protected:
    /** @brief          Perform a boolean operation on the data of register A and the data from the storage operand.
     *                  The return value is the new value of register A.
     *
     *  @param[in]      register_a_data             The data of register A.
     *  @param[in]      storage_data                The data from the storage operand.
     *
     *  @return         The result of the boolean operation, to be written back to register A.
     * */
    inline virtual native_word_t operation(
        native_word_t register_a_data,
        native_word_t storage_data
    ) = 0;
};

/** @brief AND data and Accumulator. */
class OpcodeAND : public IBooleanOpcode {
protected:
    inline native_word_t operation(
        native_word_t register_a_data,
        native_word_t storage_data
    ) override
    {
        return register_a_data & storage_data;
    };
};

/** @brief XOR data and register X. */
class OpcodeEOR : public IBooleanOpcode {
protected:
    inline native_word_t operation(
        native_word_t register_a_data,
        native_word_t storage_data
    ) override
    {
        return register_a_data ^ storage_data;
    };
};

/** @brief OR data and register Y. */
class OpcodeORA : public IBooleanOpcode {
protected:
    inline native_word_t operation(
        native_word_t register_a_data,
        native_word_t storage_data
    ) override
    {
        return register_a_data | storage_data;
    };
};

/** @brief Test bits in memory with Accumulator.
 *         Performs an AND operation between the data operand and the Accumulator and discards the result.
 *         The Zero flag is set based on the result of the AND operation,
 *         while bit 6 and 7 of the data operand are copied to the Status register's Overflow and Negative flags respectively.
 * */
class OpcodeBIT : public OpcodeAND {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t storage_offset
    ) override;

protected:
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE |
                                      REGISTER_STATUS_FLAG_MASK_ZERO |
                                      REGISTER_STATUS_FLAG_MASK_OVERFLOW;
};

} /* namespace instruction_set */

#endif /* __BOOLEAN_OPCODES_H__ */
