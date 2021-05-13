// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_SQRT
#define DATA_MATH_NUMBER_GMP_SQRT

#include <data/math/number/gmp/N.hpp>
#include <data/math/number/sqrt.hpp>
#include <data/math/complex.hpp>

namespace data::math::number::gmp {
        
    N sqrt(const N& n);
    N root(const N& n, uint32 p);
    
    N inline N::sqrt() const {
        return gmp::sqrt(*this);
    }
        
    N inline sqrt(const N& n) {
        return root(n, 2);
    }

}

namespace data::math {
    
    template <> struct sqrt<number::gmp::N, number::gmp::N> {
        number::gmp::N operator()(const number::gmp::N &n) const {
            return n.sqrt();
        }
    };
    
    template <> struct sqrt<number::gmp::N, number::gmp::Z> {
        number::gmp::N operator()(const number::gmp::Z &n) const {
            if (n < 0) return number::gmp::N{};
            return sqrt<number::gmp::N, number::gmp::N>{}(number::gmp::N{n});
        }
    };
    
    template <> struct sqrt<number::gmp::Z, number::gmp::Z> {
        number::gmp::Z operator()(const number::gmp::Z &z) const {
            number::gmp::N n = sqrt<number::gmp::N, number::gmp::Z>{}(z);
            if (n.valid()) return number::gmp::N{};
            return number::gmp::Z(n);
        }
    };

}

#endif
