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

    virtual inline enum PeNESStatus get_storage(
        const ProgramContext *program_context,
        native_dword_t operand,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset = nullptr
    ) = 0;
};

} /* namespace address_modes */

#endif /* __ADDRESS_MODE_INTERFACE_H__ */
