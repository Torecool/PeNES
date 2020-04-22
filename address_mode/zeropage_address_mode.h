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

    inline enum PeNESStatus get_storage(
        const ProgramContext *program_context,
        native_dword_t zeropage_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};

class ZeropageXIndexedAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_WORD;

    inline enum PeNESStatus get_storage(
        const ProgramContext *program_context,
        native_dword_t zeropage_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};

class ZeropageYIndexedAddressMode : public IAddressMode {
public:
    const enum InstructionArgSize arg_size = INSTRUCTION_ARG_SIZE_WORD;

    inline enum PeNESStatus get_storage(
        const ProgramContext *program_context,
        native_dword_t zeropage_address,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override;
};

} /* namespace address_modes */

#endif /* __ZEROPAGE_ADDRESS_MODE_H__ */
