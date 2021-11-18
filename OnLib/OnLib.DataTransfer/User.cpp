#include "User.h"

void data::User::Serialize(net::Message& msg, User user)
{
	msg << user.id << user.name << user.password;
}

data::User data::User::Deserialize(net::Message& msg)
{
	User u;
	msg >> u.password >> u.name >> u.id;

	return u;
}