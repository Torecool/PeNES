/**
 * @brief  
 * @author TBK
 * @date   20/04/2020
 * */

/** Headers ***************************************************************/
#include <array>

#include "utils/utils.h"

#include "instruction_set/instruction_set.h"
#include "instruction_set/opcode_interface.h"

#include "instruction_set/alu_opcodes.h"
#include "instruction_set/boolean_opcodes.h"
#include "instruction_set/branch_opcodes.h"
#include "instruction_set/compare_opcodes.h"
#include "instruction_set/flag_opcodes.h"
#include "instruction_set/inc_dec_opcodes.h"
#include "instruction_set/jump_opcodes.h"
#include "instruction_set/load_opcodes.h"
#include "instruction_set/shift_opcodes.h"
#include "instruction_set/stack_opcodes.h"
#include "instruction_set/store_opcodes.h"
#include "instruction_set/transfer_opcodes.h"

/** Namespaces ************************************************************/
using namespace instruction_set;

/** Static Variables ******************************************************/
const std::array<opcode_instance_factory_t, OPCODE_TYPE_NUM_OPCODES> OpcodeTable::opcode_instance_factory_table = {
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeADC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeAND>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBCC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBCS>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBEQ>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBIT>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBMI>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBNE>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBPL>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBRK>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBVC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeBVS>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeCLC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeCLD>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeCLI>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeCLV>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeCMP>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeCPX>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeCPY>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeDEC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeDEX>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeDEY>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeEOR>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeINC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeINX>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeINY>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeJMP>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeIndirectJMP>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeJSR>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeLDA>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeLDX>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeLDY>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeLSR>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeNOP>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeORA>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodePHA>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodePHP>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodePLA>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodePLP>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeROL>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeROR>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeRTI>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeRTS>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeSBC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeSEC>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeSED>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeSEI>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeSTA>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeSTX>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeSTY>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeTAX>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeTAY>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeTSX>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeTXA>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeTXS>,
    utils::SubClassFactory<IOpcode>::create_instance<OpcodeTYA>
};

/** Functions *************************************************************/
