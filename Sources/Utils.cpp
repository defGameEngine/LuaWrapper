/*-----------------------------------------------------------------
 *  Copyright 2026 defini7 and J-Starling. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "../Include/Utils.hpp"

#include <iostream>

void LuaPanic(std::optional<std::string> message)
{
	std::cerr << "[LUA] Panic:\n";

	if (message)
		std::cerr << message.value() << std::endl;
}
