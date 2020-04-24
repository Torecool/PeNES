/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/** Headers ***************************************************************/
#include <cstdint>
#include <byteswap.h>

/** Constants *************************************************************/
#define SYSTEM_NATIVE_WORD_SIZE_BITS (8)

/** Typedefs **************************************************************/
typedef std::uint8_t native_word_t;
typedef std::uint16_t native_dword_t;
typedef native_dword_t native_address_t;


/** Functions *************************************************************/
static inline native_address_t system_big_to_native_endianess(native_address_t address)
{
    return bswap_16(address);
}


static inline native_address_t system_native_to_big_endianess(native_address_t address)
{
    return bswap_16(address);
}

#endif /* __SYSTEM_H__ */
