// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_COUNTABLE
#define DATA_MATH_COUNTABLE

#include <type_traits>

#include <data/types.hpp>

namespace data::interface {
    
    template <typename X>
    concept has_zero_value = requires {
        { X::zero() } -> std::same_as<X>;
    };
    
}

namespace data::math {
    
    template <typename N> struct next {
        N operator()(const N &n) {
            return n + 1;
        }
    };
    
    template <typename L>
    concept countable = (interface::has_zero_value<L> || std::default_initializable<L> ) && requires (const L n) {
        { next<L>{}(n) } -> std::same_as<L>;
    };

}

#endif
