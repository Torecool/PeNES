/**
 * @brief  
 * @author TBK
 * @date   26/04/2020
 * */

#ifndef __CPU_H__
#define __CPU_H__

/** Headers ***************************************************************/
#include "penes_status.h"
#include "system.h"

#include "program_context/program_context.h"
#include "decoder/decoder.h"
#include "instruction_set/operation_types.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
class CPU : public instruction_set::IJumpOperation {
public:
    inline explicit CPU(ProgramContext *program_ctx):
        program_ctx(program_ctx), instruction_decoder(program_ctx)
    {
        ASSERT(nullptr != program_ctx);
    }

    enum PeNESStatus run();

private:
    inline enum PeNESStatus reset()
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        MemoryStorage *reset_jump_vector_storage = nullptr;

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

    ProgramContext *program_ctx;
    Decoder instruction_decoder;
};


#endif /* __CPU_H__ */
