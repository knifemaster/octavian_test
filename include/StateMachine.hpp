#include <iostream>
#include <string>
#include <mutex>


namespace sm {


class StateMachine {
public:
    enum class State {
        IDLE,
        SHOW,
        SPIN
    };

    StateMachine() : current_state(State::IDLE) {}

    void set_state(State state) {
	std::lock_guard<std::mutex> lock(state_mutex);
        std::cout << "Transitioning from " << state_to_string(current_state)
                  << " to " << state_to_string(state) << std::endl;
        current_state = state;
    }

    State get_state() const {
        return current_state;
    }

    bool is_idle() const {
        return current_state == State::IDLE;
    }

    bool is_show() const {
        return current_state == State::SHOW;
    }

    bool is_spin() const {
        return current_state == State::SPIN;
    }

//    State current_state;

    std::string state_to_string(State state) const {
        switch (state) {
            case State::IDLE: return "Idle";
            case State::SHOW: return "Show";
            case State::SPIN: return "Spin";
            default: return "Unknown";
        }
    }
	
private:
	State current_state;
	std::mutex state_mutex;	
};


class StateButton {
public:
	enum class State { PRESSED, UNPRESSED };

	StateButton() : current_state(State::UNPRESSED) {}

	void set_state(State state) {
		std::lock_guard<std::mutex> lock(state_mutex);
		std::cout << "Transitioning from" << state_to_string(current_state) << std::endl;
		current_state = state;
	}

	State get_state() const {
		return current_state;
	}

	bool is_pressed() const {
		return current_state == State::PRESSED;
	}
	bool is_unpressed() const {
		return current_state == State::UNPRESSED;
	}

	std::string state_to_string(State state) const {
		switch (state) {
			case State::PRESSED: return "pressed";
			case State::UNPRESSED: return "unpressed";
			default : return "Unknown";
		}
	}
	
private:
	State current_state;
	std::mutex state_mutex;
};



}


