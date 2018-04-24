#include "TraderDataSubscriber.h"

#include <iostream>
#include <future>


using namespace std;

TraderDataSubscriber::TraderDataSubscriber():_sub_thread_active(false), _do_work(false) {

}

TraderDataSubscriber::~TraderDataSubscriber() {
	stop();
}

void TraderDataSubscriber::set_transport(std::unique_ptr<ITransport>&& transport)
{
	this->_transport.swap(transport);
}

void TraderDataSubscriber::set_endpoint(const string& endpoint)
{
	this->_endpoint = endpoint;
}

void TraderDataSubscriber::run_subscriber()
{
	ThreadActivityTrigger tat(_thread_state);

	while (_do_work) {
		if (!_transport->is_connected()) {
			cout << "Connection to " << _endpoint << " has been lost. Aborting..." << endl;
			break;
		}
		char buf[256];
		size_t size = 0;
		_transport->receive(buf, size);

		wrapper_message msg;
		msg.ParseFromArray(buf, size);

		on_message(msg);
//		async(launch::async, &TraderDataSubscriber::on_message, this, msg);
//		auto t = thread(&TraderDataSubscriber::on_message, this, msg);
//		t.detach();
	}
}

void TraderDataSubscriber::on_message(const wrapper_message& msg)
{
	if (msg.has_user_connected_msg()) {
		_engine.on_user_connected(msg.user_connected_msg());
	} else if (msg.has_user_deal_won_msg()) {
		_engine.on_user_deal_won(msg.user_deal_won_msg());
	} else if (msg.has_user_disconnected_msg()) {
		_engine.on_user_disconnected(msg.user_disconnected_msg());
	} else if (msg.has_user_registered_msg()) {
		_engine.on_user_registered(msg.user_registered_msg());
	} else if (msg.has_user_renamed_msg()) {
		_engine.on_user_renamed(msg.user_renamed_msg());
	}
}

void TraderDataSubscriber::start()
{
	try {
		_transport->connect(_endpoint);
	} catch (const exception& e) {
		cerr << "Unable to start receiving a trader data from the kernel. Error: " << e.what() << endl;
	}

	if (_transport->is_connected()) {
		_do_work = true;
		_sub_thread = thread(&TraderDataSubscriber::run_subscriber, this);
	}
}

void TraderDataSubscriber::stop()
{
	_do_work = false;

	if (_thread_state.is_active && _sub_thread.joinable()) {

		try {
			_sub_thread.join();
		} catch (const exception& e) {
			cerr << "Unable to join SUB thread. Error: " << e.what() << endl;
		}
	}

	if (_transport) {
		_transport->disconnect();
	}
}
