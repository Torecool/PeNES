/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

/** Headers ***************************************************************/
#include "penes_status.h"
#include "common.h"

#include "memory_manager/memory_manager.h"
#include "address_mode/address_mode_interface.h"

#include "address_mode/accumulator_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_modes;

/** Functions *************************************************************/
inline enum PeNESStatus AccumulatorAddressMode::get_storage(
    const ProgramContext *program_context,
    native_dword_t operand,
    StorageLocation **output_storage,
    size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage *register_a = nullptr;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_storage);

    UNREFERENCED_PARAMETER(operand);

    /* Retrieve storage at absolute operand. */
    register_a = program_context->register_file.get_register_a();

    *output_storage = register_a;
    if (nullptr != output_storage_offset) {
        *output_storage_offset = 0;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
