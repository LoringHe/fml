//
//  push_down_system.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_push_down_system_hpp 
#define atl_detail_push_down_system_hpp

#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../../../util/util.hpp"
#include "../automaton.hpp"
#include "../no_type.hpp"

using boost::unordered_map;
using boost::unordered_set;

namespace atl {
    namespace detail {
        template <class Symbol>
        struct PDSLabel {
            Symbol symbol;
            std::vector<Symbol> symbols;
        };

        template <class Symbol, 
                  long epsilon_,
                  class SymbolProperty,
                  class StateProperty, 
                  class AutomatonProperty>
        class push_down_system_gen 
            : public automaton_gen<
                     typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                   PDSLabel<Symbol>, 
                                   Property<PDSLabel<Symbol>, 
                                                      SymbolProperty> >::type,
                     typename std::conditional<std::is_same<StateProperty, no_type>::value,
                                   boost::no_property, StateProperty>::type,
                     typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                                   boost::no_property, AutomatonProperty>::type> {
        public:
            typedef Property<PDSLabel<Symbol>, SymbolProperty> transition_property;
            typedef PDSLabel<Symbol> pds_label_type;
            typedef Symbol symbol_type;
            typedef SymbolProperty symbol_property_type;
            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  PDSLabel<Symbol>, 
                                  Property<PDSLabel<Symbol>, SymbolProperty> >::type
                transition_property_type;
            typedef typename std::conditional<std::is_same<StateProperty, no_type>::value,
                                   boost::no_property, StateProperty>::type
                state_property_type;
            typedef typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                                   boost::no_property, AutomatonProperty>::type
                automaton_property_type;
            typedef automaton_gen<transition_property_type, 
                                  state_property_type, 
                                  automaton_property_type> Base;

            typedef typename Base::Transition Transition;
            typedef typename Base::State State;

            typedef unordered_set<State> StateSet;
            typedef unordered_set<Symbol> SymbolSet;

            typedef unordered_map<State, State> State2Map;

        public:
            push_down_system_gen(const SymbolSet alphabet = SymbolSet())
                : Base(),
                  alphabet_(alphabet) {}

            push_down_system_gen(const push_down_system_gen& x)
                : Base(x),
                  alphabet_(x.alphabet_),
                  state_set_(x.state_set_),
                  control_state_set_(x.control_state_set_) {}

            push_down_system_gen& 
            operator=(const push_down_system_gen& x) {
                if (&x != this) {
                    Base::operator=(x);
                    alphabet_ = x.alphabet_;
                    state_set_ = x.state_set_;
                    control_state_set_ = x.control_state_set_;
                }
                return *this;
            }

            virtual void clear() {
                Base::clear();
                state_set_.clear();
                control_state_set_.clear();
                alphabet_.clear();
            }

            virtual State 
            add_state(const state_property_type& p) {
                State state = Base::add_state(p);
                state_set_.insert(state);
                return state;
            }

            virtual State 
            add_state() {
                State state = Base::add_state();
                state_set_.insert(state);
                return state;
            }

            transition_property_type 
            epsilon() const {
                return transition_property_type(epsilon_);
            }

            const StateSet&
            state_set() const {
                return state_set_;
            }

            void
            set_state_set(const StateSet& state_set) {
                state_set_ = state_set;
            }

            const StateSet&
            control_state_set() const {
                return control_state_set_;
            }

            void
            set_control_state_set(const StateSet& control_state_set) {
                control_state_set_ = control_state_set;
            }

            void
            clear_controle_state_set() {
                control_state_set_.clear();
            }

            const SymbolSet&
            alphabet() const {
                return alphabet_;
            }

            void
            set_alphabet(const SymbolSet& alphabet) {
                alphabet_ = alphabet;
            }

            void 
            set_alphabet(const Symbol& c) {
                alphabet_.insert(c);
            }

            void 
            set_state(State state) {
                state_set_.insert(state);
            }

            void 
            set_control_state(State state) {
                control_state_set_.insert(state);
            }

            void 
            remove_control_state(State state) {
                control_state_set_.erase(state);
            }
            
            pair<Transition, bool>
            add_transition(State s, State t,
                           const Symbol& c,
                           const std::vector<Symbol>& cs,
                           const SymbolProperty& p) {
                auto pds_label = PDSLabel<Symbol>(c, cs);
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    return Base::add_transition(s, t, pds_label);
                } else {
                    return Base::add_transition(s, t, transition_property(pds_label, p));
                }
            }

            pair<Transition, bool>
            add_transition(State s, State t,
                           const PDSLabel<Symbol>& l,
                           const SymbolProperty& p) {
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    return Base::add_transition(s, t, l);
                } else {
                    return Base::add_transition(s, t, transition_property(l, p));
                }
            }

