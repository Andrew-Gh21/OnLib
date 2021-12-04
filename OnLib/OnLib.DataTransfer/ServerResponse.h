#pragma once

#include <cstdint>


namespace data
{
	enum class ServerResponse : uint16_t
	{
		ValidationSuccessfull = 1,
		SuccesfullLogin,
		InvalidLogin,
		SuccesfullRegister,
		InvalidRegister,
		DisplayBooksRecieved
	};
	
}