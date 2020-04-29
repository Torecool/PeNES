/**
 * @brief  Definitions for status flag-related opcodes.
 * @author TBK
 * @date   22/04/2020
 * */

#ifndef __FLAG_OPCODES_H__
#define __FLAG_OPCODES_H__

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
/** @brief Interface of an implied operand flag-modification opcode. */
class IFlagOpcode : public IImpliedOperandOpcode, public IUpdateStatusOperation {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

        ASSERT(nullptr != program_ctx);

        UNREFERENCED_PARAMETER(operand_storage);
        UNREFERENCED_PARAMETER(operand_storage_offset);

        /* Call the method to update the status. */
        status = this->update_status(program_ctx);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("update_data_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }
};

/** @brief Set Carry flag. */
class OpcodeSEC : public IFlagOpcode {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_CARRY;
    }

    /** @brief Retrieve the base flag values to set in the modifiable flags of the Status register.
     *         The actual updated flag values will be equal to base_values | update_values.
     * */
    inline native_word_t get_base_update_values() const override
    {
        return REGISTER_STATUS_FLAG_MASK_CARRY;
    }
};


/** @brief Clear Carry flag. */
class OpcodeCLC : public IFlagOpcode {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_CARRY;
    }
};


/** @brief Set Decimal flag. */
class OpcodeSED : public IFlagOpcode {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_DECIMAL;
    }

    /** @brief Retrieve the base flag values to set in the modifiable flags of the Status register.
     *         The actual updated flag values will be equal to base_values | update_values.
     * */
    inline native_word_t get_base_update_values() const override
    {
        return REGISTER_STATUS_FLAG_MASK_DECIMAL;
    }
};


/** @brief Clear Decimal flag. */
class OpcodeCLD : public IFlagOpcode {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_DECIMAL;
    }
};


/** @brief Set Interrupt flag. */
class OpcodeSEI : public IFlagOpcode {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    }

    /** @brief Retrieve the base flag values to set in the modifiable flags of the Status register.
     *         The actual updated flag values will be equal to base_values | update_values.
     * */
    inline native_word_t get_base_update_values() const override
    {
        return REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    }
};


/** @brief Clear Interrupt flag. */
class OpcodeCLI : public IFlagOpcode {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_INTERRUPT;
    }
};


/** @brief Clear Overflow flag. */
class OpcodeCLV : public IFlagOpcode {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_OVERFLOW;
    }
};

} /* namespace instruction_set */

#endif /* __FLAG_OPCODES_H__ */
