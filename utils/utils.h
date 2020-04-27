/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __UTILS_H__
#define __UTILS_H__

/** Headers ***************************************************************/
#include <cstddef>
#include <unordered_map>
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
class ObjectPool : private std::unordered_set<T *> {
public:
    explicit ObjectPool(std::size_t pool_size);

    ~ObjectPool();

    inline enum PeNESStatus retrieve(T **output_object);

    inline enum PeNESStatus release(T *release_object);

private:
    std::size_t pool_size;
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
        const std::array<BaseClass *(*)(), instance_factory_list_size> *object_instance_factory_list,
        std::initializer_list<TypeIndex> type_list
    );

    ~ObjectTable();

    inline enum PeNESStatus get_type(
        std::size_t table_index,
        TypeIndex *output_type
    ) const;

    inline enum PeNESStatus get_object(
        std::size_t table_index,
        BaseClass **output_object
    ) const;

    inline enum PeNESStatus get_object_by_type(
        TypeIndex type,
        BaseClass **output_object
    ) const;

    inline std::size_t get_size() const
    {
        return this->instance_table.size();
    };

private:
    const std::vector<TypeIndex> type_table;
    std::vector<BaseClass *> instance_table;
    std::unordered_map<TypeIndex, BaseClass *> type_instance_map;
};

}

#endif /* __UTILS_H__ */
