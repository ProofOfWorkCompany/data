// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_RATIONAL
#define DATA_MATH_NUMBER_RATIONAL

#include <data/math/division.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/field.hpp>
#include <data/math/ordered.hpp>

namespace data::math {
    
    template <typename X, typename Y = X> struct arg;
    
}

namespace data::math::number {
    template <typename Q>
    requires field<Q, plus<Q>, times<Q>> && ordered<Q> 
    struct rational {};
    
    template <typename Z>
    requires ring<Z, plus<Z>, times<Z>> && ordered<Z>
    struct fraction {
        using N = norm_type<Z>;
        
        struct positive {
            N Number;
            
            positive(N n) : Number{n} {}
            
            bool valid() const {
                return data::valid(Number) && Number > 0;
            }
            
            bool operator==(const positive& p) const {
                return Number == p.Number;
            }
            
            bool operator!=(const positive& p) const {
                return Number != p.Number;
            }
            
            positive operator*(const positive& p) const {
                return positive{Number * p.Number};
            }
            
            positive operator/(const positive& p) const {
                return positive{Number / p.Number};
            }
            
            operator nonzero<N>() const {
                return nonzero<N>{Number};
            }
        };
    
        Z Numerator;
        positive Denominator;
        
        bool valid() const {
            return data::valid(Numerator) && Denominator.valid();
        }
        
        fraction() : Numerator{0}, Denominator{1} {}
        
    private:
        fraction(Z n, positive d) : Numerator{n}, Denominator{d} {}
        
        static N gcd(N a, N b) {
            return euclidian::extended<N, Z>::algorithm(a, b).GCD;
        }
        
        static fraction divide(Z n, N d) {
            if (d == 0) return fraction{Z{0}, positive{N{0}}}; // Invalid value. 
            if (n == 0) return fraction{Z{0}, positive{N{1}}};
            N gcd_ab = gcd(abs<Z>{}(n), d);
            return fraction(n / Z(gcd_ab), positive(d / gcd_ab));
        }
        
    public:
        fraction(Z n, N d) : fraction(divide(n, d)) {}
        fraction(Z n) : Numerator{n}, Denominator{uint64{1}} {}
        
        bool operator==(const fraction& f) const {
            return Numerator == f.Numerator && (Numerator == 0 || Denominator == f.Denominator);
        }
        
        bool operator==(const Z& z) const {
            return operator==(fraction{z});
        }
        
        static fraction inverse(const fraction& f) {
            if (f == 0) return fraction{};
            return fraction{Z{f.Denominator.Number} * arg<Z>{}(f.Numerator), positive{abs<Z>{}(f.Numerator)}};
        }
        
        bool operator!=(const fraction& f) const {
            return !operator==(f);
        }
        
        bool operator<(const fraction& f) const;
        
        bool operator>(const fraction& f) const;
        
        bool operator<=(const fraction& f) const;
        
        bool operator>=(const fraction& f) const;
        
        fraction operator-() const {
            return {-Numerator, Denominator};
        }
        
        fraction operator+(const fraction& f) const {
            return divide(f.Numerator * Denominator.Number + Numerator * f.Denominator.Number,   
                Denominator.Number * f.Denominator.Number);
        }
        
        fraction operator+(const Z& z) const {
            return operator+(fraction{z});
        }
        
        fraction operator-(const fraction& f) const {
            return (*this)+(-f);
        }
        
        fraction operator-(const Z& z) const {
            return operator-(fraction{z});
        }
        
        fraction operator*(const fraction& f) const;
        
        fraction operator*(const Z& z) const;
        
        fraction operator/(const nonzero<fraction>& f) const {
            if (f.Value == fraction{0}) throw division_by_zero{};
            return (*this) * *inverse(f);
        }
        
        fraction operator/(const Z& z) const {
            return operator/(fraction{z});
        }
        
        fraction operator/(const N& n) const {
            return fraction{Numerator, Denominator * n};
        }
        
        nonnegative<fraction> quadrance() const {
            return fraction{Numerator * Numerator, Denominator * Denominator};
        }
        
        fraction& operator=(const fraction& f) {
            Numerator = f.Numerator;
            Denominator = f.Denominator;
        }
        
        fraction& operator+=(const fraction& f) {
            return operator=(operator+(f));
        }
        
        fraction& operator-=(const fraction& f) {
            return operator=(operator-(f));
        }
        
        fraction& operator*=(const fraction& f) {
            return operator=(operator*(f));
        }
        
        fraction& operator/=(const fraction& f) {
            return operator=(operator/(f));
        }
    };
    
}

// TODO fill in these types correctly. 
namespace data::math {
    template <typename Z> 
    struct commutative<plus<number::fraction<Z>>, number::fraction<Z>> : commutative<plus<Z>, Z> {};
    
    template <typename Z> 
    struct associative<plus<number::fraction<Z>>, number::fraction<Z>> : associative<plus<Z>, Z> {};
    
    template <typename Z> 
    struct commutative<times<number::fraction<Z>>, number::fraction<Z>> : commutative<times<Z>, Z>{};
    
    template <typename Z> 
    struct associative<times<number::fraction<Z>>, number::fraction<Z>> : associative<times<Z>, Z>{};
    
    template <typename Z> 
    struct identity<plus<number::fraction<Z>>, number::fraction<Z>> {
        static const number::fraction<Z> value() {
            return {identity<plus<Z>, Z>::value()};
        }
    };
    
    template <typename Z> 
    struct identity<times<number::fraction<Z>>, number::fraction<Z>> {
        static const number::fraction<Z> value() {
            return {identity<times<Z>, Z>::value()};
        }
    };
}

template <typename Z>
inline std::ostream& operator<<(std::ostream& o, const data::math::number::fraction<Z>& x) {
    return o << "fraction{" << x.Numerator << ", " << x.Denominator.Number << "}";
}

namespace data::math::number {
    template <typename Z>
    inline bool fraction<Z>::operator<(const fraction& f) const {
        return Numerator * f.Denominator.Number < Denominator.Number * f.Numerator;
    }
    
    template <typename Z>
    inline bool fraction<Z>::operator>(const fraction& f) const{
        return Numerator * f.Denominator.Number > Denominator.Number * f.Numerator;
    }
    
    template <typename Z>
    inline bool fraction<Z>::operator<=(const fraction& f) const{
        return Numerator * f.Denominator.Number <= Denominator.Number * f.Numerator;
    }
    
    template <typename Z>
    inline bool fraction<Z>::operator>=(const fraction& f) const{
        return Numerator * f.Denominator.Number >= Denominator.Number * f.Numerator;
    }
    
    template <typename Z>
    inline fraction<Z> fraction<Z>::operator*(const fraction& f) const {
        return divide(Numerator * f.Numerator, Denominator.Number * f.Denominator.Number);
    }
    
    template <typename Z>
    inline fraction<Z> fraction<Z>::operator*(const Z& z) const {
        return operator*(fraction{z});
    }
}

#endif
