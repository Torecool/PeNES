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

#include "instruction_set/alu_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IAddOpcode::add(ProgramContext *program_ctx, native_word_t add_operand)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_a = nullptr;
    RegisterStorage<native_word_t> *register_status = nullptr;
    native_word_t register_a_data = 0;
    native_word_t register_status_data = 0;
    native_dword_t operation_result = 0;
    bool is_carry_set = false;

    ASSERT(nullptr != program_ctx);

    /* Retrieve register A and the Status register from the program context. */
    register_a = program_ctx->register_file.get_register_a();
    register_status = program_ctx->register_file.get_register_status();

    /* Read the data stored in the registers. */
    register_a_data = register_a->read();
    register_status_data = register_status->read();

    /* Check if the carry flag is set, we need to add it to the sum. */
    is_carry_set = (register_status_data & REGISTER_STATUS_FLAG_MASK_CARRY);

    /* Perform the addition operation between the register and the data.
     * Add the carry flag to the sum if it is set.
     * */
    operation_result = register_a_data + add_operand;
    operation_result += (true == is_carry_set)? 1: 0;

    /* Check if we need to set the Overflow flag,
     * meaning that the ADD result is incorrect from a signed perspective.
     * This occurs when both operands have the same sign but the result is flipped.
     * */
    if ((register_a_data & SYSTEM_NATIVE_WORD_SIGN_BIT_MASK) == (add_operand & SYSTEM_NATIVE_WORD_SIGN_BIT_MASK) &&
        (add_operand & SYSTEM_NATIVE_WORD_SIGN_BIT_MASK) != (operation_result & SYSTEM_NATIVE_WORD_SIGN_BIT_MASK)) {
        update_values |= REGISTER_STATUS_FLAG_MASK_OVERFLOW;
    }

    /* Write the result back to the register. */
    register_a->write(operation_result);

    /* Call the parent function to update the status flags. */
    status = this->update_status(register_status, operation_result);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeADC::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Read the data at the storage location. */
    status = data_operand_storage->read(
        &storage_data,
        sizeof(storage_data),
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Call the parent function to perform the addition operation and update the status flags. */
    status = this->add(program_ctx, storage_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass add failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus OpcodeSBC::exec(
    ProgramContext *program_ctx,
    IStorageLocation *data_operand_storage,
    std::size_t operand_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    native_word_t storage_data = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != data_operand_storage);

    /* Read the data at the storage location. */
    status = data_operand_storage->read(
        &storage_data,
        sizeof(storage_data),
        operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Call the parent function to perform the addition operation and update the status flags.
     * Note: Based on the identity -M == ~M + 1, we can just treat the subtraction operation as an addition operation
     * with the complement of the operand:
     * A - M - B == A + ~M + 1 - B == A + ~M + (1-B) == A + ~M + ~B == A + ~M + C
     * Note that we treat the "Borrow flag" as the complement of the Carry flag.
     * */
    status = this->add(program_ctx, ~storage_data);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass add failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