        private:
            StateSet control_state_set_;
            StateSet state_set_;
            SymbolSet alphabet_;
        };
    }

    #define PDS_PARAMS typename SYMBOL, long EPSILON, typename SYP, typename STP, typename AP
    #define PDS detail::push_down_system_gen<SYMBOL, EPSILON, SYP, STP, AP>

    template <PDS_PARAMS>
    inline typename PDS::SymbolSet const&
    alphabet(const PDS& pds) {
        return pds.alphabet();
    }

    template <PDS_PARAMS>
    inline void
    set_alphabet(PDS& pds,
                 typename PDS::SymbolSet const& set) {
        pds.set_alphabet(set);
    }

    template <PDS_PARAMS>
    inline typename PDS::StateSet const&
    state_set(PDS& pds) {
        return pds.state_set();
    }

    template <PDS_PARAMS>
    inline void  
    set_state_set(PDS& pds, 
                  typename PDS::StateSet const& set) {
        pds.set_state_set(set);
    }

    template <PDS_PARAMS>
    inline void  
    set_state(PDS& pds, 
              typename PDS::State state) {
        pds.set_state(state);
    }

    template <PDS_PARAMS>
    inline typename PDS::StateSet const&
    final_state_set(PDS& pds) {
        return pds.final_state_set();
    }

    template <PDS_PARAMS>
    inline void  
    set_final_state_set(PDS& pds, 
                        typename PDS::StateSet const& set) {
        pds.set_final_state_set(set);
    }

    template <PDS_PARAMS>
    inline void  
    set_final_state(PDS& pds, 
                    typename PDS::State state) {
        pds.set_final_state(state);
    }
    
    template <PDS_PARAMS>
    inline void  
    remove_final_state(PDS& pds, 
                       typename PDS::State state) {
        pds.remove_final_state(state);
    }

    template <PDS_PARAMS>
    inline void  
    clear_finale_state_set(PDS& pds) {
        pds.clear_finale_state_set();
    }

    template <PDS_PARAMS>
    inline typename PDS::StateSet const&
    control_state_set(PDS& pds) {
        return pds.control_state_set();
    }

    template <PDS_PARAMS>
    inline void  
    set_control_state_set(PDS& pds, 
                          typename PDS::StateSet const& set) {
        pds.set_control_state_set(set);
    }

    template <PDS_PARAMS>
    inline void  
    set_control_state(PDS& pds, 
                      typename PDS::State state) {
        pds.set_control_state(state);
    }
    
    template <PDS_PARAMS>
    inline void  
    remove_control_state(PDS& pds, 
                         typename PDS::State state) {
        pds.remove_control_state(state);
    }

    template <PDS_PARAMS>
    inline void  
    clear_controle_state_set(PDS& pds) {
        pds.clear_controle_state_set();
    }

    template <PDS_PARAMS>
    inline typename PDS::TransitionMap const&
    transition_map(const PDS& pds) {
        return pds.transition_map();
    }

    template <PDS_PARAMS>
    inline typename PDS::transition_property_type
    epsilon(const PDS& pds) {
        return pds.epsilon();
    }

    template <PDS_PARAMS>
    inline typename PDS::State
    add_final_state(PDS& pds,
                    typename PDS::state_property_type const& p) {
        typename PDS::State s = add_state(pds, p);
        pds.set_final_state(s);
        return s;
    }

    template <PDS_PARAMS>
    inline typename PDS::State
    add_final_state(PDS& pds) {
        typename PDS::State s = add_state(pds);
        pds.set_final_state(s);
        return s;
    }

    template <PDS_PARAMS>
    inline typename PDS::State
    add_control_state(PDS& pds,
                      typename PDS::state_property_type const& p) {
        typename PDS::State s = add_state(pds, p);
        pds.set_control_state(s);
        return s;
    }

    template <PDS_PARAMS>
    inline typename PDS::State
    add_control_state(PDS& pds) {
        typename PDS::State s = add_state(pds);
        pds.set_control_state(s);
        return s;
    }

    template <PDS_PARAMS>
    inline pair<typename PDS::Transition, bool>
    add_transition(PDS& pds,
                   typename PDS::State s,
                   typename PDS::State t,
                   typename PDS::symbol_type c,
                   const std::vector<typename PDS::symbol_type>& cs,
                   typename PDS::symbol_property_type p = typename PDS::symbol_property_type()) {
        return pds.add_transition(s, t, c, cs, p);
    }
    
    template <PDS_PARAMS>
    inline pair<typename PDS::Transition, bool>
    add_transition(PDS& pds,
                   typename PDS::State s,
                   typename PDS::State t,
                   typename PDS::pds_label_type l,
                   typename PDS::symbol_property_type p) {
        return pds.add_transition(s, t, l, p);
    }
}

#endif /* atl_detail_push_down_system_hpp */