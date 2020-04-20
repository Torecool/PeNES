/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

/** Headers ***************************************************************/
#include <unordered_set>

#include "penes_status.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
namespace utils {

template <class T>
class ObjectPool : private std::unordered_set<T> {
public:
    explicit ObjectPool(std::size_t pool_size);

    inline enum PeNESStatus retrieve(T **output_object);

    inline enum PeNESStatus release(T *release_object);
};

}


#endif /* __OBJECT_POOL_H__ */
