// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_FIELD
#define DATA_MATH_FIELD

#include <data/math/ring.hpp>
#include <data/math/abs.hpp>
#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept division_algebra = ring<elem, plus, times> && inverse_multiplicative<elem, math::nonzero<elem>>;
    
    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept normed_division_algebra = division_algebra<elem, plus, times> && interface::has_quadrance<elem>;
    
    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept field = division_algebra<elem, plus, times> && abelian<math::nonzero<elem>, times>;
    
}

#endif
