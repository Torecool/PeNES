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

#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IUpdateStatusOperation::update_status(
    RegisterStorage<native_word_t> *register_status,
    native_word_t update_values
) const
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t status_flags = 0;
    native_word_t updated_status_flags = 0;
    native_word_t update_mask = this->get_update_mask();
    native_word_t base_update_values = this->get_base_update_values();

    ASSERT(nullptr != register_status);

    /* Read contents of the status register. */
    status_flags = register_status->read();

    /* Update the status register flags with the values from base_values together with update_values.
     * A flag will only be updated if it is set in update_mask.
     * */
    updated_status_flags = (status_flags & ~update_mask) | base_update_values | update_values;

    /* Write the status register back. */
    register_status->write(updated_status_flags);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStackOperation::push(
    ProgramContext *program_ctx,
    const native_word_t *push_data,
    std::size_t num_push_words
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_stack_pointer = nullptr;
    MemoryStorage *stack_storage = nullptr;
    native_word_t stack_pointer_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != push_data);

    /* Retrieve the Stack pointer from the program context. */
    register_stack_pointer = program_ctx->register_file.get_register_stack_pointer();

    /* Read the address pointed to by the Stack pointer. */
    stack_pointer_address = register_stack_pointer->read();

    /* Retrieve the memory storage containing the stack from the program context's memory manager. */
    stack_storage = program_ctx->memory_map.get_stack();

    /* Decrement the Stack pointer to point to the location we will be writing to,
     * considering the current stack pointer is just above the last data.
     * */
    stack_pointer_address -= (num_push_words - 1);

    /* Write the data to the location in the stack just above the data on top.
     * Note that the stack grows top-down.
     * */
    status = stack_storage->write(
        push_data,
        num_push_words,
        stack_pointer_address
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("write stack storage failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Decrement the Stack pointer to point to the new top the stack, just above the data we just wrote. */
    stack_pointer_address--;

    /* Write the updated Stack address back to the register. */
    register_stack_pointer->write(stack_pointer_address);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IStackOperation::pull(
    ProgramContext *program_ctx,
    native_word_t *pull_buffer,
    std::size_t num_pull_words
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
    stack_storage = program_ctx->memory_map.get_stack();

    /* Increment the Stack pointer to point to the location we will be reading from,
     * considering the current stack pointer is just above the last data.
     * */
    stack_pointer_address++;

    /* Read the data from the storage location representing the top of the stack.
     * Note that the stack grows top-down.
     * */
    status = stack_storage->read(
        pull_buffer,
        num_pull_words,
        stack_pointer_address
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read stack storage failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Increment the Stack pointer to point to the new top of the stack,
     * meaning just above the data that was below the pulled data.
     * */
    stack_pointer_address += (num_pull_words - 1);

    /* Write the updated Stack address back to the register. */
    register_stack_pointer->write(stack_pointer_address);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IJumpOperation::jump(
    RegisterStorage<native_dword_t> *register_program_counter,
    IStorageLocation *jump_address_storage,
    std::size_t address_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_address_t jump_address = 0;
    native_address_t converted_jump_address = 0;

    ASSERT(nullptr != register_program_counter);
    ASSERT(nullptr != jump_address_storage);

    /* Read the jump address from the storage location. */
    status = jump_address_storage->read(
        reinterpret_cast<native_word_t *>(&jump_address),
        sizeof(jump_address),
        address_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Jump address storage read failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Convert the jump address to big endian because addresses are kept in little endian in memory. */
    converted_jump_address = system_native_to_host_endianness(jump_address);

    /* Write the converted jump address to the program counter. */
    register_program_counter->write(converted_jump_address);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}

