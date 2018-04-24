#pragma once

#include <string>

class ITransport {
public:
	virtual void connect(const std::string&) = 0;
	virtual bool is_connected() const = 0;
	virtual void disconnect() = 0;
	virtual void send(char*, size_t) = 0;
	virtual void configure(const std::string&, const std::string&) = 0;
	virtual void receive(char*, size_t&) = 0;
	virtual ~ITransport() {};
};
