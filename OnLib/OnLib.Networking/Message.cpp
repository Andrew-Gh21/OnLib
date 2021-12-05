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
}