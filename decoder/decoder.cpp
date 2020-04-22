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
static const address_mode::AddressModeTable address_mode_table0 = {
    new address_mode::ImmediateAddressMode(),
    new address_mode::ZeropageAddressMode(),
    nullptr,
    new address_mode::AbsoluteAddressMode(),
    nullptr,
    new address_mode::ZeropageXIndexedAddressMode(),
    nullptr,
    new address_mode::AbsoluteXIndexedAddressMode()
};

static const address_mode::AddressModeTable address_mode_table1 = {
    new address_mode::XIndexedIndirectAddressMode(),
    new address_mode::ZeropageAddressMode(),
    new address_mode::ImmediateAddressMode(),
    new address_mode::AbsoluteAddressMode(),
    new address_mode::IndirectYIndexedAddressMode(),
    new address_mode::ZeropageXIndexedAddressMode(),
    new address_mode::AbsoluteYIndexedAddressMode(),
    new address_mode::AbsoluteXIndexedAddressMode()
};

static const address_mode::AddressModeTable address_mode_table2 = {
    new address_mode::ImmediateAddressMode(),
    new address_mode::ZeropageAddressMode(),
    nullptr,
    new address_mode::AbsoluteAddressMode(),
    nullptr,
    new address_mode::ZeropageXIndexedAddressMode(),
    nullptr,
    new address_mode::AbsoluteXIndexedAddressMode()
};

Decoder::Decoder()
{
}

