#pragma once
#include <string>
#include <Message.h>

namespace data
{
	struct User
	{
		uint64_t id;
		std::string name;
		std::string password;

		static void Serialize(net::Message& msg, User user);
		static void Deserialize(net::Message& msg, User user);
	};
};

