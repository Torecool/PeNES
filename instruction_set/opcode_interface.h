/**
 * @brief  General opcode interfaces and definitions.
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __OPCODE_INTERFACE_H__
#define __OPCODE_INTERFACE_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "address_mode/address_mode.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief General interface for all instruction set opcodes.
 *         The interface includes a method for resolving the opcode's address mode based on the predicted address mode,
 *         and a method for executing the opcode instruction.
 * */
class IOpcode {
public:
    /** @brief          Given a default address mode predicted from the decode tables,
     *                  each opcode may decide to use the predicted address mode or override it in special cases (such as LDX and STX).
     *
     *  @param[in]      default_address_mode        The predicted, default address mode.
     *
     *  @return         The resolved address mode that should be used.
     * */
    inline virtual address_mode::AddressModeType resolve_address_mode(
        address_mode::AddressModeType default_address_mode
    )
    {
        return default_address_mode;
    }

    /** @brief          Execute the opcode instruction.
     *
     *  @param[in]      program_ctx                 The context object representing the program instance.
     *  @param[in]      operand_storage             The generic "storage location" resolved from the address mode.
     *                                              May represent a memory location, register, immediate, etc.
     *  @param[in]      operand_storage_offset      The offset within the storage location operand_storage that contains the data.
     *
     *  @return         Status indicating the success of the operation.
     * */
    inline virtual enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    )
    {
        return PENES_STATUS_SUCCESS;
    };
};

/** @brief General interface for all instruction set opcodes with an implied-only address mode.
 *         The interface overrides the superclass address mode resolution method to always return the implied address mode.
 * */
class IImpliedOperandOpcode : public IOpcode {
public:
    inline address_mode::AddressModeType resolve_address_mode(address_mode::AddressModeType default_address_mode) override {
        return address_mode::AddressModeType::ADDRESS_MODE_TYPE_IMPLIED;
    }
};

/** @brief Perform no operation. */
class OpcodeNOP : public IImpliedOperandOpcode {};

} /* namespace instruction_set */

#endif /* __OPCODE_INTERFACE_H__ */
