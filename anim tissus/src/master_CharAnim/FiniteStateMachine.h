#ifndef __FINITESTATEMACHINE_H__
#define __FINITESTATEMACHINE_H__

#include <map>
#include <utility>
#include <functional>
#include <iostream>

enum ETATS
{
    Idle, 
    SlowWalk,
    Walk, 
    Run,
    Kick,
    Backflip, 
    Crouch, 
    CrouchWalk, 
    Jump
};

enum Event
{
    Z,
    S,
    FinKick, 
    V, 
    CtrlZ, 
    ZShift,
    ZAlt,  
    Ctrl,
    FinJump, 
    FinBackflip, 
    B, 
    Space, 
    I
};

class FiniteStateMachine{
public:
    using Action = std::function<void()>;

    FiniteStateMachine(){
        currentState = ETATS::Idle; 
    }

    void addTransition(ETATS from, ETATS to, Event event, Action action) {
        transitions[{from, event}] = {to, action};
    }

    void handleEvent(Event event) {
        previousState = currentState; 
        auto key = std::make_pair(currentState, event);
        // sinon pas de transition possible 
        if (transitions.find(key) != transitions.end()) {
            std::pair<ETATS, Action> transition = transitions[key];
            transition.second(); // Executer l'action associée
            currentState = transition.first;
        }
    }

    ETATS getCurrentState() const {
        return currentState;
    }
    ETATS getPreviousState() const {
        return previousState;
    }

private:

    std::map<std::pair<ETATS, Event>, std::pair<ETATS, Action>> transitions;
    ETATS currentState;
    ETATS previousState;
};
#endif