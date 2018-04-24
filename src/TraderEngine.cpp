#include "TraderEngine.h"

using namespace std;

TraderEngine::TraderEngine() {

}

TraderEngine::~TraderEngine() {

}

void TraderEngine::print_user_not_registered(uint64_t user_id)
{
	cerr << "User with id: " << user_id << " is not registered" << endl;
}

void TraderEngine::on_user_connected(const user_connected& msg)
{
	uint64_t user_id = msg.user_id();

	{
		lock_guard<mutex> lock(_active_users_mutex);
		if (_active_users.count(user_id)) {
			_active_users.at(user_id)->set_connected(true);
			return;
		}
	}

	print_user_not_registered(user_id);
}

void TraderEngine::on_user_disconnected(const user_disconnected& msg)
{
	uint64_t user_id = msg.user_id();

	{
		lock_guard<mutex> lock(_active_users_mutex);
		if (_active_users.count(user_id)) {
			_active_users.at(user_id)->set_connected(false);
			return;
		}
	}

	print_user_not_registered(user_id);
}

void TraderEngine::on_user_deal_won(const user_deal_won& msg)
{
	uint64_t user_id = msg.user_id();

	shared_ptr<UserState> state;

	{
		lock_guard<mutex> lock(_active_users_mutex);
		if (_active_users.count(user_id)) {
			state = _active_users.at(user_id);
		}
	}

	if (state) {
		state->set_amount(msg.amount());
		state->set_time(msg.time());
		return;
	}

	print_user_not_registered(user_id);

}

void TraderEngine::on_user_renamed(const user_renamed& msg)
{
	uint64_t user_id = msg.user_id();

	shared_ptr<UserState> state;

	{
		lock_guard<mutex> lock(_active_users_mutex);
		if (_active_users.count(user_id)) {
			state = _active_users.at(user_id);
		}
	}

	if (state) {
		state->set_user_name(msg.name());
		return;
	}

	print_user_not_registered(user_id);
}

void TraderEngine::on_user_registered(const user_registered& msg)
{
	uint64_t user_id = msg.user_id();

	{
		lock_guard<mutex> lock(_active_users_mutex);

		if (!_active_users.count(user_id)) {

			auto state = make_shared<UserState>();

			state->set_user_id(user_id);

			state->set_user_name(msg.name());

			_active_users.insert({user_id, state});

			return;
		}
	}

	cerr << "User with id: " << user_id << " is already registered" << endl;
}
