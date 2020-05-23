/**
 * @brief  
 * @author TBK
 * @date   19/04/2020
 * */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/** Headers ***************************************************************/
#include <cstdint>
#include <climits>
#include <type_traits>
#include <byteswap.h>

#include "common.h"

/** Constants *************************************************************/
#define SYSTEM_NATIVE_WORD_SIZE_BITS (8)
#define SYSTEM_NATIVE_WORD_SIGN_BIT_MASK (1 << (SYSTEM_NATIVE_WORD_SIZE_BITS - 1))
#define SYSTEM_IS_NATIVE_LITTLE_ENDIAN (true)
#define SYSTEM_NATIVE_WORD_NUM_WORDS (1)
#define SYSTEM_NATIVE_DWORD_NUM_WORDS (2)
#define SYSTEM_NATIVE_ADDRESS_NUM_WORDS (SYSTEM_NATIVE_DWORD_NUM_WORDS)
/** Typedefs **************************************************************/
static_assert(CHAR_BIT == SYSTEM_NATIVE_WORD_SIZE_BITS, "Host char size must match native char size.");

/* Note: This typedef should be unsigned char and not std::uint8_t because of the strict aliasing rule.
 * The standard defines that the a cast between pointers of unrelated types is only legal if the destination type is a pointer to a variant of char.
 * This is necessary in the read/write functions of the StorageLocation class,
 * where an address buffer (native_address_t) must be reinterpreted as a word buffer (native_word_t *).
 * */
typedef unsigned char native_word_t;
typedef signed char native_signed_word_t;
typedef std::uint16_t native_dword_t;
typedef std::int16_t native_signed_dword_t;
typedef native_dword_t native_address_t;


/** Functions *************************************************************/
static bool system_is_host_little_endian()
{
    std::uint16_t magic = 0x1;
    char *magic_ptr = reinterpret_cast<char *>(&magic);

    /* Check if reinterpreting the data has changed the order of bytes within it. */
    return (magic_ptr[0] == magic);
}


static inline native_dword_t system_native_to_host_endianness(native_dword_t dword_data)
{
    static bool is_little_endian = system_is_host_little_endian();

    return (SYSTEM_IS_NATIVE_LITTLE_ENDIAN == is_little_endian) ? dword_data : bswap_16(dword_data);
}


static inline native_dword_t system_host_to_native_endianness(native_dword_t dword_data)
{
    return system_native_to_host_endianness(dword_data);
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
