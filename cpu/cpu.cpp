/**
 * @brief  
 * @author TBK
 * @date   26/04/2020
 * */

/** Headers ***************************************************************/
#include <sys/time.h>
#include <iostream>

#include "cpu/cpu.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
enum PeNESStatus CPU::run()
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    instruction_set::Instruction *current_instruction = nullptr;

    struct timeval start_time = {0};
    struct timeval end_time = {0};
    std::size_t total_instructions = 0;

    /* Since the program is starting up, reset the machine by jumping to the address at the reset interrupt vector. */
    status = this->reset();
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("reset failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    gettimeofday(&start_time, NULL);


    while (total_instructions < 20000) {
        /* Retrieve next instruction. */
        status = this->instruction_decoder.next_instruction(&current_instruction);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("next_instruction failed. Status: %d.\n", status);
            goto l_cleanup;
        }

        /* Execute the instruction. */
        status = current_instruction->exec();
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("exec failed. Status: %d.\n", status);
            goto l_cleanup;
        }

        /* TODO CPU cycle bookkeeping. */

        /* Check for interrupts and service if necessary. */
        status = service_interrupts();
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("service_interrupts failed. Status: %d.\n", status);
            goto l_cleanup;
        }

        /* Release resources used by the instruction. */
        delete current_instruction;

        total_instructions++;
    }

    gettimeofday(&end_time, NULL);
    std::cout << "Average Instruction time (us): " << static_cast<double>(end_time.tv_usec - start_time.tv_usec) / total_instructions  << std::endl;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus CPU::reset()

{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_status = nullptr;
    MemoryStorage *reset_jump_vector_storage = nullptr;
    native_word_t previous_status = 0;

    /* Reset the interrupt receive flags. */
    this->program_ctx->did_receive_irq = false;
    this->program_ctx->did_receive_nmi = false;

    /* Set the Interrupt Disable status flag. */
    register_status = this->program_ctx->register_file.get_register_status();
    previous_status = register_status->read();
    register_status->write(previous_status | REGISTER_STATUS_FLAG_MASK_INTERRUPT);

    /* Retrieve the reset interrupt handler vector from the program context. */
    reset_jump_vector_storage = this->program_ctx->memory_map.get_reset_jump_vector();

    /* Perform the jump operation to the address stored within the vector table */
    status = IJumpOperation::jump(this->program_ctx, reset_jump_vector_storage);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass jump failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus CPU::service_interrupts()
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_status = nullptr;
    MemoryStorage *jump_vector_storage = nullptr;
    native_word_t program_status = 0;
    bool did_set_interrupt_disable = false;

    /* Get the Interrupt Disable status flag. */
    register_status = this->program_ctx->register_file.get_register_status();
    program_status = register_status->read();
    did_set_interrupt_disable = (0 != (program_status & REGISTER_STATUS_FLAG_MASK_INTERRUPT));

    /* Check for NMI interrupts first (has higher priority). */
    if (true == this->program_ctx->did_receive_nmi) {
        /* Retrieve the NMI interrupt handler vector from the program context. */
        jump_vector_storage = this->program_ctx->memory_map.get_nmi_jump_vector();
        /* Reset NMI receive status because the NMIs are esge-triggered. */
        this->program_ctx->did_receive_nmi = false;
        /* Next, check for IRQ interrupts (in case they haven't been disabled). */
    } else if ((true == did_set_interrupt_disable) &&
               (true == this->program_ctx->did_receive_nmi)) {
        /* Retrieve the IRQ interrupt handler vector from the program context. */
        jump_vector_storage = this->program_ctx->memory_map.get_irq_jump_vector();
        /* No condition is met, skip the rest. */
    } else {
        status = PENES_STATUS_SUCCESS;
        goto l_cleanup;
    }

    /* Enter the corresponding interrupt handler routine. */
    status = this->execute_interrupt_handler(program_ctx, jump_vector_storage);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass execute_interrupt_handler failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
