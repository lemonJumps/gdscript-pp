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
#include <functional>

#include <vector>

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


class VirtualParam
{
    auto callWith()
    {

    }
};

/**
 * @brief black magic, will keep parameters and will call function with new parameters, if the types match
 * 
 * @tparam R return type
 * @tparam Args variable arguments
 */
template<typename... Args>
class Param : VirtualParam
{
    std::tuple<Args...> params;

public:

    explicit constexpr Param(Args&&... argv): 
    mFuncPtr{ func }, 
    mParams{ std::make_tuple(std::forward<Args>(argv)...) } 
    {}

    template<typename R>
    auto callWith(R(*func)(Args...)) -> std::conditional_t<std::is_void_v<R>, void*, R>
    {
        if constexpr (std::is_void_v<R>) 
        {  
            // invoke the function pointer with tuple pack. 
            std::apply(func, params);
            return nullptr;     // Use nullptr for void return type
        }
        else 
        {
            return std::apply(func, params);;
        }
    }
};

class VM
{
public:
    std::vector<void *> FunctionBuffer;


    std::vector<VirtualParam> ParameterBuffer;


    // std::vector<> StorageBuffer;


    std::vector<uint32_t> Stack;

private:
    
};


}