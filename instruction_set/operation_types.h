/**
 * @brief  
 * @author TBK
 * @date   27/04/2020
 * */

#ifndef __OPERATION_TYPES_H__
#define __OPERATION_TYPES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "system.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "address_mode/address_mode.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief General interface for all operations that modify the value of the Status register.
 *         A subclass wishing to update the status register performs the following operations:
 *         - Override via get_update_mask which flags are allowed to be modified. Only flags set in this variable will be modified.
 *         - Override via get_base_update_values the flag values to be set by any instance of the class regardless of the update_values parameter.
 *         - Set via
 *         - Call this->update_status to perform the update operation,
 *           passing update_values as the new values for the flags that are allowed to be modified.
 *           The rest of the flag values are ignored.
 * */
class IUpdateStatusOperation {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline virtual native_word_t get_update_mask() const
    {
        return REGISTER_STATUS_FLAG_MASK_NONE;
    }


    /** @brief Retrieve the base flag values to set in the modifiable flags of the Status register.
     *         The actual updated flag values will be equal to base_values | update_values.
     * */
    inline virtual native_word_t get_base_update_values() const
    {
        return REGISTER_STATUS_FLAG_MASK_NONE;
    }

    /** @brief          Update the Status register based on the mask of flags allowed to be modified (set through update_mask),
     *                  the base update values (set through base_values),
     *                  and the new values for those flags (set through update_values).
     *
     *  @param[in,out]  register_status             The Status register to update.
     *  @param[in]      update_values               The new flag values to set in the modifiable flags of the Status register.
     *                                              The rest of the flag values are ignored.
     *
     *  @return         Status indicating the success of the operation.
     * */
    enum PeNESStatus update_status(
        RegisterStorage<native_word_t> *register_status,
        native_word_t update_values = 0
    ) const;

    /** @brief          Utility wrapper for the update_status method that receives the Status register.
     *                  Retrieves the Status register from the program context and calls update_status.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *  @param[in]      update_values               The new flag values to set in the modifiable flags of the Status register.
     *                                              The rest of the flag values are ignored.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus update_status(
        ProgramContext *program_ctx,
        native_word_t update_values = 0
    ) const
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;

        ASSERT(nullptr != program_ctx);

        /* Retrieve the Status register from the program context. */
        register_status = program_ctx->register_file.get_register_status();

        /* Call the "real" update_status. */
        status = this->update_status(register_status, update_values);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("update_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }
};

/** @brief General interface for all operations that modify the value of the Status register based on the contents of given data.
 *         The interface extends the standard status-updating operation interface by adding the update_data_status method that
 *         receives a WORD of data and decides whether to modify the Negative and Zero status flags accordingly.
 * */
class IUpdateDataStatusOperation : public IUpdateStatusOperation {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    }

