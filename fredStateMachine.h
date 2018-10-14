#ifndef FREDSTATEMACHINE_H
#define FREDSTATEMACHINE_H

//#define USEENTERFUNCTIONS
//#define USELEAVEFUNCTIONS

typedef int (*stateFunction)();
typedef void (*stateEnterLeaveHandler)(int);

const int noState = -1;

template<int statecount> class fredStateMachine {
private:
#ifdef USEENTERFUNCTIONS
    stateEnterLeaveHandler stateEnterHandlers[statecount];
#endif
#ifdef USELEAVEFUNCTIONS
    stateEnterLeaveHandler stateLeaveHandlers[statecount];
#endif
    stateFunction stateFunctions[statecount];
    volatile int _state;
public:
    fredStateMachine() :
            _state(noState) {
    }

    void loop() {
        if (_state != noState) {
            auto stateFunc = stateFunctions[_state];
            if (stateFunc) {
                int newState = stateFunc();
                changeState(newState);
            }
        }
    }

    int getState() {
        return _state;
    }

    //directly setting the state, no enter or leave functions are called
    void setState(int state) {
        _state = state;
    }

    //changing state (if newState is not noState). this will call enter and leave functions
    void changeState(int newState) {
        if (newState != noState) {
#ifdef USELEAVEFUNCTIONS
            if (_state != noState) {
                stateEnterLeaveHandler leaveFunc = stateLeaveHandlers[_state];
                if (leaveFunc) {
                    leaveFunc(newState);
                }
            }
#endif
#ifdef USEENTERFUNCTIONS
            int oldState = _state;
#endif
            _state = newState;
#ifdef USEENTERFUNCTIONS
            stateEnterLeaveHandler enterFunc = stateEnterHandlers[_state];
            if (enterFunc) {
                enterFunc(oldState);
            }
#endif
        }
    }

    void setStateFunction(int state, stateFunction function) {
        stateFunctions[state] = function;
    }

#ifdef USEENTERFUNCTIONS
    void setStateEnterHandler(int state, stateEnterLeaveHandler handler) {
        stateEnterHandlers[state] = handler;
    }
#endif

#ifdef USELEAVEFUNCTIONS
    void setStateLeaveHandler(int state, stateEnterLeaveHandler handler) {
        stateLeaveHandlers[state] = handler;
    }
#endif
};
#endif
