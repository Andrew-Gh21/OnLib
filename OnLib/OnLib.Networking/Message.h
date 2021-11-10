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
}