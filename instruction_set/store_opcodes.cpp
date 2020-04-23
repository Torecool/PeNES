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
enum PeNESStatus IStoreOpcode::store(
    ProgramContext *program_ctx,
    RegisterStorage<native_word_t> *store_register,
    IStorageLocation *store_storage,
    std::size_t storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != store_register);
    ASSERT(nullptr != store_storage);

    /* Transfer the entire contents of the store register to the operand memory location. */
    status = store_register->transfer(
        store_storage,
        store_register->get_storage_size(),
        0,
        storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


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

    /* Store the contents of register A in the operand memory location. */
    status = store(program_ctx, register_a, store_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("store failed. Status: %d", status);
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

    /* Store the contents of register X in the operand memory location. */
    status = store(program_ctx, register_x, store_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("store failed. Status: %d", status);
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

    /* Store the contents of register Y in the operand memory location. */
    status = store(program_ctx, register_y, store_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("store failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
