// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_GROUP
#define DATA_MATH_GROUP

#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename elem, typename op>
    concept group = monoid<op, elem> && invertible<elem, op, elem>;
    
    template <typename elem, typename op>
    concept abelian = group<elem, op> && requires { typename commutative<op, elem>; };
    
}

#endif 
