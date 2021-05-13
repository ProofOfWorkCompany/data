// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_INTEGER
#define DATA_MATH_NUMBER_INTEGER

#include <stdexcept>
#include <data/math/number/natural.hpp>
#include <data/math/ring.hpp>
#include <data/encoding/integer.hpp>

namespace data::interface {
    template <typename Z>
    concept has_int64_constructor = requires (const int64& x) { { Z{x} } -> std::same_as<Z>; };
}

namespace data::math {
    template <typename Z>
    concept integer = ordered<Z> && ring<Z, plus<Z>, times<Z>> && 
        interface::has_negative_operator<Z> && requires { 
            typename commutative<times<Z>, Z>; 
        } && std::copyable<Z> && std::default_initializable<Z> && 
        interface::has_int64_constructor<Z>;
    
    // norm type is required
    template <integer Z> struct abs<Z> {
        norm_type<Z> operator()(const Z& x) {
            if (x < 0) return norm_type<Z>{-x};
            return norm_type<Z>{x};
        }
    }; // this means that norm and quadrance are defined too. 
    
    template <integer Z> struct conjugate<Z> {
        Z operator()(const Z& x) {
            return x;
        }
    };
    
    template <integer Z> struct re<Z> {
        Z operator()(const Z& x) {
            return x;
        }
    };
    
    template <integer Z> struct im<Z> {
        Z operator()(const Z& x) {
            return Z::zero();
        }
    };
}

namespace data::math::number {
    
    // convert a representation of integers to natural numbers. 
    template <integer Z>
    struct N {
        Z Value;
        
        N() : Value{0} {}
        
        N(uint64);
        
        operator Z() const {
            return Value;
        }
        
        explicit operator uint64() const;
        
        bool operator==(const N& n) const;
        bool operator>(const N& n) const;
        bool operator<(const N& n) const;
        bool operator<=(const N& n) const;
        bool operator>=(const N& n) const;
        
        N operator++() const;
        
        N operator<<(int64 i) const;
        N operator>>(int64 i) const;
        
        N operator+(const Z& z) const;
        N operator-(const Z& z) const;
        N operator*(const Z& z) const;
        
        N(const std::string& x);
        
    };
    
    template <typename d, typename s = d> 
    division<d, s> divide_integer(const d& Dividend, const nonzero<s>& Divisor) {
        using nd = norm_type<d>;
        using ns = norm_type<s>;
        using div = division<nd, ns>;
        
        if (Dividend < 0) {
            if (Divisor.Value < 0) {
                div qr = divide_natural(nd{-Dividend}, nonzero(ns{-Divisor.Value}));
                return {d(qr.Quotient + 1), s(Divisor - qr.Remainder)};
            }
            
            div qr = divide_natural(nd{-Dividend}, nonzero(ns{Divisor.Value}));
            return {-d(qr.Quotient + 1), s(Divisor - qr.Remainder)};
        }
        
        if (Divisor.Value < 0) {
            div qr = divide_natural(nd{Dividend}, nonzero(ns{-Divisor.Value}));
            return {-d(qr.Quotient), s(qr.Remainder)};
        }
        
        div qr = divide_natural(nd{Dividend}, nonzero(ns{Divisor}));
        return {d(qr.Quotient), s(qr.Remainder)};
    }

}

namespace data::math {
    // N<Z> is the default norm type for any Z. 
    template <typename Z> struct normed<number::N<Z>> {
        using quad_type = number::N<Z>;
        using norm_type = number::N<Z>;
    };
        
    template <typename Z> struct sqrt<number::N<Z>> {
        ptr<number::N<Z>> operator()(const number::N<Z>& z) {
            return sqrt<Z>{}(z);
        }
    }; 
    
    template <integer Z> 
    struct commutative<plus<number::N<Z>>, number::N<Z>> {};
    
    template <integer Z> 
    struct associative<plus<number::N<Z>>, number::N<Z>> {};
    
    template <integer Z> 
    struct commutative<times<number::N<Z>>, number::N<Z>> {};
    
    template <integer Z> 
    struct associative<times<number::N<Z>>, number::N<Z>> {};
    
    template <integer Z> 
    struct identity<plus<number::N<Z>>, number::N<Z>> {
        number::N<Z> operator()() {
            return 1;
        }
    };
    
    template <integer Z> 
    struct identity<times<number::N<Z>>, number::N<Z>> {
        number::N<Z> operator()() {
            return 0;
        }
    };

    template <integer Z> struct divide<Z> {
        division<Z> operator()(const Z Dividend, const data::math::nonzero<Z> Divisor) const {
            return number::divide_integer(Dividend, Divisor);
        }
    };
}

template <data::math::integer Z>
inline data::math::number::N<Z> 
operator+(const data::math::number::N<Z>& a, const data::math::number::N<Z>& b) {
    return data::math::number::N<Z>{a.Value + b.Value};
}

