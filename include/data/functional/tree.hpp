// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TREE
#define DATA_TREE

#include <data/functional/list.hpp>
#include <data/functional/set.hpp>
#include <data/tools/linked_stack.hpp>
#include <data/tools/iterator_list.hpp>
    
namespace data::interface {
        
    template <typename tree>
    concept has_left_method = requires (tree t) {
        { t.left() } -> std::convertible_to<tree>;
    };
    
    template <typename tree>
    concept has_right_method  = requires (tree t) {
        { t.right() } -> std::convertible_to<tree>;
    };
        
    template <typename tree, typename element>
    concept has_root_method = requires (tree t) {
        { t.root() } -> std::convertible_to<element>;
    };

    template <typename tree, typename element>
    concept has_tree_constructor = requires (element e, tree r, tree l) {
        { tree{e, r, l} };
    } && requires (element e) {
        { tree{e} };
    };
    
}
    
namespace data::functional {
    
    template <typename T, typename element = decltype(std::declval<T>().root())>
    concept tree = container<const T, element> && 
        interface::has_left_method<const T> && 
        interface::has_right_method<const T> && 
        interface::has_root_method<const T, element> && 
        interface::has_tree_constructor<T, element> && 
        std::default_initializable<T>;
    
    template <typename T, typename X> requires tree<T, X>
    T insert(T t, X x) {
        if (empty(t)) return T{x, T{}, T{}};
        X& r = root(t);
        if (x == r) return t;
        if (x < r) return T{r, insert(left(t), x), right(t)};
        if (x > r) return T{r, left(t), insert(right(t))};
    }
    
    template <typename value, typename tree>
    struct tree_node final {
        value Value;
        tree Left;
        tree Right;
        tree_node(const value& v, tree l, tree r) : Value{v}, Left{l}, Right{r} {}
    };
    
    template <typename tree, typename value>
    class tree_iterator {
        tool::linked_stack<tree> Branches;
        tree Current;
        size_t Index;
    
        tree_iterator(tool::linked_stack<tree> b, tree c, size_t i) : Branches{b}, Current{c}, Index{i} {}
    public:
        tree_iterator(size_t size) : Branches{}, Current{}, Index{size} {}
        tree_iterator(tree t) : Branches{}, Current{t}, Index{0} {}
        tree_iterator() : Branches{}, Current{}, Index{0} {}
        
        value& operator*() const;
        
        tree_iterator& operator++();
        tree_iterator operator++(int);
        
        bool operator==(const tree_iterator&) const;
        
        int operator-(const tree_iterator& i) const;
    };
    
}

namespace std {
    
    template <typename tree, typename elem> 
    struct iterator_traits<data::functional::tree_iterator<tree, elem>> {
        using value_type = remove_const_t<elem>;
        using difference_type = int;
        using pointer = remove_reference_t<elem>*;
        using reference = elem&;
        using iterator_concept = input_output_iterator_tag;
    };
    
    template <typename tree, typename elem> 
    struct iterator_traits<data::functional::tree_iterator<tree, const elem>> {
        using value_type = remove_const_t<elem>;
        using difference_type = int;
        using pointer = const remove_reference_t<elem>*;
        using reference = const elem&;
        using iterator_concept = input_iterator_tag;
    };
}

namespace data {

    template <typename X, typename elem> requires interface::has_root_method<X, elem>
    inline const decltype(std::declval<const X>().root()) root(const X& x) {
        return x.root();
    }
    
    template <typename X>
    inline X left(const X& x) {
        return x;
    }
    
    template <interface::has_left_method X>
    inline X left(const X& x) {
        return x.left();
    }
    
    template <typename X>
    inline X right(const X& x) {
        return x;
    }
    
    template <interface::has_right_method X>
    inline X right(const X& x) {
        return x.right();
    }

}

namespace data::functional {
    
    template <typename tree, typename value>
    inline value& tree_iterator<tree, value>::operator*() const {
        return Current.root();
    }
    
    template <typename tree, typename value>
    inline bool tree_iterator<tree, value>::operator==(const tree_iterator& i) const {
        return Current == i.Current && Branches == i.Branches && Index == i.Index;
    }
    
    template <typename tree, typename value>
    tree_iterator<tree, value>& tree_iterator<tree, value>::operator++() {
        if (Current.size() == 0) *this = tree_iterator{Index};
        if (Current.left().size() != 0) {
            if (Current.right().size() != 0) *this = tree_iterator{Branches << Current.right(), Current.left(), Index + 1};
            else *this = tree_iterator{Branches, Current.left(), Index + 1};
        } else if (Current.right().size() != 0) *this = tree_iterator{Branches, Current.right(), Index + 1};
        else if (Branches.size() != 0) *this = tree_iterator{Branches.rest(), Branches.first(), Index + 1};
        else *this = tree_iterator{Index + 1};
        return *this;
    }
    
    template <typename tree, typename value>
    inline tree_iterator<tree, value> tree_iterator<tree, value>::operator++(int) { // Postfix
        tree_iterator n = *this;
        operator++();
        return n;
    }
    
    template <typename tree, typename value>
    inline int tree_iterator<tree, value>::operator-(const tree_iterator& i) const {
        return static_cast<int>(Index) - i.Index;
    }
}

#endif
