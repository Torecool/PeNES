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

/** Enums *****************************************************************/
/** @brief Enum representing all supported address modes,
 *  to be used as an index into the InstanceFactoryList address_mode_instance_factory_list.
 * */
enum AddressModeType {
    ADDRESS_MODE_TYPE_NONE = 0,
    ADDRESS_MODE_TYPE_IMPLIED,
    ADDRESS_MODE_TYPE_ACCUMULATOR,
    ADDRESS_MODE_TYPE_ABSOLUTE,
    ADDRESS_MODE_TYPE_ABSOLUTE_X_INDEXED,
    ADDRESS_MODE_TYPE_ABSOLUTE_Y_INDEXED,
    ADDRESS_MODE_TYPE_IMMEDIATE_SINGLE,
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

/** Classes ***************************************************************/
/** @brief Object representing a single address mode table,
 *  mapping a set of address mode encodings to their corresponding address mode objects.
 * */
class AddressModeTable : public utils::ObjectTable<enum AddressModeType, IAddressMode> {
public:
    AddressModeTable(std::initializer_list<enum AddressModeType> address_mode_types):
        ObjectTable(&address_mode_instance_factory_list, address_mode_types)
    {};

private:
    /** Internal data structure of adress mode type - address mode object mappings, for querying based on encoding. */
    static const utils::InstanceFactoryList<enum AddressModeType, IAddressMode> address_mode_instance_factory_list;
};

} /* namespace address_modes */

#endif /* __ADDRESS_MODE_H__ */
