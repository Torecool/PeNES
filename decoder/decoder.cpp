/**
 * @brief  
 * @author TBK
 * @date   17/04/2020
 * */


/** Headers ***************************************************************/
#include "decoder/decoder.h"

/** Constants *************************************************************/
#define DECODER_INSTRUCTION_GROUP_ENCODING_OFFSET (0)
#define DECODER_INSTRUCTION_GROUP_ENCODING_BIT_MASK (0b11 << DECODER_INSTRUCTION_GROUP_ENCODING_OFFSET)

#define DECODER_ADDRESS_MODE_ENCODING_OFFSET (2)
#define DECODER_ADDRESS_MODE_ENCODING_BIT_MASK (0b111 << DECODER_ADDRESS_MODE_ENCODING_OFFSET)

#define DECODER_OPCODE_ENCODING_OFFSET (5)
#define DECODER_OPCODE_ENCODING_BIT_MASK (0b111 << DECODER_OPCODE_ENCODING_OFFSET)

/** Macros ****************************************************************/
#define DECODER_GET_INSTRUCTION_GROUP_ENCODING(instruction_data) (                                                    \
    ((instruction_data) & DECODER_INSTRUCTION_GROUP_ENCODING_BIT_MASK) >> DECODER_INSTRUCTION_GROUP_ENCODING_OFFSET   \
)

#define DECODER_GET_ADDRESS_MODE_ENCODING(instruction_data) (                                                         \
    ((instruction_data) & DECODER_ADDRESS_MODE_ENCODING_BIT_MASK) >> DECODER_ADDRESS_MODE_ENCODING_OFFSET             \
)

#define DECODER_GET_OPCODE_ENCODING(instruction_data) (                                                               \
    ((instruction_data) & DECODER_OPCODE_ENCODING_BIT_MASK) >> DECODER_OPCODE_ENCODING_OFFSET                         \
)

/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Static Variables ******************************************************/
const std::initializer_list<enum instruction_set::OpcodeType> Decoder::default_opcode_table_group_1 = {
    instruction_set::OPCODE_TYPE_ORA,
    instruction_set::OPCODE_TYPE_AND,
    instruction_set::OPCODE_TYPE_EOR,
    instruction_set::OPCODE_TYPE_ADC,
    instruction_set::OPCODE_TYPE_STA,
    instruction_set::OPCODE_TYPE_LDA,
    instruction_set::OPCODE_TYPE_CMP,
    instruction_set::OPCODE_TYPE_SBC
};


const std::initializer_list<enum instruction_set::OpcodeType> Decoder::default_opcode_table_group_2 = {
    instruction_set::OPCODE_TYPE_ASL,
    instruction_set::OPCODE_TYPE_ROL,
    instruction_set::OPCODE_TYPE_LSR,
    instruction_set::OPCODE_TYPE_ROR,
    instruction_set::OPCODE_TYPE_STX,
    instruction_set::OPCODE_TYPE_LDX,
    instruction_set::OPCODE_TYPE_DEC,
    instruction_set::OPCODE_TYPE_INC
};


const InstructionDecodeGroup Decoder::instruction_group_0 = {
    {
        address_mode::ADDRESS_MODE_TYPE_IMMEDIATE,
        address_mode::ADDRESS_MODE_TYPE_ZEROPAGE,
        address_mode::ADDRESS_MODE_TYPE_IMPLIED,
        address_mode::ADDRESS_MODE_TYPE_ABSOLUTE,
        address_mode::ADDRESS_MODE_TYPE_RELATIVE,
        address_mode::ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED,
        address_mode::ADDRESS_MODE_TYPE_IMPLIED,
        address_mode::ADDRESS_MODE_TYPE_ABSOLUTE_X_INDEXED
    },
    {
        {
            instruction_set::OPCODE_TYPE_BRK,
            instruction_set::OPCODE_TYPE_JSR,
            instruction_set::OPCODE_TYPE_RTI,
            instruction_set::OPCODE_TYPE_RTS,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_LDY,
            instruction_set::OPCODE_TYPE_CPY,
            instruction_set::OPCODE_TYPE_CPX
        },
        {
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_BIT,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_STY,
            instruction_set::OPCODE_TYPE_LDY,
            instruction_set::OPCODE_TYPE_CPY,
            instruction_set::OPCODE_TYPE_CPX
        },
        {
            instruction_set::OPCODE_TYPE_PHP,
            instruction_set::OPCODE_TYPE_PLP,
            instruction_set::OPCODE_TYPE_PHA,
            instruction_set::OPCODE_TYPE_PLA,
            instruction_set::OPCODE_TYPE_DEY,
            instruction_set::OPCODE_TYPE_TAY,
            instruction_set::OPCODE_TYPE_INY,
            instruction_set::OPCODE_TYPE_INX
        },
        {
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_BIT,
            instruction_set::OPCODE_TYPE_JMP,
            instruction_set::OPCODE_TYPE_INDIRECT_JMP,
            instruction_set::OPCODE_TYPE_STY,
            instruction_set::OPCODE_TYPE_LDY,
            instruction_set::OPCODE_TYPE_CPY,
            instruction_set::OPCODE_TYPE_CPX
        },
        {
            instruction_set::OPCODE_TYPE_BPL,
            instruction_set::OPCODE_TYPE_BMI,
            instruction_set::OPCODE_TYPE_BVC,
            instruction_set::OPCODE_TYPE_BVS,
            instruction_set::OPCODE_TYPE_BCC,
            instruction_set::OPCODE_TYPE_BCS,
            instruction_set::OPCODE_TYPE_BNE,
            instruction_set::OPCODE_TYPE_BEQ
        },
        {
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_STY,
            instruction_set::OPCODE_TYPE_LDY,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE
        },
        {
            instruction_set::OPCODE_TYPE_CLC,
            instruction_set::OPCODE_TYPE_SEC,
            instruction_set::OPCODE_TYPE_CLI,
            instruction_set::OPCODE_TYPE_SEI,
            instruction_set::OPCODE_TYPE_TYA,
            instruction_set::OPCODE_TYPE_CLV,
            instruction_set::OPCODE_TYPE_CLD,
            instruction_set::OPCODE_TYPE_SED
        },
        {
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_LDY,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE
        }
    }
};


