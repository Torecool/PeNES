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

#include "instruction_set/load_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus ILoadOpcode::load(
    ProgramContext *program_ctx,
    RegisterStorage<native_word_t> *load_register,
    IStorageLocation *load_storage,
    std::size_t storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t operand_storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != load_register);
    ASSERT(nullptr != load_storage);

    /* Read the data at the operand storage location. */
    status = load_storage->read(
        &operand_storage_data,
        sizeof(operand_storage_data),
        storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Write the data into the register. */
    load_register->write(operand_storage_data);

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, operand_storage_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeLDA::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;
    native_word_t operand_storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register A from the program context. */
    register_a = program_ctx->register_file.get_register_a();

    /* Load the data at the storage location into the register. */
    status = this->load(program_ctx, register_a, data_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeLDX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;
    native_word_t operand_storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register X from the program context. */
    register_x = program_ctx->register_file.get_register_x();

    /* Load the data at the storage location into the register. */
    status = this->load(program_ctx, register_x, data_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeLDY::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_y = nullptr;
    native_word_t operand_storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register Y from the program context. */
    register_y = program_ctx->register_file.get_register_y();

    /* Load the data at the storage location into the register. */
    status = this->load(program_ctx, register_y, data_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;}
