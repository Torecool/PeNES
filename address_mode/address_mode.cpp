/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include "utils/utils.h"

#include "address_mode/address_mode_interface.h"
#include "address_mode/absolute_address_mode.h"
#include "address_mode/indirect_address_mode.h"
#include "address_mode/zeropage_address_mode.h"
#include "address_mode/accumulator_address_mode.h"
#include "address_mode/immediate_address_mode.h"

#include "address_mode/address_mode.h"

/** Namespaces ************************************************************/
using namespace address_mode;

/** Static Variables ******************************************************/
const utils::InstanceFactoryList<enum AddressModeType, IAddressMode> AddressModeTable::address_mode_instance_factory_list = {
    nullptr,
    utils::SubClassFactory<IAddressMode>::create_instance<ImpliedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<AccumulatorAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<AbsoluteAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<AbsoluteXIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<AbsoluteYIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ImmediateSingleAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ImmediateDoubleAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<RelativeAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<IndirectAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<XIndexedIndirectAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<IndirectYIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ZeropageAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ZeropageXIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ZeropageYIndexedAddressMode>
};
