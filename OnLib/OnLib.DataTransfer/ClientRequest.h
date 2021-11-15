#pragma once

#include <cstdint>

enum class ClientRequest : uint16_t
{
	Login = 1,
	Register
};