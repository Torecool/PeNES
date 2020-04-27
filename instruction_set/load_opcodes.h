/**
 * @brief  Definitions for load-related opcodes.
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __LOAD_OPCODES_H__
#define __LOAD_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"
#include "instruction_set/operation_types.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing a load operation.
 *         Extends the standard data-status-updating opcode interface by adding the load method.
 * */
class ILoadOpcode : public IOpcode, public IUpdateDataStatusOperation {
public:
    /** @brief          Load a WORD of data from a storage location into a register and update the Status register accordingly.
     *
     *  @param[in]      program_ctx                 The program context containing the Status register to update.
     *  @param[in]      load_register               The register to load the data into.
     *  @param[in]      load_storage                The storage location containing the data to load.
     *  @param[in]      storage_offset              The offset within the storage location to read the data from.
     *
     *  @return         Status indicating the success of the operation.
     * */
    enum PeNESStatus load(
        ProgramContext *program_ctx,
        RegisterStorage<native_word_t> *load_register,
        IStorageLocation *load_storage,
        std::size_t storage_offset = 0
    );
};

/** @brief Load Accumulator with Data. */
class OpcodeLDA : public ILoadOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Load register X with Data. */
class OpcodeLDX : public ILoadOpcode {
public:
    inline address_mode::AddressModeType resolve_address_mode(
        address_mode::AddressModeType default_address_mode
    ) override
    {
        /* If the address mode was predicted as zp,X, this should actually be zp,Y.
         * Also, if the address mode was predicted as abs,X, this should actually be abs,Y.
         * The reason for this is that the opcode's address mode bit pattern fits the otherwise zp,X/abs,X templates.
         * */
        if (address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED == default_address_mode) {
            return address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_Y_INDEXED;
        } else if (address_mode::AddressModeType::ADDRESS_MODE_TYPE_ABSOLUTE_X_INDEXED == default_address_mode) {
            return address_mode::AddressModeType::ADDRESS_MODE_TYPE_ABSOLUTE_Y_INDEXED;
        } else {
            return default_address_mode;
        }
    }

    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Load register Y with Data. */
class OpcodeLDY : public ILoadOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *data_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __LOAD_OPCODES_H__ */
