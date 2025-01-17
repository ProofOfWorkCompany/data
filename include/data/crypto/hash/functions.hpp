// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CRYPTO_HASH_FUNCTIONS
#define DATA_CRYPTO_HASH_FUNCTIONS

#include "digest.hpp"
#include <data/crypto/one_way.hpp>

namespace data::crypto::hash {
    
    // a cryptographic hash function also needs to be a one-way function
    // and indistinguishable from a random function. 
    template <typename f, typename d = digest<f::size>> 
    concept function = requires {
        { f::size };
    } && one_way<f, d, bytes_view>;
    
    // supported hash functions.
    digest<16> RIPEMD_128(bytes_view);
    digest<20> RIPEMD_160(bytes_view);
    digest<32> RIPEMD_256(bytes_view);
    digest<40> RIPEMD_320(bytes_view);
    
    digest<28> SHA2_224(bytes_view);
    digest<32> SHA2_256(bytes_view);
    digest<48> SHA2_384(bytes_view);
    digest<64> SHA2_512(bytes_view);
    
    digest<28> SHA3_224(bytes_view);
    digest<32> SHA3_256(bytes_view);
    digest<48> SHA3_384(bytes_view);
    digest<64> SHA3_512(bytes_view);
    
    digest<20> Bitcoin_160(bytes_view);
    digest<32> Bitcoin_256(bytes_view);
    
    template <typename W> 
    concept writer = requires {
        { W{} };
        { W::size };
    } && requires (W w, const byte *b, size_t x) {
        { w.write(b, x) };
    } && requires(W w) {
        { w.finalize() } -> std::same_as<digest<W::size>>;
    };
    
    template <writer W>
    digest<W::size> inline calculate(bytes_view b) {
        W w{};
        w.write(b.data(), b.size());
        return w.finalize();
    }
    
    // these are both functions and writers. 
    struct SHA1;
    template <size_t size> struct RIPEMD;
    template <size_t size> struct SHA2;
    template <size_t size> struct SHA3;
    template <size_t size> struct Bitcoin;

}

#endif

