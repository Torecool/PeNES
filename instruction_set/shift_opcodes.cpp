/**
 * @brief  
 * @author TBK
 * @date   24/04/2020
 * */

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"
#include "common.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

#include "instruction_set/shift_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Functions *************************************************************/
enum PeNESStatus IShiftOpcode::exec(
    ProgramContext *program_ctx,
    IStorageLocation *shift_storage,
    std::size_t storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    RegisterStorage<native_word_t> *register_status = nullptr;
    native_word_t register_status_data = 0;
    native_word_t shift_storage_data = 0;
    native_word_t shift_result_truncated = 0;
    native_dword_t shift_result = 0;

    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != shift_storage);

    /* Retrieve the Status register from the program context. */
    register_status = program_ctx->register_file.get_register_status();

    /* Read the Status register. */
    register_status_data = register_status->read();

    /* Read the data at the shift storage location. */
    status = shift_storage->read(
        &shift_storage_data,
        sizeof(shift_storage_data),
        storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read data storage failed. Status: %d\n", status);
        goto l_cleanup;
    }

    /* Perform the shift operation into a larger data type, in order to retain the carry. */
    shift_result = this->operation(register_status_data, shift_storage_data);

    /* Truncate the carry bit from the result so that we can write it back to the data storage. */
    shift_result_truncated = static_cast<native_word_t>(shift_result);

    /* Write the truncated result back to the data storage. */
    status = shift_storage->write(
        &shift_result_truncated,
        sizeof(shift_result_truncated),
        storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("write data storage failed. Status: %d", status);
        goto l_cleanup;
    }

    /* Call the parent function with the extended return value to update the status flags. */
    status = this->update_status(register_status, shift_result);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Superclass update_status failed. Status: %d", status);
        goto l_cleanup;
    }

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
