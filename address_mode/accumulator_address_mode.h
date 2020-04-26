/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

#ifndef __ACCUMULATOR_ADDRESS_MODE_H__
#define __ACCUMULATOR_ADDRESS_MODE_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "address_mode/address_mode_interface.h"

/** Namespaces ************************************************************/
namespace address_mode {

/** Classes ***************************************************************/
class AccumulatorAddressMode : public IAddressMode {
public:
    const enum InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_NO_OPERAND;

    inline enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t operand,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset = nullptr
    ) override;
};

} /* namespace address_modes */

#endif /* __ACCUMULATOR_ADDRESS_MODE_H__ */
