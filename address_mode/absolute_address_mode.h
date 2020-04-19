/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __ABSOLUTE_ADDRESS_MODE_H__
#define __ABSOLUTE_ADDRESS_MODE_H__

/** Headers ***************************************************************/
#include <cstdint>

#include "penes_status.h"

#include "address_mode/address_mode_interface.h"

/** Namespaces ************************************************************/
namespace address_modes {

/** Classes ***************************************************************/
class AbsoluteAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_DWORD;

    inline enum PeNESStatus get_data(
        const ProgramContext *program_context,
        native_dword_t absolute_address,
        native_word_t *output_data
    ) override;
};


class AbsoluteXIndexedAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_DWORD;

    inline enum PeNESStatus get_data(
        const ProgramContext *program_context,
        native_dword_t absolute_indexed_address,
        native_word_t *output_data
    ) override;
};


class AbsoluteYIndexedAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_DWORD;

    inline enum PeNESStatus get_data(
        const ProgramContext *program_context,
        native_dword_t absolute_indexed_address,
        native_word_t *output_data
    ) override;
};

} /* namespace address_modes */


#endif /* __ABSOLUTE_ADDRESS_MODE_H__ */
