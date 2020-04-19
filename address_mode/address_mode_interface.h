/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __ADDRESS_MODE_INTERFACE_H__
#define __ADDRESS_MODE_INTERFACE_H__

/** Headers ***************************************************************/
#include <cstdint>

#include "penes_status.h"
#include "common.h"

#include "program_context/program_context.h"

/** Namespaces ************************************************************/
namespace address_modes {

/** Enums *****************************************************************/
enum InstructionArgSize {
    INSTRUCTION_ARG_SIZE_NO_ARG = 0,
    INSTRUCTION_ARG_SIZE_WORD = sizeof(native_word_t),
    INSTRUCTION_ARG_SIZE_DWORD = sizeof(native_dword_t)
};

/** Classes ***************************************************************/
class IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_NO_ARG;

    virtual inline enum PeNESStatus get_data(
        const ProgramContext *program_context,
        native_dword_t operand,
        native_word_t *output_data
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

        ASSERT(nullptr != program_context);
        ASSERT(nullptr != output_data);

        *output_data = static_cast<native_word_t>(operand);

        status = PENES_STATUS_SUCCESS;
l_cleanup:
        return status;
    }
};

} /* namespace address_modes */

#endif /* __ADDRESS_MODE_INTERFACE_H__ */
