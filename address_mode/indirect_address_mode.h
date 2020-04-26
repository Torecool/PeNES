/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __INDIRECT_ADDRESS_MODE_H__
#define __INDIRECT_ADDRESS_MODE_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "address_mode/address_mode_interface.h"

/** Namespaces ************************************************************/
namespace address_mode {

/** Classes ***************************************************************/
class IndirectAddressMode : public IAddressMode {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_DWORD;

    inline enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t indirect_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};


class XIndexedIndirectAddressMode : public IAddressMode {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_WORD;

    inline enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t indirect_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};


class IndirectYIndexedAddressMode : public IAddressMode {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_WORD;

    inline enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t indirect_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};

} /* namespace address_modes */


#endif /* __INDIRECT_ADDRESS_MODE_H__ */
