#include "User.h"

void data::User::Serialize(net::Message& msg, User user)
{
	msg << user.id << user.name << user.password;
}

void data::User::Deserialize(net::Message& msg, User user)
{
	msg >> user.password >> user.name >> user.id;
}