#pragma once


struct ThreadState {
	bool is_active;
};

class ThreadActivityTrigger {
private:
	ThreadState _ts;

	ThreadActivityTrigger() = delete;

public:

	ThreadActivityTrigger(ThreadState& ts) {
		_ts = ts;
	}

	~ThreadActivityTrigger(){
		_ts.is_active = false;
	}
};
