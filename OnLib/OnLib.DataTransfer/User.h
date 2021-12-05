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
	};
};

namespace net
{
	template<>
	inline void Serialize(Message& message, const data::User& user)
	{
		Serialize(message, user.id);
		Serialize(message, std::cbegin(user.name), std::cend(user.name));
		Serialize(message, std::cbegin(user.password), std::cend(user.password));
	}

	template<>
	inline void Deserialize(Message& message, data::User& data)
	{
		Deserialize(message, data.password, true);
		Deserialize(message, data.name, true);
		Deserialize(message, data.id);
	}
}