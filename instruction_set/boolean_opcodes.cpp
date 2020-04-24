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

#include "instruction_set/boolean_opcodes.h"

/** Constants *************************************************************/
#define BOOLEAN_OPCODES_BIT_OPCODE_BIT_6_MASK (1 << 6)
#define BOOLEAN_OPCODES_BIT_OPCODE_BIT_7_MASK (1 << 7)

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IBooleanOpcode::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;
    native_word_t register_a_data = 0;
    native_word_t storage_data = 0;
    native_dword_t operation_result = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register A from the program context. */
    register_a = program_ctx->register_file.get_register_a();

    /* Read the data stored in the register. */
    register_a_data = register_a->read();

    /* Read the data at the storage location. */
    status = data_operand_storage->read(
        &storage_data,
        sizeof(storage_data),
        storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Perform the ALU operation between the register and the data. */
    operation_result = this->operation(register_a_data, storage_data);

    /* Write the result back to the register. */
    register_a->write(operation_result);

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, operation_result);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeBIT::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;
    native_word_t register_a_data = 0;
    native_word_t storage_data = 0;
    native_dword_t and_result = 0;
    bool is_bit6_set = false;
    bool is_bit7_set = false;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Retrieve register A from the program context. */
    register_a = program_ctx->register_file.get_register_a();

    /* Read the data stored in the register. */
    register_a_data = register_a->read();

    /* Read the data at the storage location. */
    status = data_operand_storage->read(
        &storage_data,
        sizeof(storage_data),
        storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Perform the AND operation between the register and the data. */
    and_result = this->operation(register_a_data, storage_data);

    /* Set the Status register's Overflow flag and Negative flag,
     * based on the 6th and 7th bit of the storage data respectively.
     * */
    is_bit6_set = (storage_data & BOOLEAN_OPCODES_BIT_OPCODE_BIT_6_MASK);
    is_bit7_set = (storage_data & BOOLEAN_OPCODES_BIT_OPCODE_BIT_7_MASK);

    update_values |= (true == is_bit6_set)? REGISTER_STATUS_FLAG_MASK_OVERFLOW: 0;
    update_values |= (true == is_bit7_set)? REGISTER_STATUS_FLAG_MASK_NEGATIVE: 0;

    /* Call the parent function to update the status flags. */
    status = this->update_status(program_ctx, and_result);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
