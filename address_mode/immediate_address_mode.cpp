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

#include "address_mode/immediate_address_mode.h"

/** Namespaces ************************************************************/
using namespace address_modes;

/** Functions *************************************************************/
inline enum PeNESStatus ImmediateAddressMode::get_storage(
    const ProgramContext *program_context,
    native_dword_t address,
    StorageLocation **output_storage
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != program_context);
    ASSERT(nullptr != output_storage);

    UNREFERENCED_PARAMETER(program_context);
    UNREFERENCED_PARAMETER(address);

    /* The concept of a storage location makes no sense for an immediate address mode, even just for reading. */
    output_storage = nullptr;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}

