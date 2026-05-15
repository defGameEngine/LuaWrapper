/*-----------------------------------------------------------------
 *  Copyright 2026 defini7 and J-Starling. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <sol/state.hpp>

void InitialisePixelConstants(sol::state& lua);
void InitialiseSpriteConstants(sol::state& lua);
void InitialiseTextureConstants(sol::state& lua);

#endif
