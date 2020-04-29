/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __ABSOLUTE_ADDRESS_MODE_H__
#define __ABSOLUTE_ADDRESS_MODE_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "address_mode/address_mode_interface.h"

/** Namespaces ************************************************************/
namespace address_mode {

/** Classes ***************************************************************/
class AbsoluteAddressMode : public IAddressMode {
public:
    inline enum InstructionOperandSize get_operand_size() const override
    {
        return INSTRUCTION_OPERAND_SIZE_DWORD;
    }

    enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t absolute_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};


class AbsoluteXIndexedAddressMode : public IAddressMode {
public:
    inline enum InstructionOperandSize get_operand_size() const override
    {
        return INSTRUCTION_OPERAND_SIZE_DWORD;
    }

    enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t absolute_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};


class AbsoluteYIndexedAddressMode : public IAddressMode {
public:
    inline enum InstructionOperandSize get_operand_size() const override
    {
        return INSTRUCTION_OPERAND_SIZE_DWORD;
    }

    enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t absolute_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};

} /* namespace address_modes */


#endif /* __ABSOLUTE_ADDRESS_MODE_H__ */
