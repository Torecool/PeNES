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

#include "instruction_set/transfer_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus ITransferOpcode::transfer(
    ProgramContext *program_ctx,
    RegisterStorage<native_word_t> *src_register,
    RegisterStorage<native_word_t> *dest_register
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t dest_register_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != src_register);
    ASSERT(nullptr != dest_register);

    /* Transfer the entire contents of the source register to the destination register. */
    src_register->transfer(dest_register);

    /* Read the contents of the dest register in order to determine status flags to update. */
    dest_register_data = dest_register->read();

    /* Call the parent function to update the status flags. */
    status = this->update_data_status(program_ctx, dest_register_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_data_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeTAX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;
    RegisterStorage<native_word_t> *register_x = nullptr;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers A and X. */
    register_a = program_ctx->register_file.get_register_a();
    register_x = program_ctx->register_file.get_register_x();

    /* Perform the transfer operation between the source and dest registers A and X. */
    status = this->transfer(program_ctx, register_a, register_x);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeTAY::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;
    RegisterStorage<native_word_t> *register_y = nullptr;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers A and Y. */
    register_a = program_ctx->register_file.get_register_a();
    register_y = program_ctx->register_file.get_register_y();

    /* Perform the transfer operation between the source and dest registers A and Y. */
    status = this->transfer(program_ctx, register_a, register_y);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeTXA::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;
    RegisterStorage<native_word_t> *register_a = nullptr;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers X and A. */
    register_x = program_ctx->register_file.get_register_x();
    register_a = program_ctx->register_file.get_register_a();

    /* Perform the transfer operation between the source and dest registers X and A. */
    status = this->transfer(program_ctx, register_x, register_a);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeTSX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_stack_pointer = nullptr;
    RegisterStorage<native_word_t> *register_x = nullptr;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get the Stack pointer and register X. */
    register_stack_pointer = program_ctx->register_file.get_register_stack_pointer();
    register_x = program_ctx->register_file.get_register_x();

    /* Perform the transfer operation between the source and dest Stack pointer and register X. */
    status = this->transfer(program_ctx, register_stack_pointer, register_x);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeTXS::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;
    RegisterStorage<native_word_t> *register_stack_pointer = nullptr;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers X and the Stack pointer. */
    register_x = program_ctx->register_file.get_register_x();
    register_stack_pointer = program_ctx->register_file.get_register_stack_pointer();

    /* Transfer the entire contents of the register X to the Stack pointer.
     * Since this function does not set any status flags,
     * we will skip calling the superclass transfer function,
     * but keep the class hierarchy for its logical meaning.
     * */
    register_x->transfer(register_stack_pointer);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeTYA::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_y = nullptr;
    RegisterStorage<native_word_t> *register_a = nullptr;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers Y, A and the status. */
    register_y = program_ctx->register_file.get_register_y();
    register_a = program_ctx->register_file.get_register_a();

    /* Perform the transfer operation between the source and dest registers Y and A. */
    status = this->transfer(program_ctx, register_y, register_a);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
