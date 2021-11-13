#include "User.h"

void data::User::Serialize(net::Message& msg)
{
	msg << id << name << password;
}

void data::User::Deserialize(net::Message& msg)
{
	msg >> password >> name >> id;
}