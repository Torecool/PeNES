/**
 * @brief  
 * @author TBK
 * @date   22/04/2020
 * */

#ifndef __ALU_OPCODES_H__
#define __ALU_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
namespace instruction_set {

class OpcodeADC : public IOpcode {

};


class OpcodeAND : public IOpcode {
    inline enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        RegisterStorage *register_a = nullptr;
        native_word_t register_a_data = 0;
        native_word_t operand_data = 0;
        native_word_t result = 0;

        register_a = program_ctx->register_file.get_register_a();

        /* Read contents of register a. */
        status = register_a->read(&register_a_data, sizeof(register_a_data));
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read from register A failed. Status: %d\n", status);
            goto l_cleanup;
        }

        /* Read contents of operand storage. */
        status = operand_storage->read(&operand_data, sizeof(operand_data), operand_storage_offset);
        if (PENES_STATUS_SUCCESS != status) {
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("read from operand storage failed. Status: %d\n", status);
            goto l_cleanup;
        }

        result = register_a_data & operand_data;




        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    };
};

}


#endif /* __ALU_OPCODES_H__ */