const InstructionDecodeGroup Decoder::instruction_group_1 = {
    {
        address_mode::ADDRESS_MODE_TYPE_X_INDEXED_INDIRECT,
        address_mode::ADDRESS_MODE_TYPE_ZEROPAGE,
        address_mode::ADDRESS_MODE_TYPE_IMMEDIATE,
        address_mode::ADDRESS_MODE_TYPE_ABSOLUTE,
        address_mode::ADDRESS_MODE_TYPE_INDIRECT_Y_INDEXED,
        address_mode::ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED,
        address_mode::ADDRESS_MODE_TYPE_ABSOLUTE_Y_INDEXED,
        address_mode::ADDRESS_MODE_TYPE_ABSOLUTE_X_INDEXED
    },
    {
        Decoder::default_opcode_table_group_1,
        Decoder::default_opcode_table_group_1,
        {
            instruction_set::OPCODE_TYPE_ORA,
            instruction_set::OPCODE_TYPE_AND,
            instruction_set::OPCODE_TYPE_EOR,
            instruction_set::OPCODE_TYPE_ADC,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_LDA,
            instruction_set::OPCODE_TYPE_CMP,
            instruction_set::OPCODE_TYPE_SBC
        },
        Decoder::default_opcode_table_group_1,
        Decoder::default_opcode_table_group_1,
        Decoder::default_opcode_table_group_1,
        Decoder::default_opcode_table_group_1,
        Decoder::default_opcode_table_group_1
    }
};


const InstructionDecodeGroup Decoder::instruction_group_2 = {
    {
        address_mode::ADDRESS_MODE_TYPE_IMMEDIATE,
        address_mode::ADDRESS_MODE_TYPE_ZEROPAGE,
        address_mode::ADDRESS_MODE_TYPE_ACCUMULATOR,
        address_mode::ADDRESS_MODE_TYPE_ABSOLUTE,
        address_mode::ADDRESS_MODE_TYPE_NONE,
        address_mode::ADDRESS_MODE_TYPE_ZEROPAGE_X_INDEXED,
        address_mode::ADDRESS_MODE_TYPE_IMPLIED,
        address_mode::ADDRESS_MODE_TYPE_ABSOLUTE_X_INDEXED
    },
    {
        {
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_LDX,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE
        },
        Decoder::default_opcode_table_group_2,
        {
            instruction_set::OPCODE_TYPE_ASL,
            instruction_set::OPCODE_TYPE_ROL,
            instruction_set::OPCODE_TYPE_LSR,
            instruction_set::OPCODE_TYPE_ROR,
            instruction_set::OPCODE_TYPE_TXA,
            instruction_set::OPCODE_TYPE_TAX,
            instruction_set::OPCODE_TYPE_DEX,
            instruction_set::OPCODE_TYPE_NOP
        },
        Decoder::default_opcode_table_group_2,
        {
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE
        },
        Decoder::default_opcode_table_group_2,
        {
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_TXS,
            instruction_set::OPCODE_TYPE_TSX,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_NONE
        },
        {
            instruction_set::OPCODE_TYPE_ASL,
            instruction_set::OPCODE_TYPE_ROL,
            instruction_set::OPCODE_TYPE_LSR,
            instruction_set::OPCODE_TYPE_ROR,
            instruction_set::OPCODE_TYPE_NONE,
            instruction_set::OPCODE_TYPE_LDX,
            instruction_set::OPCODE_TYPE_DEC,
            instruction_set::OPCODE_TYPE_INC
        }
    }
};

