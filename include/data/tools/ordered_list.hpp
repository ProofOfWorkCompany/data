// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_ORDERED_LIST
#define DATA_TOOLS_ORDERED_LIST

#include <concepts>
#include <milewski/OrdList/OrdList.hpp>
#include <data/tools/functional_queue.hpp>
#include <data/tools/linked_stack.hpp>
    
namespace data::tool {
    
    template <std::totally_ordered element> class ordered_list;
    
    template <typename element>
    struct ordered_list_item {
        ordered_list_item(element v, std::shared_ptr<const ordered_list_item> const & tail) : _val(v), _next(tail) {}
        element _val;
        std::shared_ptr<const ordered_list_item> _next;
        
        const element& first() const { return _val; }
        std::shared_ptr<const ordered_list_item> rest() const { return _next; }
    };
    
    template <typename element, typename item>
    struct ordered_list_iterator {
        ordered_list_iterator() : Ordered{} {}
        
        const element& operator*() const;
        
        ordered_list_iterator& operator++();
        ordered_list_iterator operator++(int);
        bool operator==(const ordered_list_iterator) const;
    private:
        milewski::okasaki::OrdList<element, item> Ordered;
        ordered_list_iterator(milewski::okasaki::OrdList<element, item> o) : Ordered{o} {}
        
        friend class ordered_list<element>;
    };
    
    template <std::totally_ordered element>
    class ordered_list {
        using item = ordered_list_item<element>;
        uint32 Size;
        using next = std::shared_ptr<const item>;
        milewski::okasaki::OrdList<element, item> Ordered;
        
    public:
        ordered_list();
        explicit ordered_list(const functional_queue<linked_stack<element>>& l);
        
        bool operator==(const ordered_list x) const;
        bool operator!=(const ordered_list x) const;
        
        bool empty() const;
        size_t size() const;
        
        bool valid() const;
        
        ordered_list insert(const element& x) const;
        ordered_list operator<<(const element& x) const;
        ordered_list operator<<(const linked_stack<element> l) const;
        
        ordered_list rest() const;
        const element& first() const;
        
        const element& operator[](uint32 n) const;
        
        const element& last() const;
        
        template <sequence seq>
        bool operator==(const seq& x) const;
        
        template <sequence seq>
        bool operator!=(const seq& x) const;
        
        using const_iterator = ordered_list_iterator<element, item>;
        
        const_iterator begin() const;
        const_iterator end() const;
        
    private:
        const element& last_private() const;
        
        ordered_list(uint32 size, milewski::okasaki::OrdList<element, item> ordered);
    };
    
}

namespace std {
    
    template <typename element, typename item> 
    struct iterator_traits<data::tool::ordered_list_iterator<element, item>> {
        using value_type = remove_const_t<element>;
        using difference_type = int;
        using pointer = const remove_reference_t<element>*;
        using reference = const element&;
        using iterator_concept = input_iterator_tag;
    };
}

namespace data::tool {
    
    template<typename element>
    ordered_list<element> merge(const ordered_list<element>& a, const ordered_list<element>& b) {
        if (a.empty()) return b;
        if (b.empty()) return a;
        if (a.first() <= b.first()) return merge(a.rest(), b) << a.first();
        else return merge(a, b.rest()) << b.first();
    }

    template <typename element>
    std::ostream& operator<<(std::ostream& o, const data::tool::ordered_list<element>& l) {
        o << "ordered_list{";
        if (!l.empty()) {
            data::tool::ordered_list<element> x = l;
            while(true) {
                o << x.first();
                x = x.rest();
                if (x.empty()) break;
                o << ", ";
            }
            return o << "}";
        }
    }
    
    template <typename element>
    bool ordered_list<element>::valid() const {
        if (empty()) return true;
        if (!data::valid(first())) return false;
        return rest().valid();
    }
    
    template <typename element>
    inline ordered_list<element>::ordered_list() : Size{0}, Ordered{} {}
        
    template <typename element>
    ordered_list<element>::ordered_list(const functional_queue<linked_stack<element>>& l) : ordered_list{} {
        functional_queue<linked_stack<element>> g = l;
        while(!data::empty(g)) {
            *this = *this << g.first();
            g = g.rest();
        }
    }
        
    template <typename element>
    template <sequence seq>
    bool ordered_list<element>::operator==(const seq& x) const {
        if (Size != x.size()) return false;
        if (Size == 0) return true;
        if (first() != x.first()) return false;
        return rest() == x.rest();
    }
    
    template <typename element>
    template <sequence seq>
    inline bool ordered_list<element>::operator!=(const seq& x) const {
        return !operator==(x);
    }
    
    template <typename element>
    inline bool ordered_list<element>::empty() const {
        return Size == 0;
    }
    
    template <typename element>
    inline size_t ordered_list<element>::size() const {
        return Size;
    }
    
    template <typename element>

    inline ordered_list<element> ordered_list<element>::insert(const element& x) const {
        return {Size + 1, Ordered.inserted(x)};
    }
    
    template <typename element>
    inline ordered_list<element> ordered_list<element>::operator<<(const element& x) const {
        return {Size + 1, Ordered.inserted(x)};
    }
    
    template <typename element>
    ordered_list<element> ordered_list<element>::operator<<(const linked_stack<element> l) const {
        if (l.empty()) return *this;
        return *this << l.first() << l.rest();
    }
    
    template <typename element>
    inline ordered_list<element> ordered_list<element>::rest() const {
        if (Size == 0) return {};
        return {Size - 1, Ordered.popped_front()};
    }
    
    template <typename element>
    inline const element& ordered_list<element>::first() const {
        return Ordered.front();
    }
    
    template <typename element>
    const element& ordered_list<element>::operator[](uint32 n) const {
        if (n >= Size) throw std::out_of_range{"ordered list"};
        if (n == 0) return first();
        return rest()[n - 1];
    }
    
    template <typename element>
    inline const element& ordered_list<element>::last() const {
        if (Size == 0) throw std::out_of_range{"ordered list"};
        return last_private();
    }
    
    template <typename element>
    inline const element& ordered_list<element>::last_private() const {
        if (Size == 1) return first();
        return rest().last_private();
    }
    
    template <typename element, typename item>
    inline const element& ordered_list_iterator<element, item>::operator*() const {
        return Ordered.front();
    }
    
    template <typename element, typename item>
    inline bool ordered_list_iterator<element, item>::operator==(const ordered_list_iterator i) const {
        return Ordered == i.Ordered;
    }
    
    template <typename element, typename item>
    inline ordered_list_iterator<element, item>& ordered_list_iterator<element, item>::operator++() {
        if (!Ordered.isEmpty()) Ordered = Ordered.popped_front();
        return *this;
    }
    
    template <typename element, typename item>
    inline ordered_list_iterator<element, item> ordered_list_iterator<element, item>::operator++(int) { // Postfix
        ordered_list_iterator n = *this;
        operator++();
        return n;
    }
    
    template <typename element>
    inline ordered_list<element>::ordered_list(uint32 size, milewski::okasaki::OrdList<element, item> ordered) : Size{size}, Ordered{ordered} {}
    
    template <typename element>
    inline ordered_list<element>::const_iterator ordered_list<element>::begin() const {
        return const_iterator{Ordered};
    }
    
    template <typename element>
    inline ordered_list<element>::const_iterator ordered_list<element>::end() const {
        return const_iterator{};
    }
}

#endif

