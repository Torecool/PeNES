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

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
class CPU {
public:
    inline explicit CPU(ProgramContext *program_ctx):
        instruction_decoder(program_ctx)
    {
        ASSERT(nullptr != program_ctx);
    }

    enum PeNESStatus run();

private:
    ProgramContext *program_ctx;
    Decoder instruction_decoder;
};


#endif /* __CPU_H__ */
