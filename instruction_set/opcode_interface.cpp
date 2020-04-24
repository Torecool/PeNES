/**
 * @brief  
 * @author TBK
 * @date   24/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"
#include "system.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IUpdateStatusOpcode::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_status = nullptr;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get the status register storage. */
    register_status = program_ctx->register_file.get_register_status();

    /* Call the method to update the status. */
    status = this->update_status(register_status);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IUpdateStatusOpcode::update_status(RegisterStorage<native_word_t> *register_status)
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


enum PeNESStatus IUpdateDataStatusOpcode::update_status(
    RegisterStorage<native_word_t> *register_status,
    native_dword_t opcode_result
)

{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    bool is_zero = ((opcode_result << SYSTEM_NATIVE_WORD_SIZE_BITS) == 0);
    bool did_carry = ((opcode_result >> SYSTEM_NATIVE_WORD_SIZE_BITS) != 0);

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
}


enum PeNESStatus IUpdateDataStatusOpcode::update_status(
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


enum PeNESStatus IStackOpcode::push(
    ProgramContext *program_ctx,
    void *push_data,
    std::size_t push_size
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_stack_pointer = nullptr;
    MemoryStorage *stack_storage = nullptr;
    native_address_t stack_pointer_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != push_data);

    /* Retrieve the Stack pointer from the program context. */
    register_stack_pointer = program_ctx->register_file.get_register_stack_pointer();

    /* Read the address pointed to by the Stack pointer. */
    stack_pointer_address = register_stack_pointer->read();

    /* Retrieve the memory storage containing the stack from the program context's memory manager. */
    stack_storage = program_ctx->memory_manager.get_stack_storage();

    /* Write the data to the location in the stack just above the data on top.
     * Note that the stack grows top-down.
     * */
    status = stack_storage->write(
        push_data,
        push_size,
        stack_pointer_address
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("write stack storage failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Decrement the Stack pointer to point to the new top of the stack. */
    stack_pointer_address -= sizeof(push_size);

    /* Write the updated Stack address back to the register. */
    register_stack_pointer->write(stack_pointer_address);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStackOpcode::pull(
    ProgramContext *program_ctx,
    void *pull_buffer,
    std::size_t pull_size
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_stack_pointer = nullptr;
    MemoryStorage *stack_storage = nullptr;
    native_address_t stack_pointer_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != pull_buffer);

    /* Retrieve the Stack pointer from the program context. */
    register_stack_pointer = program_ctx->register_file.get_register_stack_pointer();

    /* Read the address pointed to by the Stack pointer. */
    stack_pointer_address = register_stack_pointer->read();

    /* Retrieve the memory storage containing the stack from the program context's memory manager. */
    stack_storage = program_ctx->memory_manager.get_stack_storage();

    /* Increment the Stack pointer to point to the data at the top of the stack. */
    stack_pointer_address += sizeof(pull_size);

    /* Read the data from the storage location representing the top of the stack.
     * Note that the stack grows top-down.
     * */
    status = stack_storage->read(
        pull_buffer,
        pull_size,
        stack_pointer_address
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read stack storage failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Write the updated Stack address back to the register. */
    register_stack_pointer->write(stack_pointer_address);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStackOpcode::push(ProgramContext *program_ctx, native_word_t push_word)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != program_ctx);

    /* Call the "real" push implementation with the data word to push. */
    status = push(program_ctx, &push_word, sizeof(push_word));
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("push failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStackOpcode::push(ProgramContext *program_ctx, native_address_t push_address)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_address_t converted_push_address = system_big_to_native_endianess(push_address);

    ASSERT(nullptr != program_ctx);

    /* Call the "real" push implementation with the address to push, after it has been byteswapped. */
    status = push(program_ctx, &converted_push_address, sizeof(converted_push_address));
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("push failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStackOpcode::pull(ProgramContext *program_ctx, native_word_t *output_pull_word)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t pull_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_pull_word);

    /* Call the "real" pull implementation with a buffer to contain the data word being pulled. */
    status = push(program_ctx, &pull_data, sizeof(pull_data));
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("push failed. Status: %d", status);
        goto l_cleanup;
    }

    *output_pull_word = pull_data;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStackOpcode::pull(ProgramContext *program_ctx, native_address_t *output_pull_address)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_address_t pull_address = 0;
    native_address_t converted_pull_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != output_pull_address);

    /* Call the "real" pull implementation with a buffer to contain the address being pulled. */
    status = push(program_ctx, &pull_address, sizeof(pull_address));
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("push failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Convert the address from its storage endianess, little endian, to big endian. */
    converted_pull_address = system_native_to_big_endianess(pull_address);

    *output_pull_address = converted_pull_address;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
