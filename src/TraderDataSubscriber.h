#pragma once

#include <memory>
#include <thread>

#include "ITransport.h"
#include "ByteVector.h"
#include "ThreadActivityTrigger.h"
#include "TraderEngine.h"
#include "messages.pb.h"

class TraderEngine;
class TraderDataSubscriber {
private:
	std::unique_ptr<ITransport> _transport;
	std::thread _sub_thread;
	bool _sub_thread_active;
	bool _do_work;
	ThreadState _thread_state;
	std::string _endpoint;
	TraderEngine _engine;

	void on_message(const wrapper_message&);
public:
	TraderDataSubscriber();
	void set_transport(std::unique_ptr<ITransport>&&);
	void set_endpoint(const std::string&);
	void run_subscriber();
	void start();
	void stop();
	virtual ~TraderDataSubscriber();
};
