/**
 * @brief  
 * @author TBK
 * @date   18/04/2020
 * */

/** Headers ***************************************************************/
#include "penes_status.h"
#include "common.h"

#include "address_mode/address_mode_interface.h"
#include "address_mode/address_mode.h"

/** Namespaces ************************************************************/
using namespace address_modes;

/** Functions *************************************************************/
enum PeNESStatus AddressModeTable::get_address_mode(
    size_t index,
    const IAddressMode **output_address_mode
) const
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    const IAddressMode *address_mode = nullptr;

    ASSERT(nullptr != output_address_mode);

    /* Try to find the address mode. */
    address_mode = this->address_mode_list.at(index);
    if (nullptr == address_mode) {
        status = PENES_STATUS_ADDRESS_MODE_TABLE_GET_ADDRESS_MODE_NO_MODE;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "No mode found at the given index. Status: %d. Mode: %zu\n",
            status,
            index
        );
        goto l_cleanup;
    }

    *output_address_mode = address_mode;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
