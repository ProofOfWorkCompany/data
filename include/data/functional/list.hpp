// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST
#define DATA_LIST

#include <data/functional/stack.hpp>
#include <data/functional/queue.hpp>

namespace data::functional {
    
    template <typename Q, typename elem = decltype(std::declval<Q>().first())>
    concept list = stack<Q, elem> && queue<Q, elem>;
    
}

namespace data::meta {
    
    template <functional::queue list>
    list take_queue(list l, uint32 x) {
        if (x >= l.size()) return l;
        list n = l;
        list r{};
        while (x > 0) {
            r = data::append(r, n.first());
            n = n.rest();
            x--;
        }
        return r;
    }
    
    template <functional::stack list>
    list take_stack(list l, uint32 x) {
        if (x >= l.size()) return l;
        list n = l;
        list r{};
        while (x > 0) {
            r = data::prepend(r, n.first());
            n = n.rest();
            x--;
        }
        return data::reverse(r);
    }
    
    template <typename list>
    struct take {
        list operator()(const list& l, uint32 x) {
            return take_stack(l, x);
        }
    };
    
    template <functional::list list>
    struct take<list> {
        list operator()(const list& l, uint32 x) {
            return take_queue(l, x);
        }
    };
    
    template <functional::queue list>
    struct take<list> {
        list operator()(const list& l, uint32 x) {
            return take_queue(l, x);
        }
    };
    
    template <functional::queue L>
    L shuffle(const L x) {
        L q = x;
        L z{};
        while (!empty(q)) {
            q = rotate_right(q, std::uniform_int_distribution<int>(0, q.size() - 1)(get_random_engine()));
            z = z << q.first();
            q = q.rest();
        }
        return z;
    }
}

namespace data {
    
    template <functional::list L>
    inline L rotate_right(const L x) {
        size_t s = size(x);
        if (s == 0 || s == 1) return x; 
        
        return prepend(rest(x), first(x));
    }
    
    template <typename list> requires functional::queue<list> || functional::stack<list>
    list take(list l, uint32 x) {
        return meta::take<list>{}(l, x);
    }
}

#endif

