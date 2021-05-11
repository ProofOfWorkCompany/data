// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_ONE_WAY
#define DATA_CRYPTO_ONE_WAY

#include <data/function.hpp>

namespace data::crypto {
    
    // one_way asserts that a non-invertable
    // function exists which takes s to p. 
    template <typename f, typename pub, typename priv> requires function<f, pub, priv> struct one_way_function;
    
    template <typename f, typename pub, typename priv>
    concept one_way = function<f, pub, priv> && requires {
        typename one_way_function<f, pub, priv>; 
    };
    
}

#endif
