#ifndef FREDSTATEMACHINE_H
#define FREDSTATEMACHINE_H

typedef int (*stateFunction)();
typedef void (*stateEnterLeaveHandler)(int);

const int noState = -1;

template<int statecount> class fredStateMachine {
private:
    stateEnterLeaveHandler stateEnterHandlers[statecount];
    stateEnterLeaveHandler stateLeaveHandlers[statecount];
    stateFunction stateFunctions[statecount];
    int _state;
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

    void setState(int state) {
        _state = state;
    }

    void changeState(int newState) {
        if (newState != noState) {
            if (_state != noState) {
                stateEnterLeaveHandler leaveEnterFunc = stateLeaveHandlers[_state];
                if (leaveEnterFunc) {
                    leaveEnterFunc(newState);
                }
            }
            int oldState = _state;
            _state = newState;
            if (_state != noState) {
                stateEnterLeaveHandler leaveEnterFunc = stateEnterHandlers[_state];
                if (leaveEnterFunc) {
                    leaveEnterFunc(oldState);
                }
            }
        }
    }

    void setStateFunction(int state, stateFunction function) {
        stateFunctions[state] = function;
    }

    void setStateEnterHandler(int state, stateEnterLeaveHandler handler) {
        stateEnterHandlers[state] = handler;
    }

    void setStateLeaveHandler(int state, stateEnterLeaveHandler handler) {
        stateLeaveHandlers[state] = handler;
    }
};
#endif
