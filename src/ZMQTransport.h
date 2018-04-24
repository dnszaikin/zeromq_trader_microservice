#pragma once

#include <string>
#include <zmq.h>
#include <zmq_utils.h>

#include "ByteVector.h"
#include "ITransport.h"


class ZMQTransport : public ITransport
{
private:
	void* _context;
	void* _socket;
	int _socket_type;
	bool _is_connected;

public:
	ZMQTransport();

	virtual void connect(const std::string&);
	virtual void disconnect();
	virtual void send(char*, size_t);
	virtual void receive(char* buf, size_t& size);
	virtual void configure(const std::string&, const std::string&);
	virtual bool is_connected() const;
	virtual ~ZMQTransport();
};

