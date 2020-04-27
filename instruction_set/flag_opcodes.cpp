/**
 * @brief  
 * @author TBK
 * @date   27/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

#include "instruction_set/flag_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IFlagOpcode::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Call the method to update the status. */
    status = this->update_status(program_ctx);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("update_data_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}