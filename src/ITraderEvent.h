#pragma once

#include <stdint.h>
#include <string>
#include <chrono>

typedef uint64_t user_id_t;
typedef std::string user_name_t;
typedef std::chrono::system_clock::time_point time_t;
typedef int64_t amount_t;

class ITraderEvent {
public:
	void virtual user_registered(user_id_t, const user_name_t&) const  = 0;
	void virtual user_renamed(user_id_t, const user_name_t) const  = 0;
	void virtual user_deal_won(user_id_t id, const time_t&, amount_t) const = 0;
	void virtual user_connected(user_id_t id) const = 0;
	void virtual user_disconnected(user_id_t id) const = 0;
	virtual ~ITraderEvent() {};
};
