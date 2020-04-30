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


template<typename TypeIndex, class BaseClass>
class InstanceFactoryList {
public:
    InstanceFactoryList(std::initializer_list<BaseClass *(*)()> instance_function_list):
        instance_functions(instance_function_list)
    {};

    inline enum PeNESStatus create_instance(
        TypeIndex instance_type_index,
        BaseClass **output_instance
    ) const
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        BaseClass *(*instance_function)() = nullptr;
        BaseClass *new_instance = nullptr;

        ASSERT(nullptr != output_instance);

        /* Check if the parameter type index is within the bounds of the vector. */
        if ((this->instance_functions.size() <= instance_type_index) ||
            (0 > instance_type_index)) {
            status = PENES_STATUS_UTILS_INSTANCE_FACTORY_LIST_CREATE_INSTANCE_OUT_OF_BOUNDS;
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Type index out of bounds. Status: %d", status);
            goto l_cleanup;
        }

        /* Retrieve the object instance function and initialize it.
         * If the instance function is null, return null.
         * */
        instance_function = this->instance_functions.at(instance_type_index);
        if (nullptr != instance_function) {
            new_instance = instance_function();
        }

        *output_instance = new_instance;

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

private:
    const std::vector<BaseClass *(*)()> instance_functions;
};


template<typename TypeIndex, class BaseClass>
class ObjectTable {
public:
    ObjectTable(
        const InstanceFactoryList<TypeIndex, BaseClass> *instance_factory_list,
        std::initializer_list<TypeIndex> type_list
    ): instance_factory_list(instance_factory_list)
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        BaseClass *new_object = nullptr;

        ASSERT(nullptr != instance_factory_list);

        /* Iterate through the argument type list.
         * For each type, find the corresponding instance factory method and create an instance.
         * Insert the new instance into a table.
         * Additionally, insert the instance and corresponding type into a map for lookup by type.
         * */
        for (TypeIndex type_index : type_list) {

            status = this->instance_factory_list->create_instance(type_index, &new_object);
            ASSERT(PENES_STATUS_SUCCESS == status);

            this->instance_table.push_back(std::make_pair(type_index, new_object));
            this->type_lookup_map.insert({type_index, new_object});
        }
    }

    ~ObjectTable()
    {
        /* Iterate through the object vector table and delete each instance. */
        for (std::pair<TypeIndex, BaseClass *> object_pair : this->instance_table) {
            delete object_pair.second;
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

        /* Check if the parameter index is within the bounds of the table. */
        if (this->instance_table.size() <= table_index) {
            status = PENES_STATUS_UTILS_OBJECT_TABLE_GET_TYPE_OUT_OF_BOUNDS;
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Table index out of bounds. Status: %d", status);
            goto l_cleanup;
        }

        /* Retrieve the object type from the vector table. */
        *output_type = this->instance_table.at(table_index).first;

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

        /* Check if the parameter index is within the bounds of the table. */
        if (this->instance_table.size() <= table_index) {
            status = PENES_STATUS_UTILS_OBJECT_TABLE_GET_OBJECT_OUT_OF_BOUNDS;
            DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Table index out of bounds. Status: %d", status);
            goto l_cleanup;
        }

        /* Retrieve the object instance from the vector table. */
        *output_object = this->instance_table.at(table_index).second;

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    enum PeNESStatus get_object_by_type(
        TypeIndex type_index,
        BaseClass **output_object
    )
    {
        enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
        typename std::unordered_map<TypeIndex, BaseClass *>::const_iterator instance_iter;
        BaseClass *found_object = nullptr;

        ASSERT(nullptr != output_object);

        /* Check if the type exists in the map.
         * If it doesn't, create a new instance of said type and insert it into the map.
         * */
        instance_iter = this->type_lookup_map.find(type_index);
        if (this->type_lookup_map.end() == instance_iter) {
            status = this->instance_factory_list->create_instance(type_index, &found_object);
            if (PENES_STATUS_SUCCESS != status) {
                DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("create_instance failed. Status: %d\n", status);
                goto l_cleanup;
            }

            this->type_lookup_map.insert({type_index, found_object});
        } else {
            /* Retrieve the object instance from the map. */
            found_object = instance_iter->second;
        }

        *output_object = found_object;

        status = PENES_STATUS_SUCCESS;
    l_cleanup:
        return status;
    }

    inline std::size_t get_size() const
    {
        return this->instance_table.size();
    };

private:
    const InstanceFactoryList<TypeIndex, BaseClass> *instance_factory_list;
    std::vector<std::pair<TypeIndex, BaseClass *>> instance_table;
    std::unordered_map<TypeIndex, BaseClass *> type_lookup_map;
};

}

#endif /* __UTILS_H__ */
