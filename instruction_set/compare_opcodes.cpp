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

#include "instruction_set/compare_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus ICompareOpcode::compare(
    ProgramContext *program_ctx,
    RegisterStorage<native_word_t> *compare_register,
    IStorageLocation *compare_storage,
    std::size_t storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t compare_register_data = 0;
    native_word_t compare_storage_data = 0;
    native_dword_t comparison_result = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != compare_register);
    ASSERT(nullptr != compare_storage);

    /* Read the data stored in the register. */
    compare_register_data = compare_register->read();

    /* Read the data at the compare storage location. */
    status = compare_storage->read(
        &compare_storage_data,
        sizeof(compare_storage_data),
        storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Perform the comparison operation by subtracting the comparison operand from the register. */
    comparison_result = compare_register_data - compare_storage_data;

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, comparison_result);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeCMP::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register A from the program context. */
    register_a = program_ctx->register_file.get_register_a();

    /* Compare the register with the operand memory storage. */
    status = this->compare(program_ctx, register_a, data_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("compare failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeCPX::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_x = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register X from the program context. */
    register_x = program_ctx->register_file.get_register_x();

    /* Compare the register with the operand memory storage. */
    status = this->compare(program_ctx, register_x, data_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("compare failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeCPY::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_y = nullptr;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register Y from the program context. */
    register_y = program_ctx->register_file.get_register_y();

    /* Compare the register with the operand memory storage. */
    status = this->compare(program_ctx, register_y, data_operand_storage, operand_storage_offset);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("compare failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
