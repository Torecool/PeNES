/**
 * @brief  Definitions for store-related opcodes.
 * @author TBK
 * @date   23/04/2020
 * */

#ifndef __STORE_OPCODES_H__
#define __STORE_OPCODES_H__

/** Headers ***************************************************************/
#include <cstddef>

#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Classes ***************************************************************/
/** @brief Interface of an opcode performing a store operation.
 *         Extends the standard opcode interface by adding the store method.
 * */
class IStoreOpcode : public IOpcode {
protected:
    /** @brief          Store a WORD of data from a register into a storage location.
     *
     *  @param[in]      store_register              The register containing the data to store.
     *  @param[in]      store_storage               The storage location to write the data to.
     *  @param[in]      storage_offset              The offset within the storage location to write the data to.
     *
     *  @return         Status indicating the success of the operation.
     * */
    static enum PeNESStatus store(
        RegisterStorage<native_word_t> *store_register,
        IStorageLocation *store_storage,
        std::size_t storage_offset
    );
};

/** @brief Store Accumulator in memory. */
class OpcodeSTA : public IStoreOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *store_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Store Index X in memory. */
class OpcodeSTX : public IStoreOpcode {
public:
    inline address_mode::AddressModeType resolve_address_mode(
        address_mode::AddressModeType default_address_mode
    ) override
    {
        /* If the address mode was predicted as zp,X, this should actually be zp,Y.
         * The reason for this is that the opcode's address mode bit pattern fits the otherwise zp,X template.
         * */
        if (address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED == default_address_mode) {
            return address_mode::AddressModeType::ADDRESS_MODE_TYPE_ZEROPAGE_Y_INDEXED;
        } else {
            return default_address_mode;
        }
    }

    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *store_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

/** @brief Store Index Y in memory. */
class OpcodeSTY : public IStoreOpcode {
public:
    enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *store_operand_storage,
        std::size_t operand_storage_offset
    ) override;
};

} /* namespace instruction_set */

#endif /* __STORE_OPCODES_H__ */
