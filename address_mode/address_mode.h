/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __ADDRESS_MODE_H__
#define __ADDRESS_MODE_H__

/** Headers ***************************************************************/
#include <cstdint>
#include <vector>
#include <initializer_list>
#include <byteswap.h>

#include "penes_status.h"
#include "common.h"

#include "program_context/program_context.h"
#include "address_mode/address_mode_interface.h"
#include "utils/utils.h"

/** Namespaces ************************************************************/
namespace address_mode {

enum AddressModeType {
    ADDRESS_MODE_TYPE_NONE = -1,
    ADDRESS_MODE_TYPE_IMPLIED = 0,
    ADDRESS_MODE_TYPE_ACCUMULATOR,
    ADDRESS_MODE_TYPE_ABSOLUTE,
    ADDRESS_MODE_TYPE_ABSOLUTE_X_INDEXED,
    ADDRESS_MODE_TYPE_ABSOLUTE_Y_INDEXED,
    ADDRESS_MODE_TYPE_IMMEDIATE,
    ADDRESS_MODE_TYPE_IMMEDIATE_DOUBLE,
    ADDRESS_MODE_TYPE_RELATIVE,
    ADDRESS_MODE_TYPE_INDIRECT,
    ADDRESS_MODE_TYPE_X_INDEXED_INDIRECT,
    ADDRESS_MODE_TYPE_INDIRECT_Y_INDEXED,
    ADDRESS_MODE_TYPE_ZEROPAGE,
    ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED,
    ADDRESS_MODE_TYPE_ZEROPAGE_Y_INDEXED,
    ADDRESS_MODE_TYPE_NUM_ADDRESS_MODES
};

/** Typedefs **************************************************************/
typedef IAddressMode *(*address_mode_instance_factory_t)();

/** Classes ***************************************************************/
class AddressModeTable : public utils::ObjectTable<IAddressMode, enum AddressModeType> {
public:
    AddressModeTable(std::initializer_list<enum AddressModeType> address_mode_types):
        ObjectTable<IAddressMode, enum AddressModeType>(
            &address_mode_instance_factory_table,
            address_mode_types
        )
    {};

private:
    static const std::array<address_mode_instance_factory_t, ADDRESS_MODE_TYPE_NUM_ADDRESS_MODES> address_mode_instance_factory_table;
};

} /* namespace address_modes */

#endif /* __ADDRESS_MODE_H__ */
