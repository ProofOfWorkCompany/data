// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <data/math/number/natural.hpp>

namespace data::math::number::integer {
    
    template <typename Z>
    static division<Z> divide(const Z Dividend, const Z Divisor) {
        
        if (Dividend < 0) {
            if (Divisor < 0) {
                division<Z> d{natural::divide<Z>(-Dividend, -Divisor)};
                return {d.Quotient + 1, Divisor - d.Remainder};
            }
            
            division<Z> d{natural::divide<Z>(-Dividend, Divisor)};
            return {-(d.Quotient + 1), Divisor - d.Remainder};
        }
        
        if (Divisor < 0) {
            division<Z> d{natural::divide<Z>(Dividend, -Divisor)};
            return {-d.Quotient, d.Remainder};
        }
        
        return natural::divide<Z>(Dividend, Divisor);
    }

}

#endif
