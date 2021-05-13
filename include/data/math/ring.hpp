// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_RING
#define DATA_MATH_RING

#include <data/math/group.hpp>

namespace data::math {
    
    template <typename elem, typename plus = math::plus<elem>, typename times = math::times<elem>>
    concept ring = abelian<elem, plus> && monoid<times, elem>;
    
}

#endif 
