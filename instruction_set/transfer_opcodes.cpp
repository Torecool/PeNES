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
enum PeNESStatus OpcodeTAX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;
    RegisterStorage<native_word_t> *register_x = nullptr;
    native_word_t register_x_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers A and X. */
    register_a = program_ctx->register_file.get_register_a();
    register_x = program_ctx->register_file.get_register_x();

    /* Transfer the entire contents of register A to register X. */
    status = register_a->transfer(register_x, register_a->get_storage_size());
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Read contents of register X in order to determine status flags to update. */
    register_x_data = register_x->read();

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, register_x_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
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
    native_word_t register_y_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers A and Y. */
    register_a = program_ctx->register_file.get_register_a();
    register_y = program_ctx->register_file.get_register_y();

    /* Transfer the entire contents of register A to register Y. */
    status = register_a->transfer(register_y, register_a->get_storage_size());
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Read contents of register Y in order to determine status flags to update. */
    register_y_data = register_y->read();

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, register_y_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
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
    native_word_t register_a_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers X and A. */
    register_x = program_ctx->register_file.get_register_x();
    register_a = program_ctx->register_file.get_register_a();

    /* Transfer the entire contents of register X to register A. */
    status = register_x->transfer(register_a, register_x->get_storage_size());
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Read contents of register A in order to determine status flags to update. */
    register_a_data = register_a->read();

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, register_a_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
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
    native_word_t register_x_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get the Stack pointer and register X. */
    register_stack_pointer = program_ctx->register_file.get_register_stack_pointer();
    register_x = program_ctx->register_file.get_register_x();

    /* Transfer the entire contents of the Stack pointer to register X. */
    status = register_stack_pointer->transfer(
        register_x,
        register_stack_pointer->get_storage_size()
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Read contents of register X in order to determine status flags to update. */
    register_x_data = register_x->read();

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, register_x_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
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

    /* Transfer the entire contents of the register X to the Stack pointer. */
    status = register_x->transfer(
        register_stack_pointer,
        register_x->get_storage_size()
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

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
    native_word_t register_a_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Get registers Y, A and the status. */
    register_y = program_ctx->register_file.get_register_y();
    register_a = program_ctx->register_file.get_register_a();

    /* Transfer the entire contents of the Stack pointer to register X. */
    status = register_y->transfer(
        register_a,
        register_y->get_storage_size()
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("transfer failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Read contents of register A in order to determine status flags to update. */
    register_a_data = register_a->read();

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, register_a_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
