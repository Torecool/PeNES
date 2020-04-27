/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/** Headers ***************************************************************/
#include <cstdint>
#include <type_traits>
#include <byteswap.h>

#include "common.h"

/** Constants *************************************************************/
#define SYSTEM_NATIVE_WORD_SIZE_BITS (8)
#define SYSTEM_NATIVE_WORD_SIGN_BIT_MASK (1 << (SYSTEM_NATIVE_WORD_SIZE_BITS - 1))

/** Typedefs **************************************************************/
/* Note: This typedef should be unsigned char and not std::uint8_t because of the strict aliasing rule.
 * The standard defines that the a cast between pointers of unrelated types is only legal if the destination type is a pointer to a variant of char.
 * This is necessary in the read/write functions of the StorageLocation class,
 * where an address buffer (native_address_t) must be reinterpreted as a word buffer (native_word_t *).
 * */
typedef unsigned char native_word_t;
typedef std::uint16_t native_dword_t;
typedef native_dword_t native_address_t;


/** Functions *************************************************************/
static inline native_address_t system_big_to_native_endianness(native_address_t address)
{
    return bswap_16(address);
}


static inline native_address_t system_native_to_big_endianness(native_address_t address)
{
    return bswap_16(address);
}


static constexpr inline std::size_t system_bytes_to_words(std::size_t num_bytes)
{
    return num_bytes / sizeof(native_word_t) + MIN(num_bytes % sizeof(native_word_t), 1);
}


static constexpr inline std::size_t system_words_to_bytes(std::size_t num_words)
{
    return num_words * sizeof(native_word_t);
}


#endif /* __SYSTEM_H__ */
