#pragma once
#include <cstdint>
#include <vector>
#include <ostream>
#include <type_traits>

namespace net
{
	/// <summary>
	/// Struct representing the header of a sent message.
	/// </summary>
	struct MessageHeader
	{
		int32_t messageType;
		uint32_t messageSize;
	};

	/// <summary>
	/// Encapsulates data sent between clients and the server
	/// </summary>
	struct Message
	{
		MessageHeader header{};
		std::vector<uint8_t> body;
	
		std::size_t GetSize() const noexcept;
		friend std::ostream& operator << (std::ostream& out, const Message& message);

		template<typename Data>
		/// <summary>
		/// Insert data in message (FIFO)
		/// </summary>
		/// <typeparam name="Data">Inserted data type</typeparam>
		/// <param name="message">Current message(chaining)</param>
		/// <param name="data">Data to be inserted</param>
		/// <returns>Modified message</returns>
		friend Message& operator << (Message& message, const Data& data);

		template<typename Data>
		/// <summary>
		/// Extract data from message(FIFO)
		/// </summary>
		/// <typeparam name="Data">Data type of the extracted data</typeparam>
		/// <param name="message">Current message</param>
		/// <param name="data">Extraction destination</param>
		/// <returns>The message without the extracted data</returns>
		friend Message& operator >> (Message& message, Data& data);
	};

	class ClientConnection;

	struct OwnedMessage
	{
		std::shared_ptr<ClientConnection> remote;
		Message message;
	};

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

	template<typename T, bool = std::is_enum<T>::value>
	struct safe_underlying_type
	{
		using type = typename std::underlying_type<T>::type;
	};

	template<typename T>
	struct safe_underlying_type<T, false>
	{
		using type = T;
	};


	template<class T>
	void Serialize(Message& message, const T& data)
	{
		if (std::is_enum<T>::value)
		{
			using enumUType = typename safe_underlying_type<T>::type;
			enumUType value = static_cast<enumUType>(data);
			message << value;
			return;
		}
		message << data;
	}

	template<class T>
	void Deserialize(Message& message, T& data)
	{
		if (std::is_enum<T>::value)
		{
			using enumUType = typename safe_underlying_type<T>::type;
			enumUType value;
			message >> value;
			data = static_cast<T>(value);
			return;
		}
		message >> data;
	}

	template<typename SizeType = std::size_t, class InputIt>
	void Serialize(Message& message, InputIt begin, InputIt end)
	{
		SizeType size = std::distance(begin, end);

		for (InputIt it = begin; it != end; ++it)
		{
			Serialize(message, *it);
		}

		Serialize(message, size);
	}

	template<typename SizeType = std::size_t, typename Container>
	void Deserialize(Message& message, Container& container, bool keepOrder)
	{
		using value_type = typename Container::value_type;

		SizeType size;
		Deserialize(message, size);
		value_type* buffer = new value_type[size];

		for (SizeType index = 0; index < size; index++)
		{
			SizeType insertIndex = keepOrder ? size - index - 1 : index;
			Deserialize(message, buffer[insertIndex]);
		}

		std::copy(buffer, buffer + size, std::back_inserter(container));
		delete[] buffer;
	}
}

