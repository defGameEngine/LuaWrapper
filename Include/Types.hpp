#ifndef TYPES_HPP
#define TYPES_HPP

#include <sol/state.hpp>

template <class T>
void InitialiseVec2dType(sol::state& lua, const std::string_view name)
{
	lua.new_usertype<T>(name,
		sol::constructors<T(), T(decltype(T::x), decltype(T::y))>(),
		"x", sol::property(&T::x, &T::x),
		"y", sol::property(&T::y, &T::y),

		"Clamp", &T::Clamp,
		"Lerp", &T::Lerp,
		"Distance", &T::Distance,
		"DotProduct", &T::DotProduct,
		"CrossProduct", &T::CrossProduct,
		"Angle", &T::Angle,
		"Length", &T::Length,
		"Length2", &T::Length2,
		"ManhattanDistance", &T::ManhattanDistance,
		"Max", &T::Max,
		"Min", &T::Min,
		"Swap", &T::Swap,
		"Norm", &T::Normalise,
		"Abs", &T::Abs,
		"Perpendicular", &T::Perpendicular,
		"Floor", &T::Floor,
		"Ceil", &T::Ceil,
		"Round", &T::Round,
		"Cartesian", &T::Cartesian,
		"Polar", &T::Polar,
		"ToString", &T::ToString
	);
}

void InitialiseLayerType(sol::state& lua);
void InitialisePixelType(sol::state& lua);
void InitialiseSpriteType(sol::state& lua);
void InitialiseTextureType(sol::state& lua);
void InitialiseGraphicType(sol::state& lua);
void InitialiseKeyboardConstants(sol::state& lua);
void InitialiseMouseConstants(sol::state& lua);

#endif
