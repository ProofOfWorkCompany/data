#ifndef DATA_QUEUE_HPP
#define DATA_QUEUE_HPP

#include <data/list.hpp>

namespace data {
        
    namespace queue {

        namespace definition {
                
            template <typename Q, typename X>
            struct queue : public list::definition::list<Q, X>, public list::definition::extendable<Q, X> {                
                Q append(const Q l, const X x) const {
                    return l + x;
                }
            };
                
            template <typename Q, typename X>
            struct queue<Q*, X> : public list::definition::list<Q*, X>, public list::definition::extendable<Q, X> {
                Q append(const Q l, const X x) const {
                    if (l == nullptr) return x;
                    return l->append(x);
                }
            };
                
            template <typename Q, typename X>
            struct queue<ptr<Q>, X> : public list::definition::list<ptr<Q>, X>, public queue<Q*, X> {};
            
        }
            
        template <typename Q>
        struct is_queue {
            using Element = std::remove_reference<std::__invoke_result<typename Q::first>>;
            constexpr static definition::queue<Q, Element> IsQueue{};
        };
        
        template <typename Q>
        struct is_queue<Q*> {
            using Element = std::remove_reference<std::__invoke_result<typename std::remove_pointer<Q>::first>>;
            constexpr static definition::queue<Q, Element> IsQueue{};
        };
        
        template <typename Q>
        struct is_queue<ptr<Q>> : public is_queue<Q*> {
            using Element = typename is_queue<Q*>::Element;
        };
            
        template <typename Q, typename X> 
        inline Q append(Q q, X x) {
            return definition::queue<Q, X>{}.append(q, x);
        }
            
        template <typename Q> 
        inline bool empty(Q q) {
            return list::empty(q);
        }
            
        template <typename Q> 
        inline bool size(Q q) {
            return list::size(q);
        }
            
        template <typename Q, typename X> 
        inline X& first(Q q) {
            return list::first(q);
        }
            
        template <typename Q, typename X> 
        inline Q rest(Q q) {
            return list::rest(q);
        }
        
        template <typename Q, typename X> 
        inline Q prepend(Q q, X value) {
            return list::rest(q, value);
        }
    
    }

}

#endif

