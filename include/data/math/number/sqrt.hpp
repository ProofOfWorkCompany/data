// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_SQRT
#define DATA_MATH_NUMBER_SQRT

#include <data/math/abs.hpp>

namespace data::math {

    template <typename G, typename Z = G> struct sqrt;
}

namespace data::interface {
    
    template <typename X>
    concept has_sqrt_method = requires(const X x) {
        { x.sqrt() };
    };
    
    template <typename G, typename Z = G>
    concept has_sqrt = requires(const Z x) {
        { math::sqrt<G, Z>{}(x) } -> std::same_as<G>;
    };
    
}

#endif
