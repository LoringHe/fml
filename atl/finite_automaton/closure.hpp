//
//  closure.hpp
//  atl 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_closure_hpp 
#define atl_finite_automaton_closure_hpp 

#include "../detail/automaton.hpp"
#include "../detail/finite_automaton/nondeterministic_finite_automaton.hpp"
#include "../../util/util.hpp"

namespace atl {
    enum Direction {forward, backward};
    struct reachable_closure_impl {
        template <FA_PARAMS>
        static void apply(const FA& fa,
                          typename FA::StateSet const& states_in,
                          typename FA::StateSet& states_out,
                          Direction direction = forward) {
            typedef typename FA::State State;
            typedef typename FA::StateSet StateSet;
            typedef typename FA::InTransitionIter InTransitionIter;
            typedef typename FA::OutTransitionIter OutTransitionIter;
            states_out.insert(states_in.begin(), states_in.end());
            StateSet work(states_in), new_work;
            while (work.size() > 0) {
                for (auto state : work) {
                    if (direction == forward) {
                        OutTransitionIter first, last;
                        tie(first, last) = out_transitions(fa, state);
                        for (; first != last; first++) {
                            State new_state = target(fa, *first);
                            if (states_out.insert(new_state).second) {
                                new_work.insert(new_state);
                            }
                        }
                    } else {
                        InTransitionIter first, last;
                        tie(first, last) = in_transitions(fa, state);
                        for (; first != last; first++) {
                            State new_state = source(fa, *first);
                            if (states_out.insert(new_state).second) {
                                new_work.insert(new_state);
                            }
                        }
                    }
                }
                work.clear();
                if (new_work.size() > 0) {
                    work.insert(new_work.begin(), new_work.end());
                    new_work.clear();
                }
            }
        }
    };

    template <FA_PARAMS>
    inline void
    reachable_closure(const FA& a,
                     typename FA::StateSet const& states_in,
                     typename FA::StateSet& states_out,
                     Direction direction = forward) {
        reachable_closure_impl::apply(a, states_in, states_out, direction);
    }

    template <FA_PARAMS>
    inline void
    reachable_closure(const FA& fa,
                      typename FA::StateSet& reachable_closure) {
            typename FA::StateSet forward_states({initial_state(fa)}),
                                               backward_states(final_state_set(fa));
            atl::reachable_closure(fa, forward_states, forward_states, forward);
            atl::reachable_closure(fa, backward_states, backward_states, backward);
            util::set_intersection(forward_states, backward_states, reachable_closure);
    }

    struct epsilon_closure_impl {
        template <NFA_PARAMS>
        static void apply(const NFA& nfa,
                          typename NFA::StateSet const& states_in,
                          typename NFA::StateSet& states_out) {
            typedef typename NFA::StateSet StateSet;
            states_out = states_in;
            if (!has_epsilon_transition(nfa)) return;
            StateSet work(states_in), new_work, targets;
            while (work.size()) {
                targets.clear();
                get_targets_in_map(nfa, work, nfa.epsilon(), targets);
                for (auto new_state : targets) {
                    if (states_out.insert(new_state).second) {
                        new_work.insert(new_state);
                    }
                }
                work.clear();
                if (new_work.size()) {
                    work.insert(new_work.begin(), new_work.end());
                    new_work.clear();
                }
            }
        }
    };

    template <NFA_PARAMS>
    inline void
    epsilon_closure(const NFA& fa,
                    typename NFA::StateSet const& states_in,
                    typename NFA::StateSet& states_out) {
        epsilon_closure_impl::apply(fa, states_in, states_out);
    }

    template <NFA_PARAMS>
    inline void
    epsilon_closure(const NFA& fa,
                    typename NFA::StateSet& closure) {
        closure.insert(fa.initial_state());
        epsilon_closure(fa, closure, closure);
    }
}

#endif /* closure_hpp */
