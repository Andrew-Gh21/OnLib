#pragma once
#include <cstdint>
#include <type_traits>

namespace data
{
	enum class LoginErrors : uint8_t
	{
		InvalidUser = 1,
		InvalidPassword,
		UserAlreadyConnected
	};

	enum class RegisterErrors : uint8_t
	{
		UsernameAlreadyExists = 1,
		InvalidUsername,
		InvalidPassword,
		PassConfirmMismatch,
		EmptyField
	};

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
		ReturnBook,
		SearchBooks
	};

	enum class ServerResponse : uint16_t
	{
		ValidationSuccessfull = 1,
		SuccesfullLogin,
		InvalidLogin,
		SuccesfullRegister,
		InvalidRegister,
		DisplayBooksRecieved,
		DeleteAccountSuccesful,
		DeleteAccountFailure,
		BorrowedBooksRecieved,
		SearchedBooksRecieved
	};

	enum class BookCategory : uint64_t
	{
		SF = 1,
		Romance,
		Action,
		Comedy
	};

	template <typename EnumType>
	typename std::underlying_type<EnumType>::type EnumToNumber(EnumType value)
	{
		return static_cast<typename std::underlying_type<EnumType>::type>(value);
	}
}