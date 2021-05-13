// Copyright (c) 2019 - 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_ABS
#define DATA_MATH_NUMBER_ABS

#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/division.hpp>

namespace data::math {
    
    template <typename X> struct square {
        X operator()(const X& x) {
            return x * x;
        }
    };
    
    template <typename X> struct normed;
    
    template <typename X> using quad_type = typename normed<X>::quad_type;
    
    template <typename X> using norm_type = typename normed<X>::norm_type;
    
    template <> struct normed<uint64> {
        using quad_type = uint64;
        using norm_type = uint64;
    };
    
    template <> struct normed<int64> {
        using quad_type = uint64;
        using norm_type = uint64;
    };
}

namespace data::interface {
    
    template <typename X>
    concept has_quad_type = requires {
        typename math::normed<X>;
    } && ordered<math::quad_type<X>>;
    
    template <typename X>
    concept has_norm_type = has_quad_type<X> && ordered<math::norm_type<X>>;
}

namespace data::math {
    
    template <interface::has_quad_type X> struct quadrance;
    
    template <typename X> struct abs;
    
    template <interface::has_norm_type X> struct norm;
    
    template <typename X> struct conjugate;
    
    template <interface::has_quad_type X, interface::has_quad_type Y = X> struct contract;
    
    template <interface::has_quad_type X> struct inner;
    
    template <> struct abs<uint64> {
        uint64 operator()(uint64 x) {
            return x;
        }
    };
    
    template <> struct abs<int64> {
        uint64 operator()(int64 x) {
            return x < 0 ? -x : x;
        }
    };
    
    template <> struct conjugate<uint64> {
        uint64 operator()(uint64 x) {
            return x; 
        }
    };
    
    template <> struct conjugate<int64> {
        uint64 operator()(int64 x) {
            return x;
        }
    };
}

namespace data::interface {
    
    template <typename X>
    concept has_abs_method = has_norm_type<X> && requires(const X x) {
        { x.abs() } -> std::same_as<math::norm_type<X>>;
    };
    
    template <typename X>
    concept has_abs = has_norm_type<X> && requires(const X x) {
        typename math::abs<X>;
        { math::abs<X>{}(x) } -> std::same_as<math::norm_type<X>>;
    };
    
    template <typename X>
    concept has_norm_method = has_norm_type<X> && requires(const X x) {
        { x.norm() } -> std::same_as<math::norm_type<X>>;
    };
    
    template <typename X>
    concept has_norm = has_norm_type<X> && requires(const X x) {
        typename math::norm<X>;
        { math::norm<X>{}(x) } -> std::same_as<math::norm_type<X>>;
    };
    
    // following Wildburger
    template <typename X>
    concept has_quadrance_method = has_quad_type<X> && requires(const X x) {
        { x.quadrance() } -> std::same_as<math::quad_type<X>>;
    };
    
    template <typename X>
    concept has_quadrance = has_quad_type<X> && requires(const X x) {
        { math::quadrance<X>{}(x) } -> std::same_as<math::quad_type<X>>;
    };
    
    template <typename X, typename Y = X>
    concept is_conjugate_pair = std::same_as<math::quad_type<X>, math::quad_type<Y>> && requires(const X x, const Y y) {
        { math::conjugate<X>{}(x) } -> std::same_as<Y>;
        { math::conjugate<Y>{}(y) } -> std::same_as<X>;
        { math::contract<X, Y>{}(x, y) } -> std::same_as<math::quad_type<X>>;
    };
    
    template <typename X>
    concept has_conjugate_method = requires(const X x) {
        { x.conjugate() } -> is_conjugate_pair<X>;
    };
    
    template <typename X>
    concept has_conjugate = requires(const X x) {
        { math::conjugate<X>{}(x) } -> is_conjugate_pair<X>;
    };
    
    template <typename X>
    concept has_inner_method = requires(const X a, const X b) {
        { a.inner(b) } -> std::same_as<math::quad_type<X>>;
    };
    
    template <typename X>
    concept has_inner = requires(const X a, const X b) {
        { math::inner<X>{}(a, b) } -> std::same_as<math::quad_type<X>>;
    };
}

namespace data {

    template <interface::has_quadrance X> 
    inline math::quad_type<X> quadrance(const X& x) {
        return math::quadrance<X>{}(x);
    }

    template <interface::has_abs X>
    inline math::norm_type<X> abs(const X& x) {
        return math::abs<X>{}(x);
    }

    template <interface::has_norm X>
    inline math::norm_type<X> norm(const X& x) {
        return math::norm<X>{}(x);
    }
    
    template <interface::has_conjugate X>
    inline auto conjugate(const X& x) -> decltype(math::conjugate<X>{}(x)) {
        return math::conjugate<X>{}(x);
    }

    template <typename X, typename Y = X> requires interface::is_conjugate_pair<X, Y>
    inline math::quad_type<X> contract(const X& a, const Y& b) {
        return math::contract<X, Y>{}(a, b);
    }

    template <interface::has_inner X> 
    inline math::quad_type<X> inner(const X& a, const X& b) {
        return math::inner<X>{}(a, b);
    }
}

namespace data::math {
    
    template <interface::has_abs_method X> struct abs<X> {
        norm_type<X> operator()(const X& x) const {
            return x.abs();
        }
    };
    
    template <interface::has_norm_method X> struct norm<X> {
        norm_type<X> operator()(const X& x) const {
            return x.norm();
        }
    };
    
    template <interface::has_norm_type X> requires interface::has_abs<X>
    struct norm<X> {
        norm_type<X> operator()(const X& x) const {
            return data::abs(x);
        }
    };
    
    template <interface::has_quad_type X> requires interface::has_conjugate_method<X>
    struct conjugate<X> {
        auto operator()(const X& x) -> decltype(x.conjugate()) const {
            return x.conjugate();
        }
    };
    
    template <interface::has_quadrance_method X> struct quadrance<X> {
        quad_type<X> operator()(const X& x) const {
            return x.quadrance();
        }
    };
    
    template <interface::has_quad_type X> 
    requires interface::has_norm<X> && std::same_as<norm_type<X>, quad_type<X>>
    struct quadrance<X> {
        quad_type<X> operator()(const X& x) const {
            auto n = data::norm(x);
            return n * n;
        }
    };
    
    template <interface::has_quad_type X>
    requires interface::has_inner<X>
    struct quadrance<X> {
        quad_type<X> operator()(const X& x) const {
            return data::inner(x, x); 
        }
    };
    
    template <interface::has_quad_type X>
    requires interface::has_conjugate<X>
    struct inner<X> {
        quad_type<X> operator()(const X& a, const X& b) const {
            return data::contract(a, data::conjugate(b)); 
        }
    };
}

#endif