    /** @brief          Update the Status register based on the mask of flags allowed to be modified (set through update_mask)
     *                  the base update values (set through base_values),
     *                  the new values for those flags (set through update_values),
     *                  and the state of the parameter data.
     *
     *  @param[in,out]  register_status             The Status register to update.
     *  @param[in]      operation_result            The data to use for deciding which flags to update.
     *  @param[in]      update_values               The new flag values to set in the modifiable flags of the Status register.
     *                                              The rest of the flag values are ignored.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           This method is an extension of the superclass update_status method,
     *                  meaning that it still performs the functionality provided by the superclass method.
     *                  Flags set manually via update_values will not be overridden.
     * */
    inline enum PeNESStatus update_data_status(
        RegisterStorage<native_word_t> *register_status,
        native_word_t operation_result,
        native_word_t update_values = 0
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        bool is_negative = (operation_result & SYSTEM_NATIVE_WORD_SIGN_BIT_MASK);
        bool is_zero = ((operation_result << SYSTEM_NATIVE_WORD_SIZE_BITS) == 0);

        ASSERT(nullptr != register_status);

        update_values |= (true == is_negative)? REGISTER_STATUS_FLAG_MASK_NEGATIVE: 0;
        update_values |= (true == is_zero)? REGISTER_STATUS_FLAG_MASK_ZERO: 0;

        /* Call the parent function to update the status flags. */
        status = this->update_status(register_status, update_values);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    /** @brief          Utility wrapper for the update_data_status method that receives the Status register.
     *                  Retrieves the Status register from the program context and calls update_data_status.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *  @param[in]      operation_result            The data to use for deciding which flags to update.
     *  @param[in]      update_values               The new flag values to set in the modifiable flags of the Status register.
     *                                              The rest of the flag values are ignored.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus update_data_status(
        ProgramContext *program_ctx,
        native_word_t operation_result,
        native_word_t update_values = 0
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;

        ASSERT(nullptr != program_ctx);

        /* Retrieve the Status register from the program context. */
        register_status = program_ctx->register_file.get_register_status();

        /* Call the "real" update_data_status. */
        status = this->update_data_status(register_status, operation_result, update_values);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("update_data_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }
};


/** @brief General interface for all arithmetic operations that modify the value of the Status register based on the contents of given data.
 *         The interface extends the standard data-status-updating operation interface by adding the update_arithmetic_status method that
 *         receives a DWORD of data and decides whether to modify the Negative, Zero and Carry status flags accordingly.
 *         Note that evaluating the Overflow flag requires additional data, and so it must be set manually via update_values.
 * */
class IUpdateArithmeticStatusOperation : public IUpdateDataStatusOperation {
protected:
    /** @brief Retrieve the mask of status flags that are allowed to be modified in the Status register. */
    inline native_word_t get_update_mask() const override
    {
        return REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO | REGISTER_STATUS_FLAG_MASK_CARRY;
    }

    /** @brief          Update the Status register based on the mask of flags allowed to be modified (set through update_mask)
     *                  the base update values (set through base_values),
     *                  the new values for those flags (set through update_values),
     *                  and the state of the parameter data (including the carry data that exceeded the bounds of the word.
     *
     *  @param[in,out]  register_status             The Status register to update.
     *  @param[in]      operation_result            The data to use for deciding which flags to update,
     *                                              including the carry data that exceeded the bounds of the word.
     *  @param[in]      update_values               The new flag values to set in the modifiable flags of the Status register.
     *                                              The rest of the flag values are ignored.
     *  @param[in]      is_borrow                   Is operation being performed an operation with carry (i.e additions) or borrow (i.e subtractions)?
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           This method is an extension of the superclass update_data_status method,
     *                  meaning that it still performs the functionality provided by the superclass method.
     *                  Flags set manually via update_values will not be overridden.
     * */
    inline enum PeNESStatus update_arithmetic_status(
        RegisterStorage<native_word_t> *register_status,
        native_dword_t operation_result,
        native_word_t update_values = 0,
        bool is_borrow = false
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        bool did_exceed_bounds = ((operation_result >> SYSTEM_NATIVE_WORD_SIZE_BITS) != 0);

        ASSERT(nullptr != register_status);

        update_values |= (true == (did_exceed_bounds ^ is_borrow))? REGISTER_STATUS_FLAG_MASK_CARRY: 0;

        /* Call the parent function to update the status flags. */
        status = this->update_data_status(register_status, operation_result, update_values);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_data_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    /** @brief          Utility wrapper for the update_arithmetic_status method that receives the Status register.
     *                  Retrieves the Status register from the program context and calls update_data_status.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *  @param[in]      operation_result            The data to use for deciding which flags to update,
                                                    including the carry data that exceeded the bounds of the word.
     *  @param[in]      update_values               The new flag values to set in the modifiable flags of the Status register.
     *                                              The rest of the flag values are ignored.
     *  @param[in]      is_borrow                   Is operation being performed an operation with carry (i.e additions) or borrow (i.e subtractions)?
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus update_arithmetic_status(
        ProgramContext *program_ctx,
        native_dword_t operation_result,
        native_word_t update_values = 0,
        bool is_borrow = false
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_word_t> *register_status = nullptr;

        ASSERT(nullptr != program_ctx);

        /* Retrieve the Status register from the program context. */
        register_status = program_ctx->register_file.get_register_status();

        /* Call the "real" update_arithmetic_status. */
        status = this->update_arithmetic_status(
            register_status,
            operation_result,
            update_values,
            is_borrow
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("update_arithmetic_status failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    }
};


/** @brief General interface for all operations that modify the stack. */
class IStackOperation {
protected:
    /** @brief          Utility wrapper for the push method that receives data via a buffer.
     *                  Pushes a single WORD onto the stack.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to push onto.
     *  @param[in]      push_word                   The data word to push.
     *
     *  @return         Status indicating the success of the operation.
     * */
    static inline enum PeNESStatus push(ProgramContext *program_ctx, native_word_t push_word)
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

        ASSERT(nullptr != program_ctx);

        /* Call the "real" push implementation with the data word to push. */
        status = push(
            program_ctx,
            &push_word,
            SYSTEM_NATIVE_WORD_NUM_WORDS
            );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("push failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    /** @brief          Utility wrapper for the push method that receives data via a buffer.
     *                  Pushes a single DWORD address onto the stack.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to push onto.
     *  @param[in]      push_address                The dword address to push, in big endian format.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           Since the native machine employs little endianness in memory,
     *                  addresses must be converted to little endian before pushing.
     * */
    static inline enum PeNESStatus push(ProgramContext *program_ctx, native_address_t push_address)
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        native_address_t converted_push_address = system_host_to_native_endianness(push_address);

        ASSERT(nullptr != program_ctx);

        /* Call the "real" push implementation with the address to push, after it has been byteswapped. */
        status = push(
            program_ctx,
            reinterpret_cast<native_word_t *>(&converted_push_address),
            SYSTEM_NATIVE_ADDRESS_NUM_WORDS
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("push failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    /** @brief          Utility wrapper for the pull method that receives data via a buffer.
     *                  Pulls a single WORD from the stack.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to pull from.
     *  @param[out]     output_pull_word            The pulled data word.
     *
     *  @return         Status indicating the success of the operation.
     * */
    static inline enum PeNESStatus pull(ProgramContext *program_ctx, native_word_t *output_pull_word)
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        native_word_t pull_data = 0;

        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != output_pull_word);

        /* Call the "real" pull implementation with a buffer to contain the data word being pulled. */
        status = pull(
            program_ctx,
            &pull_data,
            SYSTEM_NATIVE_WORD_NUM_WORDS
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("pull failed. Status: %d", status);
            goto l_cleanup;
        }

        *output_pull_word = pull_data;

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    /** @brief          Utility wrapper for the pull method that receives data via a buffer.
     *                  Pulls a single DWORD address from the stack.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to pull from.
     *  @param[out]     output_pull_address         The pulled dword address, in big endian format.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           Since the native machine employs little endianness in memory,
     *                  addresses must be converted to big endian after pulling.
     * */
    static inline enum PeNESStatus pull(ProgramContext *program_ctx, native_address_t *output_pull_address)
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        native_address_t pull_address = 0;
        native_address_t converted_pull_address = 0;

        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != output_pull_address);

        /* Call the "real" pull implementation with a buffer to contain the address being pulled. */
        status = pull(
            program_ctx,
            reinterpret_cast<native_word_t *>(&pull_address),
            SYSTEM_NATIVE_ADDRESS_NUM_WORDS
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("pull failed. Status: %d", status);
            goto l_cleanup;
        }

        /* Convert the address from its storage endianness, little endian, to big endian. */
        converted_pull_address = system_native_to_host_endianness(pull_address);

        *output_pull_address = converted_pull_address;

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

private:
    /** @brief          Push a buffer of data onto the stack as-is (i.e. without endianness conversions).
     *                  Write the data at the current address of the stack pointer and decrement the stack pointer by the size of the data.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to push onto.
     *  @param[in]      push_data                   A buffer containing the data to push.
     *  @param[in]      num_push_words              The size of the data to push in native words.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           The stack grows top-down (meaning a lower address is higher on the stack).
     * */
    static enum PeNESStatus push(
        ProgramContext *program_ctx,
        const native_word_t *push_data,
        std::size_t num_push_words
    );

    /** @brief          Pull data from the stack into a buffer as-is (i.e. without endianness conversions).
     *                  Increment the stack pointer by the size of the data and read the data at the current address of the stack pointer.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to pull from.
     *  @param[in,out]  pull_buffer                 A buffer to write the pulled data to.
     *  @param[in]      num_pull_words              The size of the data to pull in native words.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           The stack grows top-down (meaning a lower address is higher on the stack).
     * */
    static enum PeNESStatus pull(
        ProgramContext *program_ctx,
        native_word_t *pull_buffer,
        std::size_t num_pull_words
    );
};

/** @brief General interface for all operations that modify the program counter (i.e. perform a jump). */
class IJumpOperation {
protected:
    /** @brief          Perform a jump operation to a location specified by the jump address storage.
     *
     *  @param[in,out]  register_program_counter    The program counter register to update.
     *  @param[in]      jump_address_storage        The storage location containing the address to jump to.
     *  @param[in]      address_storage_offset      The offset within the storage location to read the address from.
     *
     *  @return         Status indicating the success of the operation.
     * */
    static enum PeNESStatus jump(
        RegisterStorage<native_dword_t> *register_program_counter,
        IStorageLocation *jump_address_storage,
        std::size_t address_storage_offset = 0
    );

    /** @brief          Utility wrapper for the jump method that receives a program context containing the Program counter register.
     *                  Perform a jump operation to a location specified by the jump address storage.
     *
     *  @param[in]      program_ctx                 The program context containing the program counter register to update.
     *  @param[in]      jump_address_storage        The storage location containing the address to jump to.
     *  @param[in]      address_storage_offset      The offset within the storage location to read the address from.
     *
     *  @return         Status indicating the success of the operation.
     * */
    static inline enum PeNESStatus jump(
        ProgramContext *program_ctx,
        IStorageLocation *jump_address_storage,
        std::size_t address_storage_offset = 0
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage<native_address_t> *register_program_counter = nullptr;

        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != jump_address_storage);

        /* Retrieve the Program counter register from the program context. */
        register_program_counter = program_ctx->register_file.get_register_program_counter();

        /* Call the "real" jump implementation. */
        status = jump(
            register_program_counter,
            jump_address_storage,
            address_storage_offset
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("jump failed. Status: %d", status);
            goto l_cleanup;
        }

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }
};

} /* namespace instruction_set */

#endif /* __OPERATION_TYPES_H__ */
