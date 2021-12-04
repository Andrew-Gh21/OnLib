#pragma once
#include <cstdint>

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
}