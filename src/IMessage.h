#pragma once

class IMessage {
public:
	void set(void*);
	void* get() const;
	virtual ~IMessage() {};
};
