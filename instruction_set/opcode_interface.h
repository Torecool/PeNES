/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __OPCODE_INTERFACE_H__
#define __OPCODE_INTERFACE_H__

/** Headers ***************************************************************/
#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "address_mode/address_mode.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
class IOpcode {
public:
    inline virtual address_modes::IAddressMode *resolve_address_mode(

        address_modes::IAddressMode *default_address_mode = nullptr
    );

    inline virtual void exec(
        ProgramContext* program_ctx
    );

private:
    const address_modes::IAddressMode *address_mode = nullptr;
    const IStorageLocation *operand_storage = nullptr;
    std::size_t operand_storage_offset = 0;
};


#endif /* __OPCODE_INTERFACE_H__ */
