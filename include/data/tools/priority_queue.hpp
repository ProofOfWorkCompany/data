// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_PRIORITY_QUEUE
#define DATA_TOOLS_PRIORITY_QUEUE

#include <data/functional/tree.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/linked_tree.hpp>
    
namespace data::tool {
    
    template <std::totally_ordered element, typename tree = linked_tree<element>, typename stack = linked_stack<element>> 
    requires functional::tree<tree, element> && sequence<stack, element>
    class priority_queue {
        
        tree Tree;
        priority_queue(tree t) : Tree{t} {}
        
        static tree merge(const tree& left, const tree right) {
            if (left.empty())
                return right;
            if (right.empty())
                return left;
            if (left.root() <= right.root())
                return tree{left.root(), left.left(), merge(left.right(), right)};
            else
                return tree{right.root(), right.left(), merge(left, right.right())};
        }
        
    public:
        size_t size() const;
        bool empty() const;
        
        priority_queue();
        priority_queue(const element& e);
        
        template<typename ... P>
        priority_queue(const element& a, const element& b, P... p);
        
        const element& first() const;
        priority_queue rest() const;
        
        priority_queue insert(const element& elem) const;
        
        template <typename list> requires sequence<list, element>
        priority_queue insert(list l) const;
        
        stack values() const {
            stack vals;
            auto p = *this;
            while (!p.empty()) {
                vals = vals << p.first();
                p = p.rest();
            }
            return reverse(vals);
        }
        
        template <typename list> requires sequence<list, element> 
        priority_queue(list l);
        
        using const_iterator = functional::tree_iterator<tree, const element>;
        
        const_iterator begin() const;
        const_iterator end() const;
    };
    
    template <typename element, typename tree, typename stack> 
    size_t priority_queue<element, tree, stack>::size() const {
        return Tree.size();
    }
    
    template <typename element, typename tree, typename stack> 
    bool priority_queue<element, tree, stack>::empty() const {
        return Tree.empty();
    }
    
    template <typename element, typename tree, typename stack> 
    priority_queue<element, tree, stack>::priority_queue() : Tree{} {}
    
    template <typename element, typename tree, typename stack>
    priority_queue<element, tree, stack>::priority_queue(const element& e) : priority_queue{priority_queue{}.insert(e)} {}
    
    template <typename element, typename tree, typename stack>
    template <typename ... P>
    priority_queue<element, tree, stack>::priority_queue(const element& a, const element& b, P... p) : 
        priority_queue{priority_queue{b, p...}.insert(a)} {} 
    
    template <typename element, typename tree, typename stack> 
    const element& priority_queue<element, tree, stack>::first() const {
        return Tree.root();
    }
    
    template <typename element, typename tree, typename stack> 
    priority_queue<element, tree, stack> priority_queue<element, tree, stack>::rest() const {
        if (empty()) return *this;
        return {merge(Tree.left(), Tree.right())};
    }
    
    template <typename element, typename tree, typename stack> 
    priority_queue<element, tree, stack> priority_queue<element, tree, stack>::insert(const element& elem) const {
        return {merge(tree{elem}, Tree)};
    }
    
    template <typename element, typename tree, typename stack> 
    template <typename list> requires sequence<list, element>
    priority_queue<element, tree, stack> priority_queue<element, tree, stack>::insert(list l) const {
        if (l.empty()) return *this;
        return insert(l.first()).insert(l.rest());
    }
    
    template <typename element, typename tree, typename stack> 
    template <typename list> requires sequence<list, element> 
    priority_queue<element, tree, stack>::priority_queue(list l) : priority_queue{priority_queue{}.insert(l)} {}
    
    template <typename element, typename tree, typename stack> 
    priority_queue<element, tree, stack>::const_iterator priority_queue<element, tree, stack>::begin() const {
        return const_iterator{Tree};
    }
    
    template <typename element, typename tree, typename stack> 
    priority_queue<element, tree, stack>::const_iterator priority_queue<element, tree, stack>::end() const {
        return const_iterator{size()};
    }
    
}

#endif
