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
#include "common.h"

/** Namespaces ************************************************************/
namespace utils {

/** Classes ***************************************************************/
template<class T>
class ObjectPool : private std::unordered_set<T *> {
public:
    explicit ObjectPool(std::size_t pool_size)
    {
        /* Fill the pool with pool_size object instances. */
        for (std::size_t index = 0; index < pool_size; index++) {
            this->insert(new T());
        }
    }

    ~ObjectPool()
    {}

    inline enum PeNESStatus retrieve(T **output_object){};

    inline enum PeNESStatus release(T *release_object){};

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
    ): type_table(type_list)
    {
        BaseClass *new_object = nullptr;
        BaseClass *(*instance_factory)() = nullptr;

        ASSERT(nullptr != object_instance_factory_list);

        /* Iterate through the argument type list.
         * For each type, find the corresponding instance factory method and create an instance.
         * Insert the new instance into a vector.
         * Additionally, insert the instance and corresponding type into a map for lookup by type.
         * */
        for (TypeIndex type_index : type_list) {

            ASSERT(instance_factory_list_size > type_index);

            instance_factory = object_instance_factory_list->at(type_index);

            new_object = nullptr;
            if (nullptr != instance_factory) {
                new_object = instance_factory();
            }

            this->instance_table.push_back(new_object);
            this->type_instance_map.insert({type_index, new_object});
        }
    }

    ~ObjectTable()
    {
        /* Iterate through the object vector and delete each instance. */
        for (BaseClass *object_instance : this->instance_table) {
            delete object_instance;
        }

        this->instance_table.clear();
    }

    enum PeNESStatus get_type(
        std::size_t table_index,
        TypeIndex *output_type
    ) const
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

        ASSERT(nullptr != output_type);

        /* Check if the parameter index is within the bounds of the type vector. */
        if (this->type_table.size() <= table_index) {
            status = PENES_STATUS_UTILS_OBJECT_TABLE_GET_TYPE_OUT_OF_BOUNDS;
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Table index out of bounds. Status: %d", status);
            goto l_cleanup;
        }

        /* Retrieve the object type from the vector table. */
        *output_type = this->type_table.at(table_index);

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    enum PeNESStatus get_object(
        std::size_t table_index,
        BaseClass **output_object
    ) const
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

        ASSERT(nullptr != output_object);

        /* Check if the parameter index is within the bounds of the object vector. */
        if (this->instance_table.size() <= table_index) {
            status = PENES_STATUS_UTILS_OBJECT_TABLE_GET_OBJECT_OUT_OF_BOUNDS;
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Table index out of bounds. Status: %d", status);
            goto l_cleanup;
        }

        /* Retrieve the object instance from the vector table. */
        *output_object = this->instance_table.at(table_index);

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    enum PeNESStatus get_object_by_type(
        TypeIndex type,
        BaseClass **output_object
    ) const
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        typename std::unordered_map<TypeIndex, BaseClass *>::const_iterator instance_iter = this->type_instance_map.find(type);

        ASSERT(nullptr != output_object);

        /* Check if the type exists in the map. */
        if (this->type_instance_map.end() == instance_iter) {
            status = PENES_STATUS_UTILS_OBJECT_TABLE_GET_OBJECT_BY_TYPE_NOT_FOUND;
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Object type not found. Status: %d", status);
            goto l_cleanup;
        }

        /* Retrieve the object instance from the map. */
        *output_object = instance_iter->second;

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    inline std::size_t get_size() const
    {
        return this->instance_table.size();
    };

private:
    /*TODO can probably be improved. */
    const std::vector<TypeIndex> type_table;
    std::vector<BaseClass *> instance_table;
    std::unordered_map<TypeIndex, BaseClass *> type_instance_map;
};

}

#endif /* __UTILS_H__ */