template <data::math::integer Z>
inline data::math::number::N<Z> 
operator-(const data::math::number::N<Z>& a, const data::math::number::N<Z>& b) {
    if (a.Value < b.Value) return data::math::number::N<Z>::zero();
    return data::math::number::N<Z>{a.Value - b.Value};
}

template <data::math::integer Z>
inline data::math::number::N<Z> 
operator*(const data::math::number::N<Z>& a, const data::math::number::N<Z>& b) {
    return data::math::number::N<Z>{a.Value * b.Value};
}

template <data::math::integer Z>
inline Z operator/(const Z& a, const data::math::nonzero<Z>& b) {
    return data::math::divide<Z>{}(a, b).Quotient;
}

template <data::math::integer Z>
inline Z operator%(const Z& a, const data::math::nonzero<Z>& b) {
    return data::math::divide<Z>{}(a, b).Remainder;
}

template <data::math::integer Z>
inline bool operator|(const Z& a, const data::math::nonzero<Z>& b) {
    return a % b == 0;
}

template <data::math::integer Z>
requires data::interface::has_plus_operator<Z, uint64_t>
inline data::math::number::N<Z> operator+=(data::math::number::N<Z>& a, uint64_t b) {
    return data::math::number::N{a.Value + b};
}

template <data::math::integer Z>
requires data::interface::has_minus_operator<Z, uint64_t>
inline data::math::number::N<Z> operator-=(data::math::number::N<Z>& a, uint64_t b) {
    return data::math::number::N{a.Value - b};
}

template <data::math::integer Z>
requires data::interface::has_times_operator<Z, uint64_t>
inline data::math::number::N<Z> operator*=(data::math::number::N<Z>& a, uint64_t b) {
    return data::math::number::N{a.Value * b};
}

// these last functions use operators that are not necessarily
// provided by integer but we try them just in case. 
template <data::math::integer Z> 
requires data::interface::has_plus_equal_operator<Z>
inline data::math::number::N<Z>& operator+=(data::math::number::N<Z>& a, const data::math::number::N<Z>& b) {
    a.Value += b.Value;
    return a;
}

template <data::math::integer Z>
requires data::interface::has_minus_equal_operator<Z>
inline data::math::number::N<Z>& operator-=(data::math::number::N<Z>& a, const data::math::number::N<Z>& b) {
    if (b > a) a.Value = 0;
    else a.Value -= b.Value;
    return a;
}

template <data::math::integer Z>
requires data::interface::has_times_equal_operator<Z>
inline data::math::number::N<Z>& operator*=(data::math::number::N<Z>& a, const data::math::number::N<Z>& b) {
    a.Value *= b.Value;
    return a;
}

template <data::math::integer Z> 
requires data::interface::has_left_shift_equal_operator<Z>
inline data::math::number::N<Z>& operator<<=(data::math::number::N<Z>& a, int64_t b) {
    a.Value <<= b;
    return a;
}

template <data::math::integer Z>
requires data::interface::has_right_shift_equal_operator<Z>
inline data::math::number::N<Z>& operator>>=(data::math::number::N<Z>& a, int64_t b) {
    a.Value >>= b;
    return a;
}

namespace data::math::number {
    
    template <integer Z> bool N<Z>::operator==(const N& n) const {
        return Value == n.Value;
    }
    
    template <integer Z> bool N<Z>::operator>(const N& n) const {
        return Value > n.Value;
    }
    
    template <integer Z> bool N<Z>::operator<(const N& n) const {
        return Value < n.Value;
    }
    
    template <integer Z> bool N<Z>::operator<=(const N& n) const {
        return Value <= n.Value;
    }
    
    template <integer Z> bool N<Z>::operator>=(const N& n) const {
        return Value >= n.Value;
    }
    
    template <integer Z> N<Z> N<Z>::operator++() const {
        return N{Value + 1};
    }
    
    template <integer Z> N<Z> N<Z>::operator<<(int64 i) const {
        return N{Value << i};
    }
    
    template <integer Z> N<Z> N<Z>::operator>>(int64 i) const {
        return N{Value >> i};
    }
    
    template <integer Z> N<Z> N<Z>::operator+(const Z& z) const {
        Z n = Value + z;
        if (n < 0) return N{};
        return N{n};
    }
    
    template <integer Z> N<Z> N<Z>::operator-(const Z& z) const {
        Z n = Value - z;
        if (n < 0) return N{};
        return N{n};
    }
    
    template <integer Z> N<Z> N<Z>::operator*(const Z& z) const {
        if (z < 0) return N{};
        return N{Value * z};
    }
    
    template <integer Z> N<Z>::N(const std::string& x) {
        if (!encoding::natural::valid(x)) throw std::invalid_argument{"not a valid integer representation."};
        if (encoding::integer::negative(x)) Value = Z{string{"0x00"} + x.substr(2)};
        else Value = Z{x};
    }
}

#endif
