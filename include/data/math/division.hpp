// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_DIVISION
#define DATA_MATH_NUMBER_DIVISION

#include <data/math/nonnegative.hpp>

namespace data::math {
    
    struct division_by_zero : std::invalid_argument {
        division_by_zero() : std::invalid_argument{"division_by_zero"} {}
    };
    
    template <typename N, typename R = N> 
    //requires std::default_initializable<N> && std::copyable<N> && std::default_initializable<R> && std::copyable<R> 
    struct division {
        N Quotient;
        R Remainder;
            
        bool valid() const {
            return valid(Quotient) && valid(Remainder);
        }
        
        division(const N& q, const R& r) : Quotient{q}, Remainder{r} {}
        division() : Quotient{}, Remainder{} {}
    };

    template <typename dividend, typename divisor = dividend> struct divide;

    template <typename A, typename B = A> requires interface::has_mod_operator<A, B>
    struct mod {
        A operator()(const A& a, const A& b) {
            return a % b;
        }
    };
    
    template <typename N, typename R>
    bool operator==(const division<N, R>& a, const division<N, R>& b) {
        return a.Quotient == a.Quotient && a.Remainder == b.Remainder;
    }
    
    template <typename N>
    inline std::ostream& operator<<(std::ostream& o, const data::math::division<N> x) {
        return o << "division{Quotient: " << x.Quotient << ", Remainder: " << x.Remainder << "}";
    }
    
}

namespace data {
    template <typename dividend, typename divisor = dividend> 
    inline auto divide(const dividend& a, const math::nonzero<divisor>& b) -> decltype(math::divide<dividend, divisor>{}(a, b)) {
        return math::divide<dividend, divisor>{}(a, b);
    }
}

#endif
