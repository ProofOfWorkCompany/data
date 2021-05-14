// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_DIGEST
#define DATA_CRYPTO_DIGEST

#include "data/types.hpp"
#include <data/math/number/bounded/bounded.hpp>

namespace data::crypto {
    
    template <size_t s>
    struct digest : uint<s> {
        static const uint32 size = s;
        
        using uint<s>::uint;
        
        digest() : uint<s>() {}
        
        bool valid() const;
    };
    
    template<size_t s>
    inline bool digest<s>::valid() const {
        return *this != digest{0};
    }
    
}

#endif

