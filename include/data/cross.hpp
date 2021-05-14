// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CROSS
#define DATA_CROSS

#include <data/sequence.hpp>
#include <data/indexed.hpp>
#include <data/slice.hpp>
#include <data/encoding/endian.hpp>
#include <data/valid.hpp>

namespace data {
    
    // The cartesian product. 
    // it is the same as a vector with some slight improvements. 
    template <typename X> struct cross : std::vector<X> {
        cross();
        cross(size_t size);
        cross(size_t size, X fill);
        
        cross(std::initializer_list<X> x);
        
        template<sequence list>
        explicit cross(list l);
        
        bool valid() const {
            for (const X& x : *this) if (!data::valid(x)) return false;
            return true;
        }
        
        X& operator[](int i) {
            return std::vector<X>::operator[](i < 0 ? std::vector<X>::size() + i : i);
        }
        
        const X& operator[](int i) const {
            return std::vector<X>::operator[](i < 0 ? std::vector<X>::size() + i : i);
        }
        
        explicit operator slice<X>();
        
        view<X> range(int) const;
        view<X> range(int, int) const;
        
        slice<X> range(int);
        slice<X> range(int, int);
        
        slice<X> range(data::range r);
    };
    
    template <typename X>
    std::ostream& operator<<(std::ostream& o, const cross<X>& s) {
        auto b = s.begin();
        while (true) {
            if (b == s.end()) return o << "]";
            else if (b == s.begin()) o << "[";
            else o << ", ";
            o << *b;
            b++;
        }
    }
    
    template <typename X, size_t size> struct array : public cross<X> {
        array() : cross<X>{size} {}
        
        static array fill(X x) {
            array n{};
            for (const X& z : n) z = x;
            return n;
        }
        
    protected:
        template <std::incrementable iterator>
        array(iterator it) : array() {
            for (X& x : *this) {
                x = *it; 
                it++;
            }
        }
    };
    
    struct bytes : cross<byte> {
        using cross<byte>::cross;
        operator bytes_view() const {
            return bytes_view(cross<byte>::data(), cross<byte>::size());
        }
        bytes(bytes_view x) : cross<byte>(x.size()) {
            std::copy(x.begin(), x.end(), cross<byte>::begin());
        }
    };
    
    template <size_t size> struct byte_array : public array<byte, size> {
        byte_array() : array<byte, size>{} {}
        byte_array(bytes_view v) : array<byte, size>{v.size() == size ? array<byte, size>{v.begin()} : array<byte, size>{}} {}
        operator bytes_view() const;
        
        byte_array operator~() const;
        
        byte_array operator<<(int32) const;
        byte_array operator>>(int32) const;
        
        byte_array operator|(const byte_array&) const;
        byte_array operator&(const byte_array&) const;
        
        bool operator==(const byte_array&) const;
        bool operator!=(const byte_array&) const;
        
        operator slice<byte, size>() const;
        
    };

    std::ostream& operator<<(std::ostream& o, const bytes& s);
    
    template <typename X>
    inline cross<X>::cross() : std::vector<X>{} {}
    
    template <typename X>
    inline cross<X>::cross(size_t size) : std::vector<X>(size) {}
    
    template <typename X>
    inline cross<X>::cross(size_t size, X fill) : std::vector<X>(size) {
        for (auto it = std::vector<X>::begin(); it < std::vector<X>::end(); it++) *it = fill;
    }
    
    template <typename X>
    inline cross<X>::cross(std::initializer_list<X> x) : std::vector<X>{x} {}
    
    template <typename X>
    template<sequence list>
    cross<X>::cross(list l) : cross{} {
        std::vector<X>::resize(data::size(l));
        auto b = std::vector<X>::begin();
        while (!l.empty()) {
            *b = l.first();
            l = l.rest();
        }
    }
    
    template <typename X>
    inline cross<X>::operator slice<X>() {
        return slice<X>(static_cast<std::vector<X>&>(*this));
    }
    
    template <typename X>
    inline view<X> cross<X>::range(int e) const {
        return operator slice<X>().range(e);
    }
    
    template <typename X>
    inline view<X> cross<X>::range(int b, int e) const {
        return operator slice<X>().range(b, e);
    }
        
    template <typename X>
    inline slice<X> cross<X>::range(int e) {
        return operator slice<X>().range(e);
    }
    
    template <typename X>
    inline slice<X> cross<X>::range(int b, int e) {
        return operator slice<X>().range(e);
    }
        
    template <typename X>
    inline slice<X> cross<X>::range(data::range r) {
        return operator slice<X>().range(r);
    }
}

#endif


