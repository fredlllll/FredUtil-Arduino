typedef int(*stateFunction)();
typedef void(*stateEnterLeaveHandler)(int, int);

const int noState = -1;

template<int statecount> class fredStateMachine {
public:
	fredStateMachine() :_state(noState) {}

	void loop() {
		if (_state != noState) {
			auto stateFunc = stateFunctions[_state];
			if (stateFunc) {
				int newState = stateFunc();
				if (newState != noState) {
					auto leaveEnterFunc = stateLeaveHandlers[_state];
					if (leaveEnterFunc) { leaveFunc(); }
					_state = newState;
					leaveEnterFunc = stateEnterHandlers[_state];
					if (leaveEnterFunc) { leaveEnterFunc(); }
				}
			}
		}
	}

	int getState() {
		return _state;
	}

	void setState(int state) {
		_state = state;
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

private:
	stateEnterLeaveHandler stateEnterHandlers[statecount];
	stateEnterLeaveHandler stateLeaveHandlers[statecount];
	stateFunction stateFunctions[statecount];
	int _state;
}