/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __OPCODE_INTERFACE_H__
#define __OPCODE_INTERFACE_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "address_mode/address_mode.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
using namespace address_mode;

namespace instruction_set {

class IOpcode {
public:
    inline virtual AddressModeType resolve_address_mode(
        AddressModeType default_address_mode = AddressModeType::ADDRESS_MODE_TYPE_NONE
    ) {
        return default_address_mode;
    }

    inline virtual enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) {};
};


class IUpdateStatusOpcode : public IOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;

        ASSERT(nullptr != program_ctx);

        UNREFERENCED_PARAMETER(operand_storage);
        UNREFERENCED_PARAMETER(operand_storage_offset);

        /* Get the status register storage. */
        register_status = program_ctx->register_file.get_register_status();

        /* Call the method to update the status. */
        status = update_status(register_status);

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    };

protected:
    inline virtual enum PeNESStatus update_status(RegisterStorage<native_word_t> *register_status)
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        native_word_t status_flags = 0;
        native_word_t updated_status_flags = 0;

        ASSERT(nullptr != register_status);

        /* Read contents of the status register. */
        status_flags = register_status->read();

        /* Update the status register flags with the values from update_values.
         * A flag will only be updated if it is set in update_mask.
         * */
        updated_status_flags = (status_flags & ~update_mask) | update_values;

        /* Write the status register back. */
        register_status->write(updated_status_flags);

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    }

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NONE;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


class IUpdateDataStatusOpcode : public IUpdateStatusOpcode {
protected:
    inline enum PeNESStatus update_status(
        RegisterStorage<native_word_t> *register_status,
        native_dword_t opcode_result
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        bool is_zero = ((opcode_result << NATIVE_WORD_SIZE_BITS) == 0);
        bool did_carry = ((opcode_result >> NATIVE_WORD_SIZE_BITS) != 0);

        ASSERT(nullptr != register_status);

        update_values |= (opcode_result & REGISTER_STATUS_FLAG_MASK_NEGATIVE);
        update_values |= (true == is_zero)? REGISTER_STATUS_FLAG_MASK_ZERO: 0;
        update_values |= (true == did_carry)? REGISTER_STATUS_FLAG_MASK_CARRY: 0;

        /* Call the parent function to update the status flags. */
        status = IUpdateStatusOpcode::update_status(register_status);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    };

    inline enum PeNESStatus update_status(
        ProgramContext *program_ctx,
        native_dword_t opcode_result
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;

        ASSERT(nullptr != program_ctx);

        /* Retrieve the Status register from the program context. */
        register_status = program_ctx->register_file.get_register_status();

        /* Call the "real" update_status. */
        status = this->update_status(register_status, opcode_result);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("update_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    }

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


class IImpliedOperandOpcode : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_IMPLIED;
    }
};


class OpcodeNOP : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override
    {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_NONE;
    }
};

}

#endif /* __OPCODE_INTERFACE_H__ */
