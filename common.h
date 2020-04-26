/**
 * @brief  Commonly used macros and constants.
 * @author TBK
 * @date   18/04/2020
 * */

#ifndef __COMMON_H__
#define __COMMON_H__

/** Constants *************************************************************/
/* Standard failure return value for libc functions. */
#define C_STANDARD_FAILURE (-1)

/* Standard success return value for libc functions. */
#define C_STANDARD_SUCCESS (0)

/* Errno reset value, indicating that no error has occured. */
#define ERRNO_VALUE_NOT_SET (0)

/* Return value of comparison functions such as strcmp, indicating that the operands are equal. */
#define CMP_EQUAL (0)

/* Decimal counting base, used primarily for calls to strtoul. */
#define DECIMAL_BASE (10)

/* Null-terminating char, indicating the end of a string. */
#define NULL_TERMINATOR '\0'

/* Directory separation char, indicating a partition between a parent directory and a file/sub-directory. */
#define DIRPATH_SEPARATOR '/'

/* Path resolution wildcard that expands into the current working directory. */
#define CWD_WILDCARD '.'

/* Int value indicating an invalid fd. */
#define INVALID_FILE_DESCRIPTOR (-1)

/* Int value indicating an invalid pid. */
#define INVALID_PROCESS_ID (-1)

/* Int value returned from fork, indicating that the process is the child and not the parent. */
#define CHILD_PROCESS_ID (0)

/* static keyword synonym, that is replaced with nothing in debug mode,
 * so that the symbols will be recognized globally.
 * */
#ifdef _DEBUG
#define STATIC
#else
#define STATIC static
#endif

/** Macros ****************************************************************/
/* Copy a buffer of memory. */
#define COPY_MEMORY(dest, src, size) ((void)memcpy(dest, src, size))
#ifdef COPY_MEMORY
#include <cstring>
#endif

/* Dynamically allocate and initialize memory on the heap. */
#define HEAPALLOCZ(size) (calloc(1, (size)))
#ifdef HEAPALLOCZ
#include <cstdlib>
#endif

/* Free memory that has been dynamically allocated on the heap. */
#define HEAPFREE(ptr)                                       \
    do {                                                    \
        if (NULL != (ptr)) {                                \
            (void) free(ptr);                                \
            (ptr) = NULL;                                   \
        }                                                   \
    } while (0)
#ifdef HEAPFREE
#include <cstdlib>
#endif

/* Perform a debug print, with the value of errno as well as additionally formatted args. */
#ifdef _DEBUG
#include <cstdio>
#include <cerrno>
#define DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(format, args...)   \
    do {                                                    \
        fprintf(                                            \
            stderr,                                         \
            "DEBUG: %s:%d:%s(). Errno: %d: " format,        \
            __FILE__,                                       \
            __LINE__,                                       \
            __func__,                                       \
            errno,                                          \
            ##args                                          \
        );                                                  \
    } while (0)
#else
#define DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(...) do {} while(0)
#endif

/* Perform a debug print with additionally formatted args. */
#ifdef _DEBUG
#include <cstdio>
#define DEBUG_PRINT_WITH_ARGS(format, args...)              \
    do {                                                    \
        fprintf(                                            \
            stderr,                                         \
            "DEBUG: %s:%d:%s(): " format,                   \
            __FILE__,                                       \
            __LINE__,                                       \
            __func__,                                       \
            ##args                                          \
        );                                                  \
    } while (0)
#else
#define DEBUG_PRINT_WITH_ARGS(...) do {} while(0)
#endif

/* Perform an assertion of a given condition. */
#ifdef _DEBUG
#include <cassert>
#define ASSERT(cond)                                        \
    do {                                                    \
        assert(cond);                                       \
    } while (0)
#else
#define ASSERT(cond) do {} while(0)
#endif

/* Eliminate the compile-time warning of an unused parameter. */
#define UNREFERENCED_PARAMETER(param)                       \
    do {                                                    \
        (param) = (param);                                  \
    } while (0)

/* Determine which item is smaller by integer comparison. */
#define MIN(a, b) (((a) <= (b))? (a): (b))

/* Determine which item is bigger by integer comparison. */
#define MAX(a, b) (((a) >= (b))? (a): (b))

#endif /* __COMMON_H__ */

