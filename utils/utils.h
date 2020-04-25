/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __UTILS_H__
#define __UTILS_H__

/** Headers ***************************************************************/
#include <cstddef>
#include <unordered_set>
#include <vector>

#include "penes_status.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
namespace utils {

template<class T>
class ObjectPool : private std::unordered_set<T> {
public:
    explicit ObjectPool(std::size_t pool_size);

    inline enum PeNESStatus retrieve(T **output_object);

    inline enum PeNESStatus release(T *release_object);
};


template<class BaseClass>
class SubClassFactory {
public:
    template<class SubClass>
    static inline BaseClass *create_instance()
    {
        return new SubClass();
    }
};


template<class BaseClass, typename TypeIndex>
class ObjectTable {
public:
    template<size_t instance_factory_list_size>
    ObjectTable(
        const std::array<BaseClass *(*)(), instance_factory_list_size> &object_instance_factory_list,
        const std::initializer_list<TypeIndex> &type_list
    );

    ~ObjectTable();

    inline enum PeNESStatus get_object(
        std::size_t table_index,
        BaseClass **output_object
    ) const;

private:
    std::vector<BaseClass *> object_instance_table;
};

}

#endif /* __UTILS_H__ */
