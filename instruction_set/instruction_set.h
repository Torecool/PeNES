/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

#ifndef __INSTRUCTION_SET_H__
#define __INSTRUCTION_SET_H__

/** Headers ***************************************************************/
#include <cstddef>
#include <initializer_list>
#include <decoder/decoder.h>


#include "penes_status.h"

#include "utils/utils.h"

#include "program_context/program_context.h"
#include "storage_location/storage_location.h"
#include "instruction_set/opcode_interface.h"

/** Namespaces ************************************************************/
namespace instruction_set {

/** Enums *****************************************************************/
enum OpcodeType {
    OPCODE_TYPE_NONE = -1,
    OPCODE_TYPE_ADC = 0,
    OPCODE_TYPE_AND,
    OPCODE_TYPE_ASL,
    OPCODE_TYPE_BCC,
    OPCODE_TYPE_BCS,
    OPCODE_TYPE_BEQ,
    OPCODE_TYPE_BIT,
    OPCODE_TYPE_BMI,
    OPCODE_TYPE_BNE,
    OPCODE_TYPE_BPL,
    OPCODE_TYPE_BRK,
    OPCODE_TYPE_BVC,
    OPCODE_TYPE_BVS,
    OPCODE_TYPE_CLC,
    OPCODE_TYPE_CLD,
    OPCODE_TYPE_CLI,
    OPCODE_TYPE_CLV,
    OPCODE_TYPE_CMP,
    OPCODE_TYPE_CPX,
    OPCODE_TYPE_CPY,
    OPCODE_TYPE_DEC,
    OPCODE_TYPE_DEX,
    OPCODE_TYPE_DEY,
    OPCODE_TYPE_EOR,
    OPCODE_TYPE_INC,
    OPCODE_TYPE_INX,
    OPCODE_TYPE_INY,
    OPCODE_TYPE_JMP,
    OPCODE_TYPE_INDIRECT_JMP,
    OPCODE_TYPE_JSR,
    OPCODE_TYPE_LDA,
    OPCODE_TYPE_LDX,
    OPCODE_TYPE_LDY,
    OPCODE_TYPE_LSR,
    OPCODE_TYPE_NOP,
    OPCODE_TYPE_ORA,
    OPCODE_TYPE_PHA,
    OPCODE_TYPE_PHP,
    OPCODE_TYPE_PLA,
    OPCODE_TYPE_PLP,
    OPCODE_TYPE_ROL,
    OPCODE_TYPE_ROR,
    OPCODE_TYPE_RTI,
    OPCODE_TYPE_RTS,
    OPCODE_TYPE_SBC,
    OPCODE_TYPE_SEC,
    OPCODE_TYPE_SED,
    OPCODE_TYPE_SEI,
    OPCODE_TYPE_STA,
    OPCODE_TYPE_STX,
    OPCODE_TYPE_STY,
    OPCODE_TYPE_TAX,
    OPCODE_TYPE_TAY,
    OPCODE_TYPE_TSX,
    OPCODE_TYPE_TXA,
    OPCODE_TYPE_TXS,
    OPCODE_TYPE_TYA,
    OPCODE_TYPE_NUM_OPCODES
};

/** Typedefs **************************************************************/
typedef IOpcode *(*opcode_instance_factory_t)();

/** Classes ***************************************************************/
class OpcodeTable : public utils::ObjectTable<IOpcode, enum OpcodeType> {
public:
    explicit OpcodeTable(std::initializer_list<enum OpcodeType> opcode_types):
        ObjectTable<IOpcode, enum OpcodeType>(
            &opcode_instance_factory_table,
            opcode_types
        )
    {};

private:
    static const std::array<opcode_instance_factory_t, OPCODE_TYPE_NUM_OPCODES> opcode_instance_factory_table;
};


class Instruction {
public:
    explicit Instruction(
        ProgramContext *program_context = nullptr,
        IOpcode *instruction_opcode = nullptr,
        IStorageLocation *operand_storage = nullptr,
        std::size_t operand_storage_offset = 0
    ) :
        program_context(program_context),
        instruction_opcode(instruction_opcode),
        operand_storage(operand_storage),
        operand_storage_offset(operand_storage_offset)
    {};

    inline virtual enum PeNESStatus exec();

    inline virtual enum PeNESStatus reset();

private:
    IOpcode *instruction_opcode;
    ProgramContext *program_context;
    IStorageLocation *operand_storage;
    std::size_t operand_storage_offset;
};

}

#endif /* __INSTRUCTION_SET_H__ */
