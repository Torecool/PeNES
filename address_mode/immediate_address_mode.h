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
template<typename SizeType>
class ImmediateAddressMode : public IAddressMode {
public:
    enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t immediate_value,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override
    {
        ImmediateStorage<SizeType> *immediate_storage = nullptr;

        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != output_storage);

        /* Create a new immediate storage object. */
        immediate_storage = new ImmediateStorage<SizeType>();

        /* Set the immediate value of the storage object.
         * This will be the return value when calling read.
         * */
        immediate_storage->set(immediate_value);

        *output_storage = immediate_storage;
        if (nullptr != output_storage_offset) {
            *output_storage_offset = 0;
        }

        return PENES_STATUS_SUCCESS;
    }

    inline enum PeNESStatus release_storage(
        ProgramContext *program_ctx,
        IStorageLocation *immediate_storage
    ) override
    {
        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != immediate_storage);

        /* Release the allocated memory of the immediate storage object. */
        delete immediate_storage;

        return PENES_STATUS_SUCCESS;
    }
};


class ImmediateSingleAddressMode : public ImmediateAddressMode<native_word_t> {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_WORD;
};


class RelativeAddressMode : public ImmediateAddressMode<native_word_t> {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_WORD;
};


class ImmediateDoubleAddressMode : public ImmediateAddressMode<native_dword_t> {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_DWORD;
};

} /* namespace address_modes */

#endif /* __IMMEDIATE_ADDRESS_MODE_H__ */
