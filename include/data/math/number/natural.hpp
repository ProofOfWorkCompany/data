// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_NATURAL
#define DATA_MATH_NUMBER_NATURAL

#include <data/encoding/endian.hpp>
#include <data/math/countable.hpp>
#include <data/math/cayley_dickson.hpp>

namespace data::interface {
    template <typename N>
    concept has_string_constructor = requires (const string& x) { { N{x} } -> std::same_as<N>; };
    
    template <typename N>
    concept uint64_convertible = requires (const uint64& x) {
        { N(x) } -> std::same_as<N>; 
    } && requires (const N& n) {
        { uint64(n) } -> std::same_as<uint64>;
    };
    
    template <typename N>
    concept has_shift_operators = requires (const N n, int64 i) { 
        { n << i } -> std::same_as<N>; 
        { n >> i } -> std::same_as<N>; 
    };
}

namespace data::math {
    template <typename N>
    concept natural = countable<N> && ordered<N> && 
        monoid<plus<N>, N> && monoid<times<N>, N> && requires {
            typename commutative<plus<N>, N>; 
            typename commutative<times<N>, N>;
        } && std::copyable<N> && std::default_initializable<N> && 
        interface::uint64_convertible<N>;
    
    template <natural N> struct normed<N> {
        using quad_type = N;
    };
    
    template <interface::has_quad_type N> requires natural<N> struct abs<N> {
        N operator()(const N& x) {
            return x;
        }
    }; // this means that norm and quadrance are defined too. 
    
    template <interface::has_quad_type N> requires natural<N> struct conjugate<N> {
        N operator()(const N& x) {
            return x;
        }
    };
    
    template <natural N> struct re<N> {
        N operator()(const N& x) {
            return x;
        }
    };
    
    template <natural N> struct im<N> {
        N operator()(const N& x) {
            return N::zero();
        }
    };
}

namespace data::math::number {
    
    // Generic division algorithm. 
    template <typename N, typename R = N> 
    division<N, R> divide_natural(const N& Dividend, const nonzero<R>& Divisor) {
        if (Divisor == 0) throw division_by_zero{};
        if (Divisor == 1) return {Dividend, 0};
        
        N pow = 1;
        R exp = Divisor;
        N remainder = Dividend;
        N quotient = 0;
        uint64 digits = 1;
        
        while (remainder > exp) { 
            exp<<=digits;
            pow<<=digits;
            digits<<=1;
        } 
        
        while(true) {
            digits >>= 1;
            if (digits == 0) break;
            if (exp > remainder) {
                exp>>=digits;
                pow>>=digits;
            } else { 
                exp<<=digits;
                pow<<=digits;
            }
        }
        
        while (pow > 0) {
            while (remainder <= exp) {
                exp>>=1;
                pow>>=1;
                if (pow == 0) goto out;
            }
            
            quotient += pow;
            remainder -= exp;
        }
        out: 
        
        return {quotient, R(remainder)};
    }
    
}

// division 
namespace data::math {
    template <natural dividend, typename divisor> struct divide<dividend, divisor> {
        division<dividend, divisor> operator()(const dividend Dividend, const data::math::nonzero<divisor> Divisor) const {
            return number::divide_natural(Dividend, Divisor);
        }
    };
}

template <data::math::natural N>
inline N operator/(const N& a, const data::math::nonzero<N>& b) {
    return data::math::divide<N>(a, b).Quotient;
}

template <data::math::natural N>
inline N operator%(const N& a, const data::math::nonzero<N>& b) {
    return data::math::divide<N>(a, b).Remainder;
}

template <data::math::natural N>
inline bool operator|(const N& a, const data::math::nonzero<N>& b) {
    return a % b == 0;
}

// these operations may be provided by the underlying type, but 
// we provide defaults just in case. 
template <data::math::natural N>   
inline N operator+=(const N& a, uint64_t b) {
    return a + N(b);
}

template <data::math::natural N>
inline N operator-=(const N& a, uint64_t b) {
    return a - N(b);
}

template <data::math::natural N>
inline N operator*=(const N& a, uint64_t b) {
    return a * N(b);
}

// in case the operators aren't proveded some other way, 
// we default to these. 
template <data::math::natural N>   
inline N& operator+=(N& a, const N& b) {
    return a = a + b;
}

template <data::math::natural N>
inline N& operator-=(N& a, const N& b) {
    return a = a - b;
}

template <data::math::natural N>
inline N& operator*=(N& a, const N& b) {
    return a = a * b;
}

template <data::math::natural N>   
inline N& operator<<=(N& a, int64_t b) {
    return a = a << b;
}

