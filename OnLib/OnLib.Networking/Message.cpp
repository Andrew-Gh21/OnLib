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

	template<typename Data>
	Message& operator<<(Message& message, const Data& data)
	{
		static_assert(std::is_standard_layout<Data>::value,
			"Unable to serialize such complicated class. Please use standard layout classes.");

		auto messageInitialSize = message.body.size();
		message.body.resize(messageInitialSize + sizeof(Data));

		// Insert new data at the end of the vector by copying the bytes in the body
		std::memcpy(message.body.data() + messageInitialSize, &data, sizeof(Data));
		message.header.messageSize = message.GetSize();

		return message;
	}

	template<typename Data>
	Message& operator>>(Message& message, Data& data)
	{
		static_assert(std::is_standard_layout<Data>::value,
			"Unable to deserialize such complicated class. Please use standard layout classes.");

		auto dataBegin = message.body.size() - sizeof(Data);
		std::memcpy(&data, message.body.data() + dataBegin, sizeof(Data));
		message.body.resize(dataBegin);

		message.header.messageSize = message.GetSize();
		return message;
	}
}

