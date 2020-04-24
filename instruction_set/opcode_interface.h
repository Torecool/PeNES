/**
 * @brief  
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
using namespace address_mode;

namespace instruction_set {

/** Classes ***************************************************************/
class IOpcode {
public:
    inline virtual AddressModeType resolve_address_mode(
        AddressModeType default_address_mode = AddressModeType::ADDRESS_MODE_TYPE_NONE
    ) {
        return default_address_mode;
    }

    inline virtual enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) {};
};


class IUpdateStatusOpcode : public IOpcode {
public:
    inline enum PeNESStatus exec(
        ProgramContext *program_ctx,
        IStorageLocation *operand_storage,
        std::size_t operand_storage_offset
    ) override;

protected:
    inline virtual enum PeNESStatus update_status(RegisterStorage<native_word_t> *register_status);

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NONE;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


class IUpdateDataStatusOpcode : public IUpdateStatusOpcode {
protected:
    inline enum PeNESStatus update_status(
        RegisterStorage<native_word_t> *register_status,
        native_dword_t opcode_result
    );

    inline enum PeNESStatus update_status(
        ProgramContext *program_ctx,
        native_dword_t opcode_result
    );

private:
    native_word_t update_mask = REGISTER_STATUS_FLAG_MASK_NEGATIVE | REGISTER_STATUS_FLAG_MASK_ZERO;
    native_word_t update_values = REGISTER_STATUS_FLAG_MASK_NONE;
};


class IImpliedOperandOpcode : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override {
        return AddressModeType::ADDRESS_MODE_TYPE_IMPLIED;
    }
};


class OpcodeNOP : public IOpcode {
public:
    inline AddressModeType resolve_address_mode(AddressModeType default_address_mode) override
    {
        return AddressModeType::ADDRESS_MODE_TYPE_NONE;
    }
};

}

#endif /* __OPCODE_INTERFACE_H__ */