template <data::math::natural N>   
inline N& operator>>=(N& a, int64_t b) {
    return a = a >> b;
}

// these operations may be provided by the underlying type, but 
// we provide defaults just in case. 
template <data::math::natural N>   
inline N operator+(uint64_t a, const N& b) {
    return b + a;
}

template <data::math::natural N>
inline N operator*(uint64_t a, const N& b) {
    return b * a;
}

template <data::math::natural N>   
bool operator==(const N& a, uint64_t b);

template <data::math::natural N>   
bool operator<(const N& a, uint64_t b);

template <data::math::natural N>   
bool operator>(const N& a, uint64_t b);

template <data::math::natural N>   
bool operator<=(const N& a, uint64_t b);

template <data::math::natural N>   
bool operator>=(const N& a, uint64_t b);

template <data::math::natural N>   
bool operator==(uint64_t a, const N& b);

template <data::math::natural N>   
bool operator<(uint64_t a, const N& b);

template <data::math::natural N>   
bool operator>(uint64_t a, const N& b);

template <data::math::natural N>   
bool operator<=(uint64_t a, const N& b);

template <data::math::natural N>   
bool operator>=(uint64_t a, const N& b);

// a template type in which every representation of a number
// is a different type. Useful for dependent types. 
namespace data {
    
    template <size_t size> struct decimal {
        char Value[size] = {};
        bool Valid{false};
        
        constexpr decimal(const char (&input)[size]) noexcept {
            if (input[0] < '1' || input[0] > '9') return;
            for (size_t i{1}; i < size - 1; ++i) {
                if (input[i] < '1' || input[i] > '9') return;
            }
            if (input[size - 1] != 0) return;
            Valid = true;
            for (size_t i{0}; i < size; ++i) Value[i] = input[i];
        }
        
        constexpr operator uint64() const {
            if (size > 20) return 0;
            uint64 x{0};
            uint64 digit{1};
            for (int i{size-2}; i >= 0; --i) {
                x += digit * static_cast<uint64>(Value[i] - '0');
                digit *= 10;
            }
            return x;
        }
    };

    template <> struct decimal<0>;
    template <> struct decimal<1>;
    
    template <> struct decimal<2> {
        char Value[2] = {};
        bool Valid{false};
        
        constexpr decimal(const char (&input)[2]) noexcept {
            if (input[0] < '0' || input[0] > '9') return;
            if (input[1] != 0) return;
            Valid = true;
            Value[0] = input[0];
            Value[1] = input[1];
        }
        
        constexpr operator uint64() const {
            return static_cast<uint64>(Value[0] - '0');
        }
    };
    
    template <size_t N> decimal(const char (&)[N]) -> decimal<N>;
    template <size_t N> decimal(decimal<N>) -> decimal<N>;
}

// Peano axioms. 
namespace data::math::number::peano {
    
    constexpr decimal zero{"0"};
    
    template <auto&> struct number;
    
    template <typename> struct natural;
    
    template <typename> struct successor;
    
    // axiom 1: zero is a natural number. 
    template <> struct 
    natural<number<zero>> {};
    
    template <typename, typename> struct 
    equal;
    
    // axiom 2: equality is reflexive.
    template <typename x> struct 
    equal<x, x> {};
    
    // axiom 3: equality is symmetric. 
    template <typename x, typename y> 
    equal<y, x> symmetric_equal(equal<x, y>);
    
    // axiom 4: equality is transitive. 
    template <typename x, typename y, typename z> 
    equal<x, z> transitive_equal(equal<x, y>, equal<y, z>);
    
    template <typename x, typename y> 
    natural<y> closed_equal(equal<x, y>, natural<x>);
    
    template <typename> struct 
    suc;
    
    template <typename x> 
    natural<suc<x>> closed_successor(natural<x>);
    
    template <typename x, typename y>
    equal<suc<x>, suc<y>> injection_up(equal<x, y>);
    
    template <typename x, typename y>
    equal<x, y> injection_down(equal<suc<x>, suc<y>>);
    
    template <template<typename> typename predicate, typename x, typename y> 
    predicate<x> induction(natural<x>, predicate<number<zero>>, predicate<suc<y>> (*)(natural<y>, predicate<y>));
    
    template <typename, typename> struct add;
    
    template <typename x> struct
    equal<add<x, number<zero>>, x> {};
    
    template <typename x, typename y> struct
    equal<add<x, successor<y>>, successor<add<x, y>>> {};
    
    template <typename, typename> struct mul;
    
    template <typename x> struct
    equal<mul<x, number<zero>>, number<zero>> {};
    
    template <typename x, typename y> struct
    equal<mul<x, successor<y>>, add<x, mul<x, y>>> {};
    
}

#endif
