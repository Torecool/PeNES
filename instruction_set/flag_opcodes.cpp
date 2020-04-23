/**
 * @brief  
 * @author TBK
 * @date   22/04/2020
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
enum PeNESStatus ISetFlagOpcode::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_status = nullptr;
    native_word_t register_status_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Read contents of the status register. */
    register_status = program_ctx->register_file.get_register_status();
    register_status_data = register_status->read();

    /* Write the status register back with the turned on bits in the mask set to 1. */
    register_status->write(register_status_data | set_flag_mask);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IClearFlagOpcode::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_status = nullptr;
    native_word_t register_status_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Read contents of the status register. */
    register_status = program_ctx->register_file.get_register_status();
    register_status_data = register_status->read();

    /* Write the status register back with the turned on bits in the mask set to 0. */
    register_status->write(register_status_data & ~clear_flag_mask);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
