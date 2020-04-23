/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

#ifndef __IMMEDIATE_ADDRESS_MODE_H__
#define __IMMEDIATE_ADDRESS_MODE_H__

/** Headers ***************************************************************/
#include <cstddef>
#include "penes_status.h"

#include "address_mode/address_mode_interface.h"

/** Namespaces ************************************************************/
namespace address_mode {

/** Classes ***************************************************************/
class ImmediateAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_WORD;

    inline enum PeNESStatus get_storage(
        const ProgramContext *program_ctx,
        native_dword_t immediate_value,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset = nullptr
    ) override;
};


class RelativeAddressMode : public ImmediateAddressMode {};

} /* namespace address_modes */

#endif /* __IMMEDIATE_ADDRESS_MODE_H__ */
