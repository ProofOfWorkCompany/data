// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "interface_tests.hpp"
#include "gtest/gtest.h"

// test whether data structures satisfy the correct interfaces. 
namespace data {
    
    TEST(ListsTest, TestOrderedListInterfaces) {
        
        is_sequence<ordered_list<int>>();
        is_sequence<ordered_list<int*>>();
        is_sequence<ordered_list<int&>>();
        
        is_sequence<ordered_list<const int>>();
        is_sequence<ordered_list<const int*>>();
        is_sequence<ordered_list<const int&>>();
        
        is_container<ordered_list<int>, const int>();
        is_container<ordered_list<int*>, const int*>();
        is_container<ordered_list<int&>, const int>();
        
        is_container<ordered_list<const int>, const int>();
        is_container<ordered_list<const int*>, const int*>();
        is_container<ordered_list<const int&>, const int>();
        
        is_const_iterable<ordered_list<int>>();
        is_const_iterable<ordered_list<int*>>();
        //is_const_iterable<ordered_list<int&>>();
        
        is_const_iterable<ordered_list<const int>>();
        is_const_iterable<ordered_list<const int*>>();
        //is_const_iterable<ordered_list<const int&>>();
        
    }
    
    TEST(ListsTest, TestPriorityQueueInterfaces) {
        
        is_sequence<priority_queue<int>>();
        is_sequence<priority_queue<int*>>();
        is_sequence<priority_queue<int&>>();
        
        is_sequence<priority_queue<const int>>();
        is_sequence<priority_queue<const int*>>();
        is_sequence<priority_queue<const int&>>();
        
        is_container<priority_queue<int>, const int>();
        is_container<priority_queue<int*>, const int*>();
        is_container<priority_queue<int&>, const int>();
        
        is_container<priority_queue<const int>, const int>();
        is_container<priority_queue<const int*>, const int*>();
        is_container<priority_queue<const int&>, const int>();
        
        is_const_iterable<priority_queue<int>>();
        is_const_iterable<priority_queue<int*>>();
        //is_const_iterable<priority_queue<int&>>();
        
        is_const_iterable<priority_queue<const int>>();
        is_const_iterable<priority_queue<const int*>>();
        //is_const_iterable<priority_queue<const int&>>();
        
    }
    
    TEST(ListsTest, TestListInterfaces) {
        
        is_list<list<int>>();
        is_list<list<int*>>();
        is_list<list<int&>>();
        
        is_list<list<const int>>();
        is_list<list<const int*>>();
        is_list<list<const int&>>();
        
        //is_iterable<list<int>, int>();
        is_const_iterable<list<const int>>();
        //is_iterable<list<int&>, int>();
        is_const_iterable<list<const int&>>();
        //is_iterable<list<int*>, int>();
        //is_const_iterable<list<const int*>>();
    }
    
}
