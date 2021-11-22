#pragma once
#include <cstdint>
#include <vector>
#include <ostream>

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

		template<typename VectorData>
		friend Message& operator << (Message& message, const std::vector<VectorData>& data);
		
		template<typename Data>
		/// <summary>
		/// Extract data from message(FIFO)
		/// </summary>
		/// <typeparam name="Data">Data type of the extracted data</typeparam>
		/// <param name="message">Current message</param>
		/// <param name="data">Extraction destination</param>
		/// <returns>The message without the extracted data</returns>
		friend Message& operator >> (Message& message, Data& data);

		template<typename VectorData>
		friend Message& operator >> (Message& message, std::vector<VectorData>& data);
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

	template<typename VectorData>
	Message& operator<<(Message& message, const std::vector<VectorData>& data)
	{
		auto vectorByteSize = data.size() * sizeof(VectorData);
		auto messageInitialSize = message.body.size();

		message.body.resize(messageInitialSize + vectorByteSize);

		std::memcpy(message.body.data() + messageInitialSize, data.data(), vectorByteSize);
		message.header.messageSize = message.GetSize();

		message << data.size();

		return message;
	}

	template<typename VectorData>
	Message& operator>>(Message& message, std::vector<VectorData>& data)
	{
		std::size_t vectSize;
		message >> vectSize;

		auto vectorByteSize = vectSize * sizeof(VectorData);

		VectorData* buffer = new VectorData[vectSize];

		auto dataBegin = message.body.size() - vectorByteSize;
		std::memcpy(buffer, message.body.data() + dataBegin, vectorByteSize);
		message.body.resize(dataBegin);

		message.header.messageSize = message.GetSize();

		std::copy(buffer, buffer + vectSize, std::back_inserter(data));
		//data = std::vector<VectorData>(buffer, vectSize);
		delete[] buffer;

		return message;
	}

	template<>
	Message& operator<<(Message& message, const std::string& data);

	template<>
	Message& operator>>(Message& message, std::string& data);
}

