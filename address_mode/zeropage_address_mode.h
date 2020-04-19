/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __ZEROPAGE_ADDRESS_MODE_H__
#define __ZEROPAGE_ADDRESS_MODE_H__

/** Headers ***************************************************************/
#include "penes_status.h"
#include "common.h"

#include "address_mode/address_mode_interface.h"

/** Namespaces ************************************************************/
namespace address_modes {

/** Classes ***************************************************************/
class ZeropageAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_WORD;

    inline enum PeNESStatus get_data(
        const ProgramContext *program_context,
        native_dword_t zeropage_address,
        native_word_t *output_data
    ) override;
};

class ZeropageXIndexedAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_WORD;

    inline enum PeNESStatus get_data(
        const ProgramContext *program_context,
        native_dword_t zeropage_indexed_address,
        native_word_t *output_data
    ) override;
};

class ZeropageYIndexedAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_WORD;

    inline enum PeNESStatus get_data(
        const ProgramContext *program_context,
        native_dword_t zeropage_indexed_address,
        native_word_t *output_data
    ) override;
};

} /* namespace address_modes */

#endif /* __ZEROPAGE_ADDRESS_MODE_H__ */
