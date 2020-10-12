// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iterator>
#include <iostream>
#include <vector>
#include <string>

#include <data/encoding/hex.hpp>
#include <data/encoding/endian.hpp>

namespace data::encoding::hex {
    
    string::operator bytes() const {
        if (!valid()) return {};
        return bytes(bytes_view(view{*this}));
    }
    
    view::view(string_view sourceString) : string_view{sourceString}, Bytes((sourceString.size() + 1) / 2), ToBytes{nullptr} {
        try {
            boost::algorithm::unhex(string_view::begin(), string_view::end(), Bytes.begin());
        }
        catch(boost::algorithm::hex_decode_error exception)
        {
            return;
        }
        ToBytes=&Bytes;
    }
    
    void write_hex(string& output, bytes_view sourceBytes, letter_case q) {
        output.resize(sourceBytes.size() * 2);
        if (q == upper) boost::algorithm::hex(sourceBytes.begin(), sourceBytes.end(), output.begin());
        else boost::algorithm::hex_lower(sourceBytes.begin(), sourceBytes.end(), output.begin());
    }
    
    string write(bytes_view sourceBytes, letter_case q) {
        string output;
        write_hex(output, sourceBytes, q);
        return output;
    }
    
    string write(bytes_view sourceBytes, endian::order r, letter_case q) {
        if (r == endian::big) return write(sourceBytes, q);
        bytes reversed(sourceBytes.size());
        std::copy(sourceBytes.rbegin(), sourceBytes.rend(), reversed.begin());
        return write(reversed, q);
    }
    
    fixed<8> write(uint64 x, letter_case q) {
        fixed<8> output;
        write_hex(output, bytes_view{uint64_big{x}.data(), sizeof(uint64)}, q);
        return output;
    }
    
    fixed<4> write(uint32 x, letter_case q) {
        fixed<4> output;
        write_hex(output, bytes_view{uint32_big{x}.data(), sizeof(uint32)}, q);
        return output;
    }
    
    fixed<2> write(uint16 x, letter_case q) {
        fixed<2> output;
        write_hex(output, bytes_view{uint16_big{x}.data(), sizeof(uint16)}, q);
        return output;
    }
    
    fixed<1> write(byte x, letter_case q) {
        fixed<1> output;
        write_hex(output, bytes_view{(byte*)(&x), sizeof(byte)}, q);
        return output;
    }
}
