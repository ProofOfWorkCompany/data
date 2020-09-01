// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "interface_tests.hpp"
#include "gtest/gtest.h"

// test whether data structures satisfy the correct interfaces. 
namespace data {
    
    TEST(IterationTest, TestIteration) {
        stack<int> s0{};
        list<int> l0{};
        ordered_list<int> o0{};
        priority_queue<int> p0{};
        tree<int> t0{};
        map<int, int> m0{};
        
        EXPECT_EQ(s0.begin(), s0.end());
        EXPECT_EQ(l0.begin(), l0.end());
        EXPECT_EQ(o0.begin(), o0.end());
        EXPECT_EQ(p0.begin(), p0.end());
        EXPECT_EQ(t0.begin(), t0.end());
        EXPECT_EQ(m0.begin(), m0.end());
        
        stack<int> s1{1};
        list<int> l1{1};
        ordered_list<int> o1{1};
        priority_queue<int> p1{1};
        tree<int> t1{1};
        map<int, int> m1{{1, 1}};
        
        EXPECT_NE(s1.begin(), s1.end());
        EXPECT_NE(l1.begin(), l1.end());
        EXPECT_NE(o1.begin(), o1.end());
        EXPECT_NE(p1.begin(), p1.end());
        EXPECT_NE(t1.begin(), t1.end());
        EXPECT_NE(m1.begin(), m1.end());
        
        EXPECT_EQ(++s1.begin(), s1.end());
        EXPECT_EQ(++l1.begin(), l1.end());
        EXPECT_EQ(++o1.begin(), o1.end());
        EXPECT_EQ(++p1.begin(), p1.end());
        EXPECT_EQ(++t1.begin(), t1.end());
        EXPECT_EQ(++m1.begin(), m1.end());
        
        entry<int, int> expected_entry{1, 1};
        
        EXPECT_EQ(*s1.begin(), 1);
        EXPECT_EQ(*l1.begin(), 1);
        EXPECT_EQ(*o1.begin(), 1);
        EXPECT_EQ(*p1.begin(), 1);
        EXPECT_EQ(*t1.begin(), 1);
        EXPECT_EQ(*m1.begin(), expected_entry);
        
    }
    
}
