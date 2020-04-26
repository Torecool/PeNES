/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include <array>

#include "penes_status.h"
#include "common.h"

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
static const std::array<address_mode_instance_factory_t, ADDRESS_MODE_TYPE_NUM_ADDRESS_MODES> address_mode_instance_factory_table = {
    nullptr,
    utils::SubClassFactory<IAddressMode>::create_instance<AccumulatorAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<AbsoluteAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<AbsoluteXIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<AbsoluteYIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ImmediateAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ImmediateDoubleAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<RelativeAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<IndirectAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<XIndexedIndirectAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<IndirectYIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ZeropageAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ZeropageXIndexedAddressMode>,
    utils::SubClassFactory<IAddressMode>::create_instance<ZeropageYIndexedAddressMode>
};

/** Functions *************************************************************/

