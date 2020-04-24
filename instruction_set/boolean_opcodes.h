/**
 * @brief  
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
class IBooleanOpcode : public IUpdateDataStatusOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t storage_offset
    ) override;

protected:
    inline virtual native_word_t operation(
        native_word_t register_a_data,
        native_word_t storage_data
    ) = 0;
};

/* AND data and Accumulator. */
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

/* XOR data and register X. */
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

/* OR data and register Y. */
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

/* Test bits in memory with Accumulator. */
class OpcodeBIT : public OpcodeAND {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t storage_offset
    ) override;

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};
}

#endif /* __BOOLEAN_OPCODES_H__ */
