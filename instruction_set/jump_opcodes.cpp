/**
 * @brief  
 * @author TBK
 * @date   24/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

#include "instruction_set/jump_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IJumpOpcode::jump(
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
        &jump_address,
        sizeof(jump_address),
        address_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Jump address storage read failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Convert the jump address to big endian because addresses are kept in little endian in memory. */
    converted_jump_address = system_native_to_big_endianness(jump_address);

    /* Write the converted jump address to the program counter. */
    register_program_counter->write(converted_jump_address);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeJMP::exec(
    ProgramContext *program_ctx,
    IStorageLocation *jump_address_storage,
    std::size_t address_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_address_t> *register_program_counter = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != jump_address_storage);

    /* Retrieve the Program counter from the program context. */
    register_program_counter = program_ctx->register_file.get_register_program_counter();

    /* Perform the jump operation to the new location. */
    status = this->jump(register_program_counter, jump_address_storage, address_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass jump failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeJSR::exec(
    ProgramContext *program_ctx,
    IStorageLocation *jump_address_storage,
    std::size_t address_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_address_t> *register_program_counter = nullptr;
    native_address_t program_counter_address = 0;
    native_address_t jump_address = 0;
    native_address_t converted_jump_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != jump_address_storage);

    /* Retrieve the Program counter from the program context. */
    register_program_counter = program_ctx->register_file.get_register_program_counter();

    /* Read the current program counter address. */
    program_counter_address = register_program_counter->read();

    /* Increment the program counter we push onto the stack by 2,
     * so that when we return from the subroutine, the value we pull from the stack + 1 will be the next instruction.
     * */
    program_counter_address += 2;

    /* Save the updated program counter on the stack. */
    status = this->push(program_ctx, program_counter_address);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass push failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Perform the jump operation to the new location. */
    status = this->jump(register_program_counter, jump_address_storage, address_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass jump failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeBRK::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_address_t> *register_program_counter = nullptr;
    RegisterStorage<native_word_t> *register_status = nullptr;
    MemoryStorage *interrupt_vector_storage = nullptr;
    native_address_t program_counter_address = 0;
    native_address_t program_status = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Retrieve the Program counter and Status register from the program context. */
    register_program_counter = program_ctx->register_file.get_register_program_counter();
    register_status = program_ctx->register_file.get_register_status();

    /* Retrieve the IRQ interrupt handler vector from the program context. */
    interrupt_vector_storage = program_ctx->memory_manager.get_irq_interrupt_vector();

    /* Read the current program counter address and status. */
    program_counter_address = register_program_counter->read();
    program_status = register_status->read();

    /* Set the Break flag on the status that is pushed to the stack,
     * to indicate that a software interrupt is occurring.
     * Note that this value will not be written back to the register.
     * */
    program_status |= REGISTER_STATUS_FLAG_MASK_BREAK;

    /* Increment the program counter we push onto the stack by 2,
     * so that when we return from the interrupt handler,
     * the value we pull from the stack will be the instruction following a 1-byte gap.
     * */
    program_counter_address += 2;

    /* Save the updated program counter on the stack. */
    status = this->push(program_ctx, program_counter_address);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass program counter push failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Save the stack version of the status on the stack. */
    status = this->push(program_ctx, program_status);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass status push failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Perform the jump operation to the address stored within the vector table */
    status = this->jump(register_program_counter, interrupt_vector_storage);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass jump failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Call the parent function to update the status flags based on the update mask. */
    status = this->update_status(register_status);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_data_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeRTI::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_address_t> *register_program_counter = nullptr;
    RegisterStorage<native_word_t> *register_status = nullptr;
    native_word_t saved_status = 0;
    native_address_t saved_program_counter = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Retrieve the Program counter and Status register from the program context. */
    register_program_counter = program_ctx->register_file.get_register_program_counter();
    register_status = program_ctx->register_file.get_register_status();

    /* Pull the saved Status register from the stack. */
    status = this->pull(program_ctx, &saved_status);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass pull status failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Pull the saved Stack pointer from the stack. */
    status = this->pull(program_ctx, &saved_program_counter);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass pull stack pointer failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Restore the saved address to the Program counter. */
    register_program_counter->write(saved_program_counter);

    /* Call the parent function to update the status flags based on the update mask. */
    status = this->update_status(program_ctx, saved_status);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_data_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeRTS::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_address_t> *register_program_counter = nullptr;
    native_address_t saved_program_counter = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Retrieve the Program counter from the program context. */
    register_program_counter = program_ctx->register_file.get_register_program_counter();

    /* Pull the saved Stack pointer from the stack. */
    status = this->pull(program_ctx, &saved_program_counter);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass pull stack pointer failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Increment the saved program counter address so that it will point to the next instruction. */
    saved_program_counter++;

    /* Restore the saved address to the Program counter. */
    register_program_counter->write(saved_program_counter);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
