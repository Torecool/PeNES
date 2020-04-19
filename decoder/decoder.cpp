/**
 * @brief  
 * @author TBK
 * @date   17/04/2020
 * */


/** Headers ***************************************************************/
#include "decoder/decoder.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
static const address_modes::AddressModeTable address_mode_table0 = {
    new address_modes::ImmediateAddressMode(),
    new address_modes::ZeropageAddressMode(),
    nullptr,
    new address_modes::AbsoluteAddressMode(),
    nullptr,
    new address_modes::ZeropageXIndexedAddressMode(),
    nullptr,
    new address_modes::AbsoluteXIndexedAddressMode()
};

static const address_modes::AddressModeTable address_mode_table1 = {
    new address_modes::XIndexedIndirectAddressMode(),
    new address_modes::ZeropageAddressMode(),
    new address_modes::ImmediateAddressMode(),
    new address_modes::AbsoluteAddressMode(),
    new address_modes::IndirectYIndexedAddressMode(),
    new address_modes::ZeropageXIndexedAddressMode(),
    new address_modes::AbsoluteYIndexedAddressMode(),
    new address_modes::AbsoluteXIndexedAddressMode()
};

static const address_modes::AddressModeTable address_mode_table2 = {
    new address_modes::ImmediateAddressMode(),
    new address_modes::ZeropageAddressMode(),
    nullptr,
    new address_modes::AbsoluteAddressMode(),
    nullptr,
    new address_modes::ZeropageXIndexedAddressMode(),
    nullptr,
    new address_modes::AbsoluteXIndexedAddressMode()
};

Decoder::Decoder()
{
    address_mode_table0  = {
        new address_modes::ImmediateAddressMode(),
        new address_modes::ZeropageAddressMode(),
        nullptr,
        new address_modes::AbsoluteAddressMode(),
        nullptr,
        new address_modes::ZeropageXIndexedAddressMode(),
        nullptr,
        new address_modes::AbsoluteXIndexedAddressMode()
    };
}

