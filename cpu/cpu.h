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
    CPU(
        ProgramContext *program_ctx,
        const native_word_t *src_binary,
        std::size_t src_binary_size
    );

    CPU(
        const native_word_t *src_binary,
        std::size_t src_binary_size
    ): CPU(new ProgramContext(), src_binary, src_binary_size)
    {};

    enum PeNESStatus run();

private:
    ProgramContext *program_ctx;
    const Decoder instruction_decoder;
};


#endif /* __CPU_H__ */
