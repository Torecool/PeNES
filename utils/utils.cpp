/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

/** Headers ***************************************************************/
#include "penes_status.h"
#include "common.h"

#include "utils/utils.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Namespaces ************************************************************/
using namespace utils;

/** Functions *************************************************************/
template<class BaseClass, typename TypeIndex>
template<size_t instance_factory_list_size>
ObjectTable<BaseClass, TypeIndex>::ObjectTable(
    const std::array<BaseClass *(*)(), instance_factory_list_size> &object_instance_factory_list,
    const std::initializer_list<TypeIndex> &type_list
)
{
    BaseClass *new_object = nullptr;
    BaseClass *(*instance_factory)() = nullptr;

    ASSERT(nullptr != object_instance_factory_list);
    ASSERT(nullptr != type_list);

    /* Iterate through the argument type list.
     * For each type, find the corresponding instance factory method and create an instance.
     * Insert the new instance into a vector.
     * */
    for (TypeIndex type_index : type_list) {

        ASSERT(instance_factory_list_size > type_index);

        instance_factory = object_instance_factory_list.at(type_index);
        new_object = instance_factory();

        this->object_instance_table.push_back(new_object);
    }
}


template<class BaseClass, typename TypeIndex>
ObjectTable<BaseClass, TypeIndex>::~ObjectTable()
{
    /* Iterate through the object vector and delete each instance. */
    for (BaseClass *object_instance : this->object_instance_table) {
        delete object_instance;
    }

    this->object_instance_table.clear();
}


template<class BaseClass, typename TypeIndex>
enum PeNESStatus ObjectTable<BaseClass, TypeIndex>::get_object(
    std::size_t table_index,
    BaseClass **output_object
) const
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    ASSERT(nullptr != output_object);

    /* Check if the parameter index is within the bounds of the object vector. */
    if (this->object_instance_table.size() <= table_index) {
        status = PENES_STATUS_UTILS_OBJECT_TABLE_GET_OBJECT_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("Table index out of bounds. Status: %d", status);
        goto l_cleanup;
    }

    /* Retrieve the object instance from the vector. */
    *output_object = this->object_instance_table.at(table_index);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}
