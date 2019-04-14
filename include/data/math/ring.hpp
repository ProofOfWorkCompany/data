#ifndef DATA_MATH_RING
#define DATA_MATH_RING

#include "group.hpp"

namespace data {
    
    namespace math {
            
        template <typename x>
        struct ring : public group::abelian<x> {
            x one() const {
                return 1;
            }
                
            x times(x a, x b) const {
                return a * b;
            }
        };
    
    }
    
}

#endif 
