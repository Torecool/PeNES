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

#include "instruction_set/branch_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IBranchOpcode::branch(
    ProgramContext *program_ctx,
    IStorageLocation *branch_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_address_t> *register_program_counter = nullptr;
    native_word_t register_program_counter_data = 0;
    native_address_t relative_branch_address = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != branch_operand_storage);

    /* Retrieve the current program counter. */
    register_program_counter = program_ctx->register_file.get_register_program_counter();
    register_program_counter_data = register_program_counter->read();

    /* Read relative branch address from operand. */
    status = branch_operand_storage->read(
        reinterpret_cast<native_word_t *>(&relative_branch_address),
        sizeof(relative_branch_address),
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read relative branch address failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Add the relative offset to the program counter. */
    register_program_counter->write(register_program_counter_data + relative_branch_address);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus IBranchOpcode::exec(
    ProgramContext *program_ctx,
    IStorageLocation *branch_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_status = nullptr;
    native_word_t register_status_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != branch_operand_storage);

    /* Retrieve the status register. */
    register_status = program_ctx->register_file.get_register_status();
    register_status_data = register_status->read();

    /* Check if the state of the status flag matches the condition specified by the opcode. */
    if (branch_on_set == (register_status_data & branch_condition_mask)) {
        status = this->branch(
            program_ctx,
            branch_operand_storage,
            operand_storage_offset
        );
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("branch failed. Status: %d\n", status);
            goto l_cleanup;
        }
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
