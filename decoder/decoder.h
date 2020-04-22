/**
 * @brief  
 * @author TBK
 * @date   17/04/2020
 * */

#ifndef __DECODER_H__
#define __DECODER_H__

/** Headers ***************************************************************/
#include <iostream>

#include "program_context/program_context.h"
#include "address_mode/address_mode.h"

/** Constants *************************************************************/
/** Macros ****************************************************************/
/** Enums *****************************************************************/
/** Typedefs **************************************************************/
/** Structs ***************************************************************/
/** Functions *************************************************************/




class Decoder {
public:
    Decoder(std::istream& src_binary);
    inline Instruction* next(void);

private:

};





#endif /* __DECODER_H__ */
