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
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_WORD;

    inline enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t immediate_value,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};


class ImmediateDoubleAddressMode : public ImmediateAddressMode {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_DWORD;
};


class RelativeAddressMode : public ImmediateAddressMode {};

} /* namespace address_modes */

#endif /* __IMMEDIATE_ADDRESS_MODE_H__ */
