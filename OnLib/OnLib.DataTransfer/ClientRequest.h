#pragma once

#include <cstdint>
namespace data
{
	enum class ClientRequest : uint16_t
	{
		Login = 1,
		Register,
		RequestDisplayBooks,
		Logout,
		DeleteAccount,
		RequestLentBooks,
		RateBook,
		BorrowBook,
		ReturnBook
	};
}

