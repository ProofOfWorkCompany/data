// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_CAYLEY_DICKSON
#define DATA_MATH_CAYLEY_DICKSON

#include <data/math/field.hpp>
#include <data/math/abs.hpp>

namespace data::math {
    
    template <typename X> struct re;
    
    template <typename X> struct im;
}

namespace data {

    template <typename X> 
    inline auto re(const X& x) -> decltype(math::re<X>{}()) {
        return math::re<X>{}(x);
    }

    template <typename X> 
    inline auto im(const X& x) -> decltype(math::re<X>{}()) {
        return math::im<X>{}(x);
    }

}
namespace data::interface {
    
    template <typename X>
    concept has_re_method = requires(X x) {
        { x.re() };
    };
    
    template <typename X>
    concept has_im_method = requires(X x) {
        { x.im() };
    };
}

namespace data::math {
    
    template <typename X> 
    requires interface::has_re_method<X>
    struct re<X> {
        auto operator()(const X& x) -> decltype(x.re()) {
            return x.re();
        }
    };
    
    template <typename X>
    requires interface::has_im_method<X>
    struct im<X> {
        auto operator()(const X& x) -> decltype(x.im()) {
            return x.im();
        }
    };
    
    template <typename nda, typename q>
    requires ordered<q> && field<q>
    struct cayley_dickson {
        // TODO use proper interfaces: 
        //   nda is a normed division algebra. 
        
        nda Re;
        nda Im;
        
        cayley_dickson() : Re{0}, Im{0} {}
        cayley_dickson(const nda& re, const nda& im) : Re{re}, Im{im} {}
        cayley_dickson(const q& x) : Re{x}, Im{0} {}
        
        cayley_dickson conjugate() const {
            return {math::conjugate<q>{}(Re), -Im};
        }
        
        cayley_dickson operator~() const {
            return conjugate();
        }
        
        q re() const {
            return math::re(Re);
        }
        
        cayley_dickson operator+(const cayley_dickson& x) const {
            return {Re + x.Re, Im + x.Im};
        }
        
        cayley_dickson operator-() const {
            return {-Re, -Im};
        }
        
        cayley_dickson operator-(const cayley_dickson& x) const {
            return {Re - x.Re, Im - x.Im};
        }
        
        cayley_dickson operator*(const cayley_dickson& x) const {
            return {Re * x.Re - x.Im * ~Im, ~Re * x.Im + x.Re * Im};
        }
        
    protected:
        nonnegative<q> quadrance() const {
            return operator*(conjugate()).real_part();
        }
        
        cayley_dickson inverse() const {
            return conjugate() / q(quadrance());
        }
        
        cayley_dickson operator/(const cayley_dickson& x) const {
            return operator*(x.inverse());
        }
    };
    
    template <typename nda, typename q> struct normed<cayley_dickson<nda, q>> {
        using quad_type = nonnegative<q>;
        using norm_type = nonnegative<q>;
    };
    
}

#endif
