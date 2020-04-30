/**
 * @brief  
 * @author TBK
 * @date   17/04/2020
 * */

#ifndef __DECODER_H__
#define __DECODER_H__

/** Headers ***************************************************************/
#include "program_context/program_context.h"
#include "address_mode/address_mode.h"
#include "instruction_set/instruction_set.h"

/** Constants *************************************************************/
#define DECODER_NUM_INSTRUCTION_DECODE_GROUPS (3)

/** Classes ***************************************************************/
class InstructionDecodeGroup {
public:
    InstructionDecodeGroup(
        std::initializer_list<address_mode::AddressModeType> address_mode_types,
        std::initializer_list<std::initializer_list<instruction_set::OpcodeType>> opcode_type_groups
    );

    inline ~InstructionDecodeGroup()
    {
        this->opcode_tables.clear();
    };

    inline enum PeNESStatus decode_instruction(
        native_word_t instruction_data,
        instruction_set::IOpcode **output_opcode,
        address_mode::IAddressMode **output_address_mode
    );

private:
    address_mode::AddressModeTable address_mode_table;
    std::vector<instruction_set::OpcodeTable *> opcode_tables;
};


class Decoder {
public:
    inline explicit Decoder(ProgramContext *program_ctx):
        program_ctx(program_ctx),
        instruction_group_table {
            InstructionDecodeGroup(address_mode_table_group_0, opcode_tables_group_0),
            InstructionDecodeGroup(address_mode_table_group_1, opcode_tables_group_1),
            InstructionDecodeGroup(address_mode_table_group_2, opcode_tables_group_2)
        }
    {
        ASSERT(nullptr != program_ctx);
    }

    enum PeNESStatus next_instruction(
        instruction_set::Instruction **output_instruction
    );

private:
    enum PeNESStatus decode_opcode(
        native_address_t *decode_address,
        instruction_set::IOpcode **output_opcode,
        address_mode::IAddressMode **output_address_mode
    );

    enum PeNESStatus decode_operand(
        native_address_t *decode_address,
        address_mode::IAddressMode *address_mode,
        IStorageLocation **output_storage_location,
        std::size_t *output_storage_offset
    );

    enum PeNESStatus read_instruction_data(
        native_address_t read_address,
        native_word_t *read_buffer,
        std::size_t num_read_words
    );

    ProgramContext *program_ctx;
    MemoryStorage *prg_rom_storage = nullptr;
    native_address_t prg_rom_storage_start_address = 0;

    static const std::initializer_list<address_mode::AddressModeType> address_mode_table_group_0;
    static const std::initializer_list<address_mode::AddressModeType> address_mode_table_group_1;
    static const std::initializer_list<address_mode::AddressModeType> address_mode_table_group_2;

    static const std::initializer_list<instruction_set::OpcodeType> default_opcode_table_group_1;
    static const std::initializer_list<instruction_set::OpcodeType> default_opcode_table_group_2;

    static const std::initializer_list<std::initializer_list<instruction_set::OpcodeType>> opcode_tables_group_0;
    static const std::initializer_list<std::initializer_list<instruction_set::OpcodeType>> opcode_tables_group_1;
    static const std::initializer_list<std::initializer_list<instruction_set::OpcodeType>> opcode_tables_group_2;

    std::array<InstructionDecodeGroup, DECODER_NUM_INSTRUCTION_DECODE_GROUPS> instruction_group_table;
};

#endif /* __DECODER_H__ */
