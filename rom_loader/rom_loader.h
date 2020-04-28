/**
 * @brief  
 * @author TBK
 * @date   27/04/2020
 * */

#ifndef __ROM_LOADER_H__
#define __ROM_LOADER_H__

/** Headers ***************************************************************/
#include <string>
#include <fstream>
#include <iostream>

#include "penes_status.h"

/** Constants *************************************************************/


/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/
class ROMLoader {
public:
    enum PeNESStatus open(const std::string& input_file);

    enum PeNESStatus get_prg_rom_bank(std::size_t bank_index, char *bank_buffer);

    inline std::size_t get_num_prg_rom_banks() const
    {
        return this->num_prg_rom_banks;
    }

private:
    std::size_t num_prg_rom_banks = 0;
    std::size_t num_chr_rom_banks = 0;
    std::ifstream input_file_stream;
};


#endif /* __ROM_LOADER_H__ */
