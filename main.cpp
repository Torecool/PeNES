#include "rom_loader/rom_loader.h"
#include "cpu/cpu.h"
#include "program_context/program_context.h"
#include "utils/utils.h"
#include "storage_location/storage_location.h"

#define ROM_INPUT_FILE ("./test/Super Mario Bros. (World).nes")

using namespace utils;
int main()
{
    enum PeNESStatus status = PENES_STATUS_UNINITIALIZED;

    /* Initialize ROM loader to load the input ROM file. */
    ROMLoader rom_loader;
    status = rom_loader.open(ROM_INPUT_FILE);
    if (PENES_STATUS_SUCCESS != status) {
        DEBUG_PRINT_WITH_ARGS("Open failed. Status: %d.\n", status);
        return -1;
    }

    /* Initialize a program context object. */
    ProgramContext program_ctx(&rom_loader);

    /* Initialize and run the emulator CPU. */
    CPU emulator(&program_ctx);
    emulator.run();
}
