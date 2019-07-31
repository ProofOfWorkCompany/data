// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_SHA256
#define DATA_CRYPTO_SHA256

#include <data/math/number/bounded.hpp>
#include <data/slice.hpp>

namespace data {
    
    namespace sha256 {

        struct digest : public uint256 {
            
            digest(uint32_t n) : uint256{n} {}
            digest() : digest(0) {}
            digest(const digest& d) : uint256{d} {}
            digest(digest&& d) : uint256{static_cast<uint256&&>(d)} {};
            digest(const std::array<byte, 32>& a) : uint256{a} {}
            digest(std::array<byte, 32>&& a) : uint256{a} {}
            digest(bytes& b) : uint256{b} {}
            
            bool valid() const;
            bool operator==(const digest& d) const;
            bool operator!=(const digest& d) const;
            bool operator>=(const digest& d) const;
            bool operator<=(const digest& d) const;
            bool operator>(const digest& d) const;
            bool operator<(const digest& d) const;
            
            using uint256::operator[];
            
            digest& operator=(const digest& d);
        };
        
        const digest Zero = digest{};

        digest hash(bytes&);
        
        template <uint32 n>
        digest hash(const std::array<byte, n>&);
        
        inline bool digest::valid() const {
            return operator!=(sha256::Zero);
        }
            
        inline bool digest::operator==(const digest& d) const {
            return uint256::operator==(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator!=(const digest& d) const {
            return uint256::operator!=(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator>=(const digest& d) const {
            return uint256::operator>=(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator<=(const digest& d) const {
            return uint256::operator<=(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator>(const digest& d) const {
            return uint256::operator>(static_cast<const uint256&>(d));
        }
        
        inline bool digest::operator<(const digest& d) const {
            return uint256::operator<(static_cast<const uint256&>(d));
        }
        
        inline digest& digest::operator=(const digest& d) {
            uint256::operator=(static_cast<const uint256&>(d));
            return *this;
        }
    
    }

}

#endif
