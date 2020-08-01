//
//  nondeterministic_finite_automaton.hpp
//  atl 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_nondeterministic_finite_automaton_hpp 
#define atl_nondeterministic_finite_automaton_hpp

#include "../detail/finite_automaton/nondeterministic_finite_automaton.hpp"
#include "operate.hpp"
#include "cast.hpp"

namespace atl {
    template <class Symbol = char, 
              long epsilon_ = 0,
              class SymbolProperty = no_type,
              class StateProperty = no_type, 
              class AutomatonProperty = no_type>
    class nondeterministic_finite_automaton
        : public detail::nondeterministic_finite_automaton_gen<Symbol, epsilon_,
                                                               SymbolProperty,
                                                               StateProperty,
                                                               AutomatonProperty> {
        public:
            typedef Symbol symbol_type;
            typedef detail::nondeterministic_finite_automaton_gen<Symbol, epsilon_,
                                                                  SymbolProperty,
                                                                  StateProperty,
                                                                  AutomatonProperty> Base;
            typedef detail::finite_automaton_gen<Symbol, epsilon_,
                                                 SymbolProperty,
                                                 StateProperty,
                                                 AutomatonProperty> fa_type;
            typedef Base nfa_type;
            typedef detail::deterministic_finite_automaton_gen<Symbol, epsilon_,
                                                               SymbolProperty,
                                                               StateProperty,
                                                               AutomatonProperty> dfa_type;

            typedef typename Base::transition_property_type transition_property_type;
            typedef typename Base::automaton_property_type automaton_property_type;
            typedef typename Base::state_property_type state_property_type;
            typedef typename Base::Symbol2StateSetMap Symbol2StateSetMap;
            typedef typename Base::TransitionMap TransitionMap;
            typedef typename Base::StateSetMap StateSetMap;
            typedef typename Base::Transition Transition;
            typedef typename Base::State State;
            typedef typename Base::SymbolSet SymbolSet;

        public:
            nondeterministic_finite_automaton()
                : Base() {}

            nondeterministic_finite_automaton(const SymbolSet& alphabet)
                : Base(alphabet) {}

            nondeterministic_finite_automaton(const std::initializer_list<Symbol>& alphabet)
                : Base(alphabet) {}

            nondeterministic_finite_automaton(const nondeterministic_finite_automaton& x)
                : Base(x) {}

            ~nondeterministic_finite_automaton() {}

            nondeterministic_finite_automaton& 
            operator=(const nondeterministic_finite_automaton& x) {
                if (&x != this) {
                    Base::operator=(x);
                }
                return *this;
            }

            dfa_type
            operator&(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator&(const nondeterministic_finite_automaton& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                intersect_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator|(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator|(const nondeterministic_finite_automaton& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                union_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator-(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }

            dfa_type
            operator-(const nondeterministic_finite_automaton& x) {
                dfa_type out, dfa_lhs, dfa_rhs, dfa;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                complement_fa(dfa_rhs, dfa);
                intersect_fa(dfa_lhs, dfa, out);
                return out;
            }

            dfa_type
            operator+(const dfa_type& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator+(const nondeterministic_finite_automaton& x) {
                dfa_type out, dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                concat_fa(dfa_lhs, dfa_rhs, out);
                return out;
            }

            dfa_type
            operator!() {
                dfa_type dfa, out;
                minimize(*this, dfa);
                complement_fa(dfa, out);
                return out;
            }

            bool
            operator==(const dfa_type& x) {
                dfa_type dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }

            bool
            operator==(const nondeterministic_finite_automaton& x) {
                dfa_type dfa_lhs, dfa_rhs;
                minimize(*this, dfa_lhs);
                minimize(x, dfa_rhs);
                return equal_fa(dfa_lhs, dfa_rhs);
            }

        private:
        };
}

#endif /* atl_nondeterministic_finite_automaton_hpp */
