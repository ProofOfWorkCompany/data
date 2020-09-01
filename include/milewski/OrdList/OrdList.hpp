#ifndef MILEWSKI_OKASAKI_ORDLIST
#define MILEWSKI_OKASAKI_ORDLIST

#include <cassert>
#include <memory>
#include <initializer_list>

namespace milewski::okasaki {
    template<class T, class Item>
    // requires Ord<T>
    struct OrdList
    {
        friend Item;
        explicit OrdList(std::shared_ptr<const Item> const & items) : _head(items) {}

        // Empty list
        OrdList() : _head{nullptr} {}
        // Cons
        OrdList(T v, OrdList const & tail) : _head(std::make_shared<Item>(v, tail._head))
        {
            assert(tail.isEmpty() || v <= tail.front());
        }
        bool isEmpty() const { return !_head; } // conversion to bool
        const T& front() const
        {
            assert(!isEmpty());
            return _head->_val;
        }
        OrdList popped_front() const
        {
            assert(!isEmpty());
            return OrdList(_head->_next);
        }
        // Additional utilities
        OrdList inserted(T v) const
        {
            if (isEmpty() || v <= front())
                return OrdList(v, OrdList(_head));
            else {
                return OrdList<T, Item>(front(), popped_front().inserted(v));
            }
        }
        // For debugging
        int headCount() const { return _head.use_count(); }

        std::shared_ptr<const Item> _head;
        
        bool operator==(const OrdList o) const { return _head == o._head; }
        
        bool operator!=(const OrdList o) const { return _head != o._head; }
    };


}

#endif
