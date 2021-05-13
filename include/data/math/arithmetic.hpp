// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ARITHMETIC
#define DATA_MATH_ARITHMETIC

#include <data/encoding/halves.hpp>

namespace data::interface {
    template <typename X, typename Y>
    concept has_mod_operator = requires(X x, Y y) {
        { x % y } -> std::same_as<X>;
    };
    
    template <typename X>
    concept has_negative_operator = requires(X x) {
        { -x } -> std::same_as<X>;
    };
    
    template <typename X, typename Y>
    concept has_bit_shift_left_operator = requires(X x, Y y) {
        { x << y } -> std::same_as<X>;
    };
    
    template <typename X, typename Y>
    concept has_bit_shift_right_operator = requires(X x, Y y) {
        { x >> y } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X>
    concept has_exp_operator = requires(X x, Y y) {
        { x ^ y } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X> 
    concept has_plus_operator = requires(X x, Y y) {
        { x.operator+(y) } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X> 
    concept has_minus_operator = requires(X x, Y y) {
        { x.operator-(y) } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X> 
    concept has_times_operator = requires(X x, Y y) {
        { x.operator*(y) } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X> 
    concept has_divide_operator = requires(X x, Y y) {
        { x.operator/(y) } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X> 
    concept has_plus_equal_operator = requires(X& x, Y y) {
        { x.operator+=(y) } -> std::same_as<X&>;
    };
    
    template <typename X, typename Y = X> 
    concept has_minus_equal_operator = requires(X& x, Y y) {
        { x.operator-=(y) } -> std::same_as<X&>;
    };
    
    template <typename X, typename Y = X> 
    concept has_times_equal_operator = requires(X& x, Y y) {
        { x.operator*=(y) } -> std::same_as<X&>;
    };
    
    template <typename X> 
    concept has_left_shift_equal_operator = requires(X& x, int64 y) {
        { x.operator<<=(y) } -> std::same_as<X&>;
    };
    
    template <typename X> 
    concept has_right_shift_equal_operator = requires(X& x, int64 y) {
        { x.operator>>=(y) } -> std::same_as<X&>;
    };
}

namespace data::math {
    
    template <typename X, typename Y = X> 
    concept additive = requires(X x, Y y) {
        { x + y } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X, typename Z = X> 
    concept multiplicative = requires(Y x, Z y) {
        { x * y } -> std::same_as<X>;
    };
    
    template <typename X, typename Y = X> 
    concept inverse_additive = requires(X a, X b) {
        { a - b } -> std::same_as<Y>;
    };
    
    template <typename quotient, typename dividend = quotient, typename divisor = quotient> 
    concept inverse_multiplicative = requires(dividend a, divisor b) {
        { a / b } -> std::same_as<quotient>;
    };

    template <typename X, typename Y = X> requires additive<X, Y>
    struct plus {
        X operator()(const X& a, const Y& b) {
            return a + b;
        }
    };

    template <typename X, typename Y = X, typename Z = X> requires multiplicative<X, Y, Z>
    struct times {
        Z operator()(const X& a, const Y& b) {
            return a * b;
        }
    }; 

    template <typename X, typename Y = X> requires inverse_additive<X, Y> 
    struct minus {
        Y operator()(const X& a, const X& b) {
            return a - b;
        }
    };
    
    template <typename f, typename x> struct associative;
    template <typename f, typename x> struct commutative;
    template <typename f, typename x> struct identity;
    template <typename x, typename f, typename... y> struct inverse;
    
    template <typename f, typename x> 
    concept monoid = requires(f fun, x a, x b) {
        typename associative<f, x>;
        { fun(a, b) } -> std::same_as<x>;
        { identity<f, x>{}() } -> std::same_as<x>;
    };
    
    template <typename x, typename f, typename... y> 
    concept invertible = requires (x a, f fun, y... b) {
        { fun(b..., inverse<x, f, y...>::invert(a, fun, b...)) } -> std::same_as<x>;
    };
    
}

namespace data {

    template <typename X, typename Y = X> 
    requires math::additive<X, Y> 
    inline X plus(const X& a, const Y& b) {
        return math::plus<X, Y>{}(a, b);
    }

    template <typename Y, typename Z = Y> 
    requires math::multiplicative<decltype(std::declval<Y>() * std::declval<Z>()), Y, Z>
    inline auto times(const Y& a, const Z& b) -> decltype(a * b) {
        return math::times<decltype(a * b), Y, Z>{}(a, b);
    } 

    template <typename X> 
    requires math::inverse_additive<decltype(std::declval<X>() * std::declval<X>()), X>
    inline auto minus(const X& a, const X& b) -> decltype(a - b) {
        return math::minus<decltype(a - b), X>{}(a, b);
    }

    template <typename X, typename Y = X> 
    requires interface::has_mod_operator<X, Y>
    inline X mod(const X& a, const Y& b) {
        return a % b;
    }

}

// declare commutivity and associativity for standard types
namespace data::math {
    template <> struct commutative<plus<uint32>, uint32> {};
    template <> struct associative<plus<uint32>, uint32> {};
    template <> struct commutative<times<uint32>, uint32> {};
    template <> struct associative<times<uint32>, uint32> {};

    template <> struct identity<plus<uint32>, uint32> {
        uint32 operator()() {
            return 1;
        }
    };
    
    template <> struct identity<times<uint32>, uint32> {
        uint32 operator()() {
            return 0;
        }
    };
    
    template <> struct commutative<plus<uint64>, uint64> {};
    template <> struct associative<plus<uint64>, uint64> {};
    template <> struct commutative<times<uint64>, uint64> {};
    template <> struct associative<times<uint64>, uint64> {};

    template <> struct identity<plus<uint64>, uint64> {
        uint64 operator()() {
            return 1;
        }
    };
    
    template <> struct identity<times<uint64>, uint64> {
        uint64 operator()() {
            return 0;
        }
    };
    
    template <> struct inverse<int64, plus<int64>, int64> {
        static int64 invert(const int64& a, const plus<int64>&, const int64& b) {
            return a - b;
        }
    };
}

namespace data::math::arithmetic {
    
    template <typename sen, std::input_or_output_iterator ita, std::incrementable itb>
    void bit_negate(sen z, ita i, itb j) {
        while (i != z) {
            *i = ~ *j;
            i++;
            j++;
        }
    }
    
    template <typename sen, std::input_or_output_iterator it, std::incrementable ita, std::incrementable itb>
    void bit_and(sen z, it i, ita a, itb b) {
        while (i != z) {
            *i = *a & *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename sen, std::input_or_output_iterator it, std::incrementable ita, std::incrementable itb>
    void bit_or(sen z, it i, ita a, itb b) {
        while (i != z) {
            *i = *a | *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename sen, std::input_or_output_iterator it, std::incrementable ita, std::incrementable itb>
    void bit_xor(sen z, it i, ita a, itb b) {
        while (i != z) {
            *i = *a ^ *b;
            i++;
            a++;
            b++;
        }
    }
    
    template <typename sen, std::input_or_output_iterator ita, std::incrementable itb>
    bool equal(sen z, ita i, itb j) {
        while (i != z) {
            if(*i != *j) return false;
            i++;
            j++;
        }
    }
    
    template <typename sen, std::incrementable ita, std::incrementable itb>
    bool less(sen z, ita i, itb j) {
        while (i != z) {
            if(*i < *j) return true;
            if(*i > *j) return false;
            i++;
            j++;
        }
        return false;
    }
    
    template <typename sen, std::incrementable ita, std::incrementable itb>
    bool greater(sen z, ita i, itb j) {
        while (i != z) {
            if(*i < *j) return false;
            if(*i > *j) return true;
            i++;
            j++;
        }
        return false;
    }
    
    template <typename sen, std::incrementable ita, std::incrementable itb>
    bool less_equal(sen z, ita i, itb j) {
        while (i != z) {
            if(*i < *j) return true;
            if(*i > *j) return false;
            i++;
            j++;
        }
        return true;
    }
    
    template <typename sen, std::incrementable ita, std::incrementable itb>
    bool greater_equal(sen z, ita i, itb j) {
        while (i != z) {
            if(*i < *j) return false;
            if(*i > *j) return true;
            i++;
            j++;
        }
        return true;
    }
    
    template <typename sen, std::incrementable ita, std::incrementable itb>
    void plus(sen z, ita i, itb a, itb b) {
        using digit = decltype(*i);
        using two_digits = typename twice<digit>::type;
        
        digit remainder = 0;
        
        while (i != z) {
            two_digits result = add(*a, *b, remainder);
            remainder = greater(result);
            *i = lesser(result);
            i++;
            a++;
            b++;
        }
    }
    
}

#endif

