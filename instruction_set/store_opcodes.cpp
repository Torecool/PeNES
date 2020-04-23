/**
 * @brief  
 * @author TBK
 * @date   23/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

#include "instruction_set/store_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus OpcodeSTA::exec(
    ProgramContext *program_ctx,
    IStorageLocation *store_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != store_operand_storage);

    /* Read contents of register A. */
    register_a = program_ctx->register_file.get_register_a();

    /* Transfer the entire contents of register A to the operand memory location. */
    register_a->transfer(
        store_operand_storage,
        register_a->get_storage_size(),
        0,
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeSTX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *store_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != store_operand_storage);

    /* Read contents of register X. */
    register_x = program_ctx->register_file.get_register_x();

    /* Transfer the entire contents of register X to the operand memory location. */
    register_x->transfer(
        store_operand_storage,
        register_x->get_storage_size(),
        0,
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeSTY::exec(
    ProgramContext *program_ctx,
    IStorageLocation *store_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_y = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != store_operand_storage);

    /* Read contents of register Y. */
    register_y = program_ctx->register_file.get_register_y();

    /* Transfer the entire contents of register Y to the operand memory location. */
    register_y->transfer(
        store_operand_storage,
        register_y->get_storage_size(),
        0,
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
