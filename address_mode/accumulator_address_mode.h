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
    inline enum PeNESStatus get_storage(
        ProgramContext *program_ctx,
        native_dword_t operand,
        IStorageLocation **output_storage,
        std::size_t *output_storage_offset
    ) override
    {
        RegisterStorage<native_word_t> *register_a = nullptr;

        ASSERT(nullptr != program_ctx);
        ASSERT(nullptr != output_storage);

        UNREFERENCED_PARAMETER(operand);

        /* Retrieve storage at absolute operand. */
        register_a = program_ctx->register_file.get_register_a();

        *output_storage = register_a;

        if (nullptr != output_storage_offset) {
            *output_storage_offset = 0;
        }

        return PENES_STATUS_SUCCESS;
    }
};

} /* namespace address_modes */

#endif /* __ACCUMULATOR_ADDRESS_MODE_H__ */
