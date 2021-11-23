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

		User() : id(0), name(), password() {}
		User(uint64_t id, const std::string& name, const std::string& password) : id(id), name(name), password(password) {}

		static void Serialize(net::Message& msg, User user);
		static User Deserialize(net::Message& msg);
	};
};