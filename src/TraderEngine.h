#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>

#include "messages.pb.h"

class UserState {
private:
	mutable std::mutex _state_mutex;
	uint64_t _user_id;
	std::string _user_name;
	uint64_t _amount;
	std::string _time;
	bool _is_connected;

public:
	UserState():_user_id(0), _user_name(""), _amount(0), _time(""), _is_connected(false) {

	}
	virtual ~UserState(){

	}

	uint64_t get_user_id() const {
		std::lock_guard<std::mutex> lock(_state_mutex);

		return _user_id;
	}

	void set_user_id(uint64_t user_id) {
		std::lock_guard<std::mutex> lock(_state_mutex);

		_user_id = user_id;
	}

	uint64_t get_amount() const {
		std::lock_guard<std::mutex> lock(_state_mutex);

		return _amount;
	}

	void set_amount(uint64_t amount) {
		std::lock_guard<std::mutex> lock(_state_mutex);

		_amount += amount;
	}

	const std::string& get_user_name() const {
		std::lock_guard<std::mutex> lock(_state_mutex);

		return _user_name;
	}

	void set_user_name(const std::string& user_name) {
		std::lock_guard<std::mutex> lock(_state_mutex);

		_user_name = user_name;
	}

	const std::string& get_time() const {
		std::lock_guard<std::mutex> lock(_state_mutex);

		return _time;
	}

	void set_time(const std::string& time) {
		std::lock_guard<std::mutex> lock(_state_mutex);

		_time = time;
	}

	bool is_connected() {
		std::lock_guard<std::mutex> lock(_state_mutex);

		return _is_connected;
	}

	void set_connected(bool b) {
		std::lock_guard<std::mutex> lock(_state_mutex);

		_is_connected = b;
	}
};


class UserState;
typedef std::unordered_map<uint64_t, std::shared_ptr<UserState>> active_users_t;

class TraderEngine {
private:
	std::mutex _active_users_mutex;
	active_users_t _active_users;
	void print_user_not_registered(uint64_t);
public:
	TraderEngine();
	void on_user_connected(const user_connected&);
	void on_user_disconnected(const user_disconnected&);
	void on_user_deal_won(const user_deal_won&);
	void on_user_renamed(const user_renamed&);
	void on_user_registered(const user_registered&);
	virtual ~TraderEngine();
};
