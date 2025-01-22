#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <sol/state.hpp>

void InitialisePixelConstants(sol::state& lua);
void InitialiseSpriteConstants(sol::state& lua);
void InitialiseTextureConstants(sol::state& lua);

void InitialiseWindowState(sol::state& lua);

#endif
