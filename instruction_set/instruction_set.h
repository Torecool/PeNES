/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __INSTRUCTION_SET_H__
#define __INSTRUCTION_SET_H__

/** Headers ***************************************************************/
#include "penes_status.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
class Instruction {
public:
    explicit Instruction(
        IOpcode *instruction_opcode = nullptr,
        ProgramContext *program_context = nullptr,
        IStorageLocation* operand_storage = nullptr,
        size_t operand_storage_offset = 0
    ) :
        instruction_opcode(instruction_opcode),
        program_context(program_context),
        operand_storage(operand_storage),
        operand_storage_offset(operand_storage_offset) {};

    inline virtual enum PeNESStatus exec();

    inline virtual enum PeNESStatus reset();

private:
    IOpcode *instruction_opcode;
    ProgramContext* program_context;
    IStorageLocation* operand_storage;
    size_t operand_storage_offset;
};


#endif /* __INSTRUCTION_SET_H__ */
