/**
 * @brief  
 * @author TBK
 * @date   26/04/2020
 * */

/** Headers ***************************************************************/
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

    while (true) {
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

        /* Check for interrupts and service if necessary. */

        /* Release resources used by the instruction. */
        delete current_instruction;
    }


    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}

