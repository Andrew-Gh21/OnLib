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

		void Serialize(net::Message& msg);

		void Deserialize(net::Message& msg);
	};
}

