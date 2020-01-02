//
//  automaton_utility.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_automaton_utility_hpp 
#define atl_automaton_utility_hpp

#include <iostream>
#include "detail/automaton.hpp"
#include "detail/no_type.hpp"
using std::cout;
using std::endl;
namespace atl {
    template <typename Automaton>
    inline void
    print_state(const Automaton& a,
                typename Automaton::State state) {
        cout << state;
        if constexpr (!std::is_same<typename Automaton::state_property_type, 
                                    boost::no_property>::value) {
            cout << " {" << atl::get_property(a, state) << "}";
        }
    }
    
    template <typename Automaton>
    inline void
    print_automaton(const Automaton& a) {
        typename Automaton::StateIter s_it, s_end;
        typename Automaton::OutTransitionIter t_it, t_end;
        for (tie(s_it, s_end) = states(a); s_it != s_end; s_it++) {
            tie(t_it, t_end) = out_transitions(a, *s_it);
            if (t_it == t_end) continue;
            cout << "State: ";
            print_state(a, *s_it);
            cout << endl;
            for ( ; t_it != t_end; t_it++) {
                cout << atl::get_property(a, *t_it) << " ";
                print_state(a, atl::target(a, *t_it));
                cout << endl;
            }
        }
    }

    template <typename Automaton>
    inline void
    print_fa(const Automaton& a, const string& name = "") {
        cout << "FA: " << name << endl;
        cout << "States: " << state_set(a).size() << endl;
        cout << "Alphabet: ";
        ID i = 0;
        for (auto& c : alphabet(a)) {
            if (++i == alphabet(a).size()) {
                cout << c << endl;
            } else {
                cout << c << " ";
            }
        }
        cout << "Initial: " << initial_state(a) << endl;
        cout << "Final: ";
        i = 0;
        for (auto s : final_state_set(a)) {
            if (++i == final_state_set(a).size()) {
                cout << s << endl;
            } else {
                cout << s << " ";
            }
        }
        cout << "--BODY--" <<endl;;
        print_automaton(a);
        cout << "--END--" << endl;
    }
}

#endif /* atl_automaton_utility_hpp */