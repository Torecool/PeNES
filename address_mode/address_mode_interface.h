/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __ADDRESS_MODE_INTERFACE_H__
#define __ADDRESS_MODE_INTERFACE_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "program_context/program_context.h"

/** Namespaces ************************************************************/
namespace address_mode {

/** Enums *****************************************************************/
enum InstructionOperandSize {
    INSTRUCTION_OPERAND_SIZE_NONE = -1,
    INSTRUCTION_OPERAND_SIZE_NO_OPERAND = 0,
    INSTRUCTION_OPERAND_SIZE_WORD = sizeof(native_word_t),
    INSTRUCTION_OPERAND_SIZE_DWORD = sizeof(native_dword_t)
};

/** Classes ***************************************************************/
class IAddressMode {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_NO_OPERAND;

    virtual inline enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t operand,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) = 0;
};

} /* namespace address_modes */

#endif /* __ADDRESS_MODE_INTERFACE_H__ */
