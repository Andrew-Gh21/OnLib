#include "Message.h"

namespace net
{
	std::size_t Message::GetSize() const noexcept
	{
		return body.size();
	}

	std::ostream& operator<<(std::ostream& out, const Message& message)
	{
		out << "Header: { Type: " << message.header.messageType << ", Size: " << message.header.messageSize << "}";
		return out;
	}

	template<>
	Message& operator<<(Message& message, const std::string& string)
	{
		// Push string data
		auto stringByteSize = string.size() * sizeof(char);
		auto messageInitialSize = message.body.size();

		message.body.resize(messageInitialSize + stringByteSize);

		std::memcpy(message.body.data() + messageInitialSize, string.data(), stringByteSize);
		message.header.messageSize = message.GetSize();

		// Push string size
		message << string.size();

		return message;
	}

	template<>
	Message& operator>>(Message& message, std::string& string)
	{
		// Pull string size
		std::size_t strSize;
		message >> strSize;

		// Pull string data
		auto stringByteSize = strSize * sizeof(char);

		char* charBuffer = new char[strSize];

		auto dataBegin = message.body.size() - stringByteSize;
		std::memcpy(charBuffer, message.body.data() + dataBegin, stringByteSize);
		message.body.resize(dataBegin);

		message.header.messageSize = message.GetSize();
		
		string = std::string(charBuffer, strSize);
		delete[] charBuffer;

		return message;
	}
}

