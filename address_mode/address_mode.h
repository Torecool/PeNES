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
#include "address_mode/absolute_address_mode.h"
#include "address_mode/indirect_address_mode.h"
#include "address_mode/zeropage_address_mode.h"
#include "address_mode/accumulator_address_mode.h"
#include "address_mode/immediate_address_mode.h"

/** Namespaces ************************************************************/
namespace address_mode {

enum class AddressModeType {
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
    ADDRESS_MODE_TYPE_ZEROPAGE_Y_INDEXED
};

/** Classes ***************************************************************/
class AddressModeTable {
public:
    AddressModeTable(
        const std::initializer_list<IAddressMode *> address_mode_list
    ) : address_mode_list(address_mode_list) {};

    inline enum PeNESStatus get_address_mode(
        enum AddressModeType address_mode_type,
        const IAddressMode **output_address_mode
    ) const;

private:
    const std::vector<IAddressMode *> address_mode_list;
};

} /* namespace address_modes */

#endif /* __ADDRESS_MODE_H__ */
