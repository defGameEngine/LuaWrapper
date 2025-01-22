#include "../Include/Utils.hpp"

#include <iostream>

void LuaPanic(std::optional<std::string> message)
{
	std::cerr << "[LUA] Panic:\n";

	if (message)
		std::cerr << message.value() << std::endl;
}
