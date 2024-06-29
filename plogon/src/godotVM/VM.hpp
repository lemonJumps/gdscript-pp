/**
 * @file VM.hpp
 * @author Lemon Jumps (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-29
 * 
 * @copyright Copyright (c) 2024
 * 
 * a 32 bit fast data processor.
 */

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#include <cstdint>

namespace VM
{

// keep size of pointer for refference
// will be different between 
static const size_t ptr_size = sizeof(void*);

PACK(
struct INST
{
    uint32_t OP;
    uint32_t PARAM1;
    uint32_t PARAM2;
    uint32_t PARAM3;
};
)



class VM
{
public:


private:
    
};


}