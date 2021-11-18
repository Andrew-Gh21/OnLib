#pragma once
#include <cstdint>

namespace data
{
	enum class LogginErrors : uint8_t
	{
		InvalidUser = 1,
		InvalidPassword,
		UserAlreadyConnected
	};

	enum class RegisterErrors : uint8_t
	{
		UsernameAlreadyExists = 1,
		UserDoesNotMeetValidationCriteria,
		PasswordConfirmationDoesNotMatch,
		PasswordDoesNotMeetValidationCriteria
	};
}