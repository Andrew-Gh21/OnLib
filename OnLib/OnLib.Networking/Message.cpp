#include "Message.h"

namespace net
{
	std::size_t Message::GetSize() const noexcept
	{
		return sizeof(MessageHeader) + body.size();
	}

	std::ostream& operator<<(std::ostream& out, const Message& message)
	{
		out << "Header: { Type: " << message.header.messageType << ", Size: " << message.header.messageSize << "}";
		return out;
	}

	template<>
	Message& operator<<(Message& message, const std::string& data)
	{
		message << data.c_str();
		message << data.size();

		return message;
	}

	template<>
	Message& operator>>(Message& message, std::string& data)
	{
		std::size_t stringSize;
		message >> stringSize;

		char* characters = new char[stringSize];
		message >> characters;

		data = std::string(characters, characters + stringSize);

		return message;
	}
}