const std::array<const InstructionDecodeGroup *, DECODER_NUM_INSTRUCTION_DECODE_GROUPS> Decoder::instruction_group_table = {
    &Decoder::instruction_group_0,
    &Decoder::instruction_group_1,
    &Decoder::instruction_group_2
};

/** Functions *************************************************************/
InstructionDecodeGroup::InstructionDecodeGroup(
    std::initializer_list<enum address_mode::AddressModeType> address_mode_types,
    std::initializer_list<std::initializer_list<enum instruction_set::OpcodeType>> opcode_type_groups
): address_mode_table(address_mode_types)
{
    ASSERT(address_mode_types.size() == opcode_type_groups.size());

    /* Create and initialize an opcode table for each group. */
    for (std::initializer_list<enum instruction_set::OpcodeType> type_group : opcode_type_groups) {
        this->opcode_tables.push_back(new instruction_set::OpcodeTable(type_group));
    }
}


enum PeNESStatus InstructionDecodeGroup::decode_instruction(
    native_word_t instruction_data,
    instruction_set::IOpcode **output_opcode,
    IAddressMode **output_address_mode
) const
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    std::size_t address_mode_encoding = DECODER_GET_ADDRESS_MODE_ENCODING(instruction_data);
    std::size_t opcode_encoding = DECODER_GET_OPCODE_ENCODING(instruction_data);
    instruction_set::OpcodeTable *opcode_table = nullptr;
    instruction_set::IOpcode *opcode = nullptr;
    address_mode::IAddressMode *address_mode = nullptr;
    enum address_mode::AddressModeType address_mode_type = ADDRESS_MODE_TYPE_NONE;
    enum address_mode::AddressModeType resolved_address_mode_type = ADDRESS_MODE_TYPE_NONE;

    ASSERT(nullptr != output_opcode);
    ASSERT(nullptr != output_address_mode);

    /* Retrieve the type of the default address mode referred to by the encoded index. */
    status = this->address_mode_table.get_type(address_mode_encoding, &address_mode_type);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Address mode table get_type failed. Status: %d. Address mode encoding: %zu\n",
            status,
            address_mode_encoding
        );
        goto l_cleanup;
    }

    /* Retrieve the opcode table to search in for the opcode, based on the encoded address mode index. */
    opcode_table = this->opcode_tables.at(address_mode_encoding);

    /* Retrieve the opcode class referred to by the encoded index from the matched opcode table. */
    status = opcode_table->get_object(opcode_encoding, &opcode);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Opcode table get_object failed. Status: %d. Opcode encoding: %zu\n",
            status,
            opcode_encoding
        );
        goto l_cleanup;
    }

    /* Using the matched opcode object, call the address mode resolve function to override the default type. */
    resolved_address_mode_type = opcode->resolve_address_mode(address_mode_type);

    /* Retrieve the address mode class from the address mode table, using the overridden address mode type. */
    status = this->address_mode_table.get_object_by_type(
        resolved_address_mode_type,
        &address_mode
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Address mode table get_object_by_type failed. Status: %d. Resolved address mode: %d\n",
            status,
            resolved_address_mode_type
        );
        goto l_cleanup;
    }

    *output_opcode = opcode;
    *output_address_mode = address_mode;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


Decoder::Decoder(
    ProgramContext *program_ctx,
    const native_word_t *src_binary,
    std::size_t src_binary_size
): program_ctx(program_ctx), src_binary(src_binary), src_binary_size(src_binary_size)
{
    ASSERT(nullptr != program_ctx);
    ASSERT(nullptr != src_binary);
    ASSERT(0 < src_binary_size);
}


