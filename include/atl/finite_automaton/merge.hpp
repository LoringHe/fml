//
//  merge.hpp
//  atl 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_merge_hpp 
#define atl_finite_automaton_merge_hpp 

namespace atl {
    template <class Prop>
    struct intersect_merge {
        Prop operator() (const Prop& lhs, const Prop& rhs) const {
            return (lhs & rhs);
        }

        template<class InputIterator>
        Prop operator() (InputIterator first, InputIterator last) const {
            Prop res = *first;
            while (++first != last) {
                res = res & (*first);
            }
            return res;
        }
    };

    template <class Prop>
    struct union_merge {
        Prop operator() (const Prop& lhs, const Prop& rhs) const {
            return (lhs | rhs);
        }

        template<class InputIterator>
        Prop operator() (InputIterator first, InputIterator last) const {
            Prop res = *first;
            while (++first != last) {
                res = res | (*first);
            }
            return res;
        }
    };
}

#endif /* atl_finite_automaton_merge_hpp */
