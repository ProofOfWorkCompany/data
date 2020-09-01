// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_POINT
#define DATA_POINT

#include <data/iterable.hpp>

namespace data {
    
    template <typename X, size_t size> struct vector : array<X, size> {
        using array<X, size>::array;
        
        vector operator+(const vector& v) const {
            vector w;
            for (int i = 0; i < size; i++) w[i] = array<X, size>::operator[](i) + v[i];
            return w;
        }
        
        vector operator-(const vector& v) const {
            vector w;
            for (int i = 0; i < size; i++) w[i] = array<X, size>::operator[](i) - v[i];
            return w;
        }
        
        vector operator*(const X& v) const {
            vector w;
            for (int i = 0; i < size; i++) w[i] = array<X, size>::operator[](i) - v[i];
            return w;
        }
    };

    template <typename X, size_t size> struct point : array<X, size> {
        using array<X, size>::array;
        
        point operator+(const vector<X, size>& v) const {
            point q;
            for (int i = 0; i < size; i++) q[i] = array<X, size>::operator[](i) + v[i];
            return q;
        }
        
        vector<X, size> operator-(const point& p) const {
            vector<X, size> w;
            for (int i = 0; i < size; i++) w[i] = array<X, size>::operator[](i) - p[i];
            return w;
        }
    };
    
}

#endif