enum PeNESStatus Decoder::next_instruction(
    instruction_set::Instruction **output_instruction
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    instruction_set::Instruction *instruction = nullptr;
    RegisterStorage<native_address_t> *register_program_counter = nullptr;
    instruction_set::IOpcode *instruction_opcode = nullptr;
    address_mode::IAddressMode *instruction_address_mode = nullptr;
    IStorageLocation *operand_storage = nullptr;
    native_address_t program_counter_address = 0;
    std::size_t operand_storage_offset = 0;

    ASSERT(nullptr != output_instruction);

    /* Retrieve Program counter register from the program context. */
    register_program_counter = program_ctx->register_file.get_register_program_counter();

    /* Read the current program counter address and verify that it is within the bounds of the source binary. */
    program_counter_address = register_program_counter->read();

    /* Read and decode the instruction opcode at the current program counter address. */
    status = this->decode_opcode(
        &program_counter_address,
        &instruction_opcode,
        &instruction_address_mode
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("decode_opcode failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Read and decode the instruction operand following the opcode. */
    status = this->decode_operand(
        &program_counter_address,
        instruction_address_mode,
        &operand_storage,
        &operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS("decode_opcode failed. Status: %d.\n", status);
        goto l_cleanup;
    }

    /* Create an instruction object. */
    instruction = new instruction_set::Instruction(
        program_ctx,
        instruction_opcode,
        operand_storage,
        operand_storage_offset
    );

    /* Write the updated program counter back to the Program counter register. */
    register_program_counter->write(program_counter_address);

    *output_instruction = instruction;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus Decoder::decode_opcode(
    native_address_t *decode_address,
    instruction_set::IOpcode **output_opcode,
    address_mode::IAddressMode **output_address_mode
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    const InstructionDecodeGroup *instruction_group = nullptr;
    instruction_set::IOpcode *instruction_opcode = nullptr;
    address_mode::IAddressMode *instruction_address_mode = nullptr;
    native_word_t instruction_opcode_data = 0;
    std::size_t instruction_group_index = 0;

    ASSERT(nullptr != decode_address);
    ASSERT(nullptr != output_opcode);
    ASSERT(nullptr != output_address_mode);

    /* Verify that the opcode read address is within the bounds of the source binary. */
    if (this->src_binary_size < *decode_address + sizeof(instruction_opcode_data)) {
        status = PENES_STATUS_DECODER_DECODE_OPCODE_ADDRESS_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Decode opcode address out of bounds. Status: %d. Decode address: %x\n",
            status,
            *decode_address
        );
        goto l_cleanup;
    }

    /* Read the instruction opcode at the decode address. */
    instruction_opcode_data = this->src_binary[*decode_address];

    /* Extract the encoded instruction group index and verify it's within range of the table. */
    instruction_group_index = DECODER_GET_INSTRUCTION_GROUP_ENCODING(instruction_opcode_data);

    if (Decoder::instruction_group_table.size() <= instruction_group_index) {
        status = PENES_STATUS_DECODER_DECODE_OPCODE_GROUP_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Encoded instruction group out of bounds. Status: %d. Instruction group: %zu\n",
            status,
            instruction_group_index
        );
        goto l_cleanup;
    }

    /* Decode the rest of the opcode according to the parsed group. */
    instruction_group = Decoder::instruction_group_table.at(instruction_group_index);

    status = instruction_group->decode_instruction(
        instruction_opcode_data,
        &instruction_opcode,
        &instruction_address_mode
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
        "decode_instruction failed. Status: %d. Instruction data: %x\n",
        status,
        instruction_opcode_data
        );
        goto l_cleanup;
    }

    *output_opcode = instruction_opcode;
    *output_address_mode = instruction_address_mode;

    /* Advance the decode address to reflect the new program counter value. */
    *decode_address += sizeof(instruction_opcode_data);

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}


enum PeNESStatus Decoder::decode_operand(
    native_address_t *decode_address,
    address_mode::IAddressMode *address_mode,
    IStorageLocation **output_storage_location,
    std::size_t *output_storage_offset
)
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;
    enum address_mode::InstructionOperandSize operand_size = INSTRUCTION_OPERAND_SIZE_NONE;
    IStorageLocation *operand_storage = nullptr;
    native_word_t instruction_operand_data = 0;
    std::size_t operand_storage_offset = 0;

    ASSERT(nullptr != decode_address);
    ASSERT(nullptr != address_mode);
    ASSERT(nullptr != output_storage_location);
    ASSERT(nullptr != output_storage_offset);

    operand_size = address_mode->operand_size;

    /* Verify that operand decode address is within the bounds of the source binary. */
    if (this->src_binary_size < *decode_address + operand_size) {
        status = PENES_STATUS_DECODER_DECODE_OPERAND_ADDRESS_OUT_OF_BOUNDS;
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "Decode operand address out of bounds. Status: %d. Decode address: %x\n",
            status,
            *decode_address
        );
        goto l_cleanup;
    }

    /* Read the instruction operand at the decode address. */
    COPY_MEMORY(&instruction_operand_data, this->src_binary, operand_size);

    /* Resolve the operand data into the instruction's storage location, using the address mode. */
    status = address_mode->get_storage(
        program_ctx,
        instruction_operand_data,
        &operand_storage,
        &operand_storage_offset
    );
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ERRNO_WITH_ARGS(
            "get_storage failed. Status: %d. Instruction operand data: %x\n",
            status,
            instruction_operand_data
        );
        goto l_cleanup;
    }

    *output_storage_location = operand_storage;
    *output_storage_offset = operand_storage_offset;

    /* Advance the decode address to reflect the new program counter value. */
    *decode_address += operand_size;

    status = PENES_STATUS_SUCCESS;
l_cleanup:
    return status;
}

