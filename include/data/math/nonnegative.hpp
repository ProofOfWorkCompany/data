// Copyright (c) 2018-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NONNEGATIVE
#define DATA_MATH_NONNEGATIVE

#include <data/valid.hpp>
#include <data/math/ordered.hpp>
#include <data/math/arithmetic.hpp>

namespace data::math {
        
    struct invalid_negative_initialization : std::invalid_argument {
        invalid_negative_initialization() : std::invalid_argument{"invalid negative initialization"} {}
    };
    
    template <typename R> requires ordered<R> && additive<R> && multiplicative<R>
    struct nonnegative {
        R Value;
        
        explicit nonnegative(const R& n) : Value{n} {}
        nonnegative() : Value{-1} {}
        
        bool valid() {
            return data::valid(Value) && Value >= R(0);
        }
        
        operator R() const {
            return Value;
        }
        
        nonnegative& operator++();
        nonnegative operator++(int);
        
        int operator<=>(const nonnegative&);
    };
    
    struct invalid_zero_initialization : std::invalid_argument {
        invalid_zero_initialization() : std::invalid_argument{"invalid negative initialization"} {}
    };
    
    template <typename R> 
    struct nonzero {
        R Value;
        
        explicit nonzero(const R& n) : Value{n} {}
        nonzero() : Value{0} {}
        
        bool valid() const {
            return data::valid(Value) && Value != R(0);
        }
        
        operator R() const {
            return Value;
        }
        
        int operator<=>(const nonzero&);
    };

    template <typename R>
    inline nonnegative<R> operator+(const nonnegative<R>& m, const nonnegative<R>& n) {
        return nonnegative<R>{m.Value + n.Value};
    }

    template <typename R>
    inline nonnegative<R> operator*(const nonnegative<R>& m, const nonnegative<R>& n) {
        return nonnegative<R>{m.Value * n.Value};
    }

    template <typename R>
    inline int nonnegative<R>::operator<=>(const nonnegative<R>& n) {
        return Value <=> n.Value;
    }
    
    template <typename R> nonnegative<R>& nonnegative<R>::operator++() {
        ++Value;
        return *this;
    }
    
    template <typename R> nonnegative<R> nonnegative<R>::operator++(int) {
        nonnegative<R> n = *this;
        ++Value;
        return n;
    }

    template <typename R>
    inline nonzero<R> operator*(const nonzero<R>& m, const nonzero<R>& n) {
        return m.Value * n.Value;
    }

    template <typename R>
    inline int nonzero<R>::operator<=>(const nonzero<R>& n) {
        return Value <=> n.Value;
    }
    /*
    template <typename R> 
    inline int operator<=>(const R &a, const nonzero<R> &b) {
        return a <=> b.Value;
    }*/

}

#endif
