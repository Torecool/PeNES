/**
 * @brief  General opcode interfaces and definitions.
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

/** Namespaces ************************************************************/
using namespace address_mode;

namespace instruction_set {

/** Classes ***************************************************************/
/** @brief General interface for all instruction set opcodes.
 *         The interface includes a method for resolving the opcode's address mode based on the predicted address mode,
 *         and a method for executing the opcode instruction.
 * */
class IOpcode {
public:
    /** @brief          Given a default address mode predicted from the decode tables,
     *                  each opcode may decide to use the predicted address mode or override it in special cases (such as LDX and STX).
     *
     *  @param[in]      default_address_mode        The predicted, default address mode.
     *
     *  @return         The resolved address mode that should be used.
     * */
    inline virtual AddressModeType resolve_address_mode(
        AddressModeType default_address_mode = AddressModeType::ADDRESS_MODE_TYPE_NONE
    )
    {
        return default_address_mode;
    }

    /** @brief          Execute the opcode instruction.
     *
     *  @param[in]      program_ctx                 The context object representing the program instance.
     *  @param[in]      operand_storage             The generic "storage location" resolved from the address mode.
     *                                              May represent a memory location, register, immediate, etc.
     *  @param[in]      operand_storage_offset      The offset within the storage location operand_storage that contains the data.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline virtual enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) {};
};

/** @brief General interface for all instruction set opcodes that modify the value of the Status register.
 *         The interface extends the standard opcode interface by adding the protected update_status method.
 *         A subclass wishing to update the status register should perform the following operations:
 *         - Declare via the update_mask which flags are allowed to be modified. Only flags set in this variable will be modified.
 *         - Set via update_values the new values for the flags that are allowed to be modified. The rest of the flag values are ignored.
 *         - Call this->update_status to perform the update operation.
 * */
class IUpdateStatusOpcode : public IOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    /** @brief          Update the Status register based on the mask of flags allowed to be modified (set through update_mask),
     *                  and the new values for those flags (set through update_values).
     *
     *  @param[in,out]  register_status             The Status register to update.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus update_status(RegisterStorage<native_word_t> *register_status);

    /** @brief          Utility wrapper for the update_status method that receives the Status register.
     *                  Retrieves the Status register from the program context and calls update_status.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus update_status(ProgramContext *program_ctx);

    /** @brief The mask of status flags that are allowed to be modified in the Status register. */
    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NONE;

    /** @brief The new flag values to set in the modifiable flags of the Status register. The rest of the flag values are ignored. */
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};

/** @brief General interface for all instruction set opcodes that modify the value of the Status register based on the contents of given data.
 *         The interface extends the standard status-updating opcode interface by overloading the update_status method with a variant that
 *         receives a DWORD of data and decides which status flags to update based on the state of the data.
 *         The supported data flags to be decided on by the method are the Negative, Zero and Carry flags.
 *         Since the Overflow flag requires more data for the decision, it must be manually updated by the opcode subclass.
 * */
class IUpdateDataStatusOpcode : public IUpdateStatusOpcode {
protected:
    /** @brief          Update the Status register based on the mask of flags allowed to be modified (set through update_mask)
     *                  the new values for those flags (set through update_values),
     *                  and the state of the parameter data.
     *
     *  @param[in,out]  register_status             The Status register to update.
     *  @param[in]      opcode_result               The data to use for deciding which flags to update.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           This method is an extension of the superclass update_status method,
     *                  meaning that it still performs the functionality provided by the superclass method.
     *                  Flag values set manually via update_values will not be overridden.
     * */
    inline enum PeNESStatus update_status(
        RegisterStorage<native_word_t> *register_status,
        native_dword_t opcode_result
    );

    /** @brief          Utility wrapper for the update_status method that receives the Status register.
     *                  Retrieves the Status register from the program context and calls update_status.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *  @param[in]      opcode_result               The data to use for deciding which flags to update.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus update_status(
        ProgramContext *program_ctx,
        native_dword_t opcode_result
    );

    const native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
};

/** @brief General interface for all instruction set opcodes with an implied-only address mode.
 *         The interface overrides the superclass address mode resolution method to always return the implied address mode.
 * */
class IImpliedOperandOpcode : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return AddressModeType::ADDRESS_MODE_TYPE_IMPLIED;
    }
};

/** @brief General interface for all instruction set opcodes that perform stack operations.
 *         The interface extends the standard opcode interface by adding the push and pull operations.
 * */
class IStackOpcode : public IImpliedOperandOpcode {
protected:
    /** @brief          Utility wrapper for the push method that receives data via a buffer.
     *                  Pushes a single WORD onto the stack.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to push onto.
     *  @param[in]      push_data                   The data word to push.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus push(ProgramContext *program_ctx, native_word_t push_data);

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
    inline enum PeNESStatus push(ProgramContext *program_ctx, native_address_t push_address);

    /** @brief          Utility wrapper for the pull method that receives data via a buffer.
     *                  Pulls a single WORD from the stack.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to pull from.
     *  @param[out]     output_pull_word            The pulled data word.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline enum PeNESStatus pull(ProgramContext *program_ctx, native_word_t *output_pull_word);

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
    inline enum PeNESStatus pull(ProgramContext *program_ctx, native_address_t *output_pull_address);

private:
    /** @brief          Push a buffer of data onto the stack as-is (i.e. without endianness conversions).
     *                  Write the data at the current address of the stack pointer and decrement the stack pointer by the size of the data.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to push onto.
     *  @param[in]      push_data                   A buffer containing the data to push.
     *  @param[in]      push_size                   The size of the data to push.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           The stack grows top-down (meaning a lower address is higher on the stack).
     * */
    inline enum PeNESStatus push(
        ProgramContext *program_ctx,
        void *push_data,
        std::size_t push_size
    );

    /** @brief          Pull data from the stack into a buffer as-is (i.e. without endianness conversions).
     *                  Increment the stack pointer by the size of the data and read the data at the current address of the stack pointer.
     *
     *  @param[in]      program_ctx                 The program context containing the stack to pull from.
     *  @param[in,out]  pull_buffer                 A buffer to write the pulled data to.
     *  @param[in]      pull_size                   The size of the data to pull.
     *
     *  @return         Status indicating the success of the operation.
     *
     *  @note           The stack grows top-down (meaning a lower address is higher on the stack).
     * */
    inline enum PeNESStatus pull(
        ProgramContext *program_ctx,
        void *pull_buffer,
        std::size_t pull_size
    );
};

/** @brief Perform no operation. */
class OpcodeNOP : public IImpliedOperandOpcode {};

} /* namespace instruction_set */

#endif /* __OPCODE_INTERFACE_H__ */
