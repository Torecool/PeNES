#include "rom_loader/rom_loader.h"
#include "cpu/cpu.h"
#include "program_context/program_context.h"
#include "utils/utils.h"
#include "storage_location/storage_location.h"

#define ROM_INPUT_FILE ("test/Super Mario Bros. (World).nes")

using namespace utils;
int main()
{
    /* Initialize ROM loader to load the input ROM file. */
    ROMLoader rom_loader;
    rom_loader.open(ROM_INPUT_FILE);

    /* Initialize a program context object. */
    ProgramContext program_ctx(&rom_loader);

    /* Initialize and run the emulator CPU. */
    CPU emulator(&program_ctx);
    emulator.run();
}
