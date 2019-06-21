#ifndef DATA_TYPES
#define DATA_TYPES

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <stdint.h>

namespace data {
    
    template<typename X>
    using vector = const std::vector<X>;
    
    template<typename X, uint32_t n>
    using array = const std::array<X, n>;

    using byte = uint8_t;
    
    using bytes = vector<byte>;

    template<typename X>
    using ptr = std::shared_ptr<X>;

    using string = const std::string;
    
    using uint = unsigned long long int;

}

#endif
