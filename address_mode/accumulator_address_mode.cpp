/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "memory_manager/memory_manager.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/accumulator_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_mode;

/** Functions *************************************************************/
inline enum PeNESStatus AccumulatorAddressMode::get_storage(
    const ProgramContext *program_ctx,
    native_dword_t operand,
    IStorageLocation **output_storage,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
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

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
