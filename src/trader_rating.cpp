#include <iostream>
#include <zmq.h>
#include <zmq_utils.h>
#include <chrono>
#include <thread>
#include <memory>

#include "TraderDataSubscriber.h"
#include "ZMQTransport.h"
#include "messages.pb.h"

using namespace std;

const char* endpoint = "tcp://127.0.0.1:8888";

size_t preprocess_footer(const ::google::protobuf::Message& msg, char* buf)
{
	size_t size = msg.ByteSize();
	msg.SerializeToArray(buf, size);

	return size;
}

size_t preprocess_user_registered(uint64_t id, const string& name, char* buf)
{
	wrapper_message parent_msg;

	user_registered* msg = new user_registered();
	msg->set_user_id(id);
	msg->set_name(name);

	parent_msg.set_allocated_user_registered_msg(msg);

	return preprocess_footer(parent_msg, buf);
}

size_t preprocess_user_renamed(uint64_t id, const string& name, char* buf)
{
	wrapper_message parent_msg;

	user_renamed* msg = new user_renamed();
	msg->set_user_id(id);
	msg->set_name(name);

	parent_msg.set_allocated_user_renamed_msg(msg);

	return preprocess_footer(parent_msg, buf);
}

size_t preprocess_user_deal_won(uint64_t id, const string& time, int64_t amount_t, char* buf)
{
	wrapper_message parent_msg;

	user_deal_won* msg = new user_deal_won();
	msg->set_user_id(id);
	msg->set_time(time);
	msg->set_amount(amount_t);

	parent_msg.set_allocated_user_deal_won_msg(msg);

	return preprocess_footer(parent_msg, buf);
}

size_t preprocess_user_connected(uint64_t id, char* buf)
{
	wrapper_message parent_msg;

	user_connected* msg = new user_connected();
	msg->set_user_id(id);

	parent_msg.set_allocated_user_connected_msg(msg);

	return preprocess_footer(parent_msg, buf);
}

size_t preprocess_user_disconnected(uint64_t id, char* buf)
{
	wrapper_message parent_msg;

	user_disconnected* msg = new user_disconnected();
	msg->set_user_id(id);

	parent_msg.set_allocated_user_disconnected_msg(msg);

	return preprocess_footer(parent_msg, buf);
}

void pub()
{
	auto pub = ZMQTransport();

	pub.configure("0MQ_SOCKET_TYPE", "ZMQ_PUB");

	pub.connect(endpoint);

	while(1) {
		char data[256];

		size_t size = preprocess_user_registered(123, "petya", data);

		pub.send(data, size);

		size = preprocess_user_connected(123, data);

		pub.send(data, size);

		size = preprocess_user_renamed(123, "vasya" ,data);

		pub.send(data, size);

		time_t t = chrono::system_clock::to_time_t(chrono::system_clock::now());
		size = preprocess_user_deal_won(123, ctime(&t) , 100500, data);

		pub.send(data, size);

		size = preprocess_user_connected(123, data);

		pub.send(data, size);

		this_thread::sleep_for(chrono::seconds(1));
	}
}

int main()
{
	try {
		TraderDataSubscriber data_sub;

		auto sub = unique_ptr<ZMQTransport>(new ZMQTransport());

		sub->configure("0MQ_SOCKET_TYPE", "ZMQ_SUB");

		data_sub.set_transport(move(sub));
		data_sub.set_endpoint(endpoint);
		data_sub.start();

		auto t = thread(pub);
		t.join();
	} catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
	} catch (...) {
		cerr << "Unknown exception" << endl;
	}

    return 0;
}

