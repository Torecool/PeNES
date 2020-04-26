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


CPU::CPU(
    ProgramContext *program_ctx,
    const native_word_t *src_binary,
    std::size_t src_binary_size
):
    instruction_decoder(program_ctx, src_binary, src_binary_size)
{
    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != src_binary);
    ASSERT(0 < src_binary_size);
}
