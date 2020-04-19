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
namespace address_modes {

/** Classes ***************************************************************/
class AddressModeTable {
public:
    AddressModeTable(
        const std::initializer_list<IAddressMode *> address_mode_list
    ) : address_mode_list(address_mode_list) {};

    inline enum PeNESStatus get_address_mode(
        size_t index,
        const IAddressMode **output_address_mode
    ) const;

private:
    const std::vector<IAddressMode *> address_mode_list;
};

} /* namespace address_modes */

#endif /* __ADDRESS_MODE_H__ */
