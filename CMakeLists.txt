cmake_minimum_required(VERSION 3.15)
project(PeNES)

set(CMAKE_CXX_STANDARD 14)

add_executable(PeNES main.cpp decoder/decoder.cpp decoder/decoder.h address_mode/address_mode.cpp address_mode/address_mode.h memory_map/memory_map.cpp memory_map/memory_map.h penes_status.h common.h address_mode/absolute_address_mode.cpp address_mode/absolute_address_mode.h address_mode/indirect_address_mode.cpp address_mode/indirect_address_mode.h address_mode/zeropage_address_mode.cpp address_mode/zeropage_address_mode.h program_context/program_context.h address_mode/address_mode_interface.h storage_location/storage_location.cpp storage_location/storage_location.h system.h address_mode/accumulator_address_mode.cpp address_mode/accumulator_address_mode.h address_mode/immediate_address_mode.cpp address_mode/immediate_address_mode.h utils/utils.cpp utils/utils.h instruction_set/opcode_interface.h instruction_set/instruction_set.cpp instruction_set/instruction_set.h instruction_set/alu_opcodes.cpp instruction_set/alu_opcodes.h instruction_set/branch_opcodes.cpp instruction_set/branch_opcodes.h instruction_set/flag_opcodes.h instruction_set/store_opcodes.cpp instruction_set/store_opcodes.h instruction_set/transfer_opcodes.cpp instruction_set/transfer_opcodes.h instruction_set/inc_dec_opcodes.cpp instruction_set/inc_dec_opcodes.h instruction_set/load_opcodes.cpp instruction_set/load_opcodes.h instruction_set/compare_opcodes.cpp instruction_set/compare_opcodes.h instruction_set/boolean_opcodes.cpp instruction_set/boolean_opcodes.h instruction_set/shift_opcodes.cpp instruction_set/shift_opcodes.h instruction_set/stack_opcodes.cpp instruction_set/stack_opcodes.h instruction_set/jump_opcodes.cpp instruction_set/jump_opcodes.h cpu/cpu.cpp cpu/cpu.h instruction_set/flag_opcodes.cpp instruction_set/operation_types.cpp instruction_set/operation_types.h)

include_directories(.)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL Debug)