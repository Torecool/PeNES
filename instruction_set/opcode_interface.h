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

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
using namespace address_mode;

namespace instruction_set {

enum FlagRegisterBitMask {
    FLAG_REGISTER_BIT_MASK_NONE = 0,
    FLAG_REGISTER_BIT_MASK_CARRY = 0x1,
    FLAG_REGISTER_BIT_MASK_ZERO = 0x10,
    FLAG_REGISTER_BIT_MASK_INTERRUPT = 0x100,
    FLAG_REGISTER_BIT_MASK_DECIMAL = 0x1000,
    FLAG_REGISTER_BIT_MASK_BREAK = 0x10000,
    FLAG_REGISTER_BIT_MASK_UNUSED = 0x100000,
    FLAG_REGISTER_BIT_MASK_OVERFLOW = 0x1000000,
    FLAG_REGISTER_BIT_MASK_NEGATIVE = 0x1000000
};


class IOpcode {
public:
    inline virtual AddressModeType resolve_address_mode(
        AddressModeType default_address_mode = AddressModeType::ADDRESS_MODE_TYPE_NONE
    ) {
        return AddressModeType::ADDRESS_MODE_TYPE_NONE;
    }

    inline virtual enum PeNESStatus exec(
        ProgramContext* program_ctx,
        IStorageLocation *operand_storage = nullptr,
        std::size_t operand_storage_offset = 0
    ) {};
};


class OpcodeNOP : public IOpcode {};

}

#endif /* __OPCODE_INTERFACE_H__ */
