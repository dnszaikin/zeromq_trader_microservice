#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>
#include "ZMQTransport.h"


using namespace std;

ZMQTransport::ZMQTransport() :
		_socket(NULL),
		_socket_type(0),
		_is_connected(false)
{
	_context = zmq_ctx_new();
	if (!_context) {
		throw runtime_error("Unable to create 0MQ context");
	}
}

ZMQTransport::~ZMQTransport()
{
	disconnect();
	if (_context) {
		zmq_ctx_destroy(_context);
	}
}

void ZMQTransport::connect(const std::string& endpoint)
{
	_socket = zmq_socket(_context, _socket_type);

	if (!_socket) {
		throw runtime_error("Unable to create PUB socket. Error: " + string(strerror(errno)));
	}

	if (_socket_type == ZMQ_PUB) {
		int rc = zmq_bind(_socket, endpoint.c_str());

		if (rc == -1) {
			throw runtime_error("Unable to bind to " + endpoint + " Error: " + string(strerror(errno)));
		}

		cout << "Listening: " << endpoint << endl;

	} else if (_socket_type == ZMQ_SUB) {
		int rc = zmq_connect(_socket, endpoint.c_str());

		if (rc == -1) {
			throw runtime_error("Unable to connect to " + endpoint + " Error: " + string(strerror(errno)));
		}

		cout << "Connected to: " << endpoint << endl;

		zmq_setsockopt(_socket, ZMQ_SUBSCRIBE, "", 0);
	}

	_is_connected = true;
}

bool ZMQTransport::is_connected() const
{
	return _is_connected;
}

void ZMQTransport::disconnect()
{
	_is_connected = false;
	if (_socket) {
		zmq_close(_socket);
	}
}

void ZMQTransport::send(char* msg, size_t size)
{
	int bytes = zmq_send(_socket, msg, size, ZMQ_DONTWAIT);
	if (bytes != size) {
		cout << "Message [" << msg << "] sent only " << bytes << " of " << size << " bytes";
	}
}

void ZMQTransport::receive(char* buf, size_t& size)
{
	size= zmq_recv(_socket, buf, 256, 0);
}

void ZMQTransport::configure(const string & key, const string & value)
{
	if (key == "0MQ_SOCKET_TYPE") {
		if (value == "ZMQ_PUB") {
			_socket_type = ZMQ_PUB;
		} else if (value == "ZMQ_SUB") {
			_socket_type = ZMQ_SUB;
		} else {
			throw runtime_error("Unsupported socket type: " + value);
		}
	}
}
