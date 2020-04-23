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
#include "inc_dec_opcodes.h"


/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus OpcodeINC::exec(
    ProgramContext *program_ctx,
    IStorageLocation *increment_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t memory_storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != increment_operand_storage);

    /* Read the data at the memory storage location. */
    status = increment_operand_storage->read(
        &memory_storage_data,
        sizeof(memory_storage_data),
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read memory storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Perform the increment operation on the data. */
    memory_storage_data++;

    /* Write the incremented data back to the memory storage location. */
    status = increment_operand_storage->write(
        &memory_storage_data,
        sizeof(memory_storage_data),
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("write memory storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, memory_storage_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeDEC::exec(
    ProgramContext *program_ctx,
    IStorageLocation *decrement_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t memory_storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != decrement_operand_storage);

    /* Read the data at the memory storage location. */
    status = decrement_operand_storage->read(
        &memory_storage_data,
        sizeof(memory_storage_data),
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read memory storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Perform the decrement operation on the data. */
    memory_storage_data--;

    /* Write the decremented data back to the memory storage location. */
    status = decrement_operand_storage->write(
        &memory_storage_data,
        sizeof(memory_storage_data),
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("write memory storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, memory_storage_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeINX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;
    native_word_t register_x_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Retrieve register X from the program context. */
    register_x = program_ctx->register_file.get_register_x();

    /* Read the data stored in register X. */
    register_x_data = register_x->read();

    /* Perform the increment operation on the data. */
    register_x_data++;

    /* Write the incremented data back to the memory storage location. */
    register_x->write(register_x_data);

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


enum PeNESStatus OpcodeDEX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;
    native_word_t register_x_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Retrieve register X from the program context. */
    register_x = program_ctx->register_file.get_register_x();

    /* Read the data stored in register X. */
    register_x_data = register_x->read();

    /* Perform the decrement operation on the data. */
    register_x_data--;

    /* Write the decremented data back to the memory storage location. */
    register_x->write(register_x_data);

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


enum PeNESStatus OpcodeINY::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_y = nullptr;
    native_word_t register_y_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Retrieve register Y from the program context. */
    register_y = program_ctx->register_file.get_register_y();

    /* Read the data stored in register Y. */
    register_y_data = register_y->read();

    /* Perform the increment operation on the data. */
    register_y_data++;

    /* Write the incremented data back to the memory storage location. */
    register_y->write(register_y_data);

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


enum PeNESStatus OpcodeDEY::exec(
    ProgramContext *program_ctx,
    IStorageLocation *operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_y = nullptr;
    native_word_t register_y_data = 0;

    ASSERT(nullptr != program_ctx);

    UNREFERENCED_PARAMETER(operand_storage);
    UNREFERENCED_PARAMETER(operand_storage_offset);

    /* Retrieve register Y from the program context. */
    register_y = program_ctx->register_file.get_register_y();

    /* Read the data stored in register Y. */
    register_y_data = register_y->read();

    /* Perform the decrement operation on the data. */
    register_y_data--;

    /* Write the decremented data back to the memory storage location. */
    register_y->write(register_y_data);

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, register_y_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;}
