#include "../Include/Types.hpp"
#include "defGameEngine.hpp"

using namespace def;

void InitialiseLayerType(sol::state& lua)
{
	lua.new_usertype<Layer>("Layer",
		sol::constructors<Layer()>(),
		"textures", sol::property(&Layer::textures),
		"pixels", sol::property(&Layer::pixels),
		"target", sol::property(&Layer::target),
		"offset", sol::property(&Layer::offset),
		"size", sol::property(&Layer::size),
		"textureStructure", sol::property(&Layer::textureStructure),
		"pixelMode", sol::property(&Layer::pixelMode),
		"visible", sol::property(&Layer::visible),
		"update", sol::property(&Layer::update),
		"tint", sol::property(&Layer::tint),
		"shader", sol::property(&Layer::shader)
	);
}

void InitialisePixelType(sol::state& lua)
{
	lua.new_usertype<Pixel>("Pixel",
		sol::constructors<Pixel(), Pixel(uint8_t, uint8_t, uint8_t, uint8_t)>(),
		"r", sol::property(&Pixel::r, &Pixel::r),
		"g", sol::property(&Pixel::g, &Pixel::g),
		"b", sol::property(&Pixel::b, &Pixel::b),
		"a", sol::property(&Pixel::a, &Pixel::a),

		"Lerp", &Pixel::Lerp,
		"ToString", &Pixel::ToString
	);

	lua["PixelFloat"] = &Pixel::Float;
}

void InitialiseSpriteType(sol::state& lua)
{
	lua.new_usertype<Sprite>("Sprite",
		sol::constructors<Sprite(), Sprite(const Vector2i&), Sprite(std::string_view)>(),
		"size", sol::property(&Sprite::size, &Sprite::size),
		"pixels", sol::property(&Sprite::pixels, &Sprite::pixels),

		"Create", &Sprite::Create,
		"Load", &Sprite::Load,
		"Save", &Sprite::Save,
		"SetPixel", sol::overload(
			static_cast<bool (Sprite::*)(int, int, const Pixel&)>(&Sprite::SetPixel),
			static_cast<bool (Sprite::*)(const Vector2i&, const Pixel&)>(&Sprite::SetPixel)
		),
		"GetPixel", sol::overload(
			static_cast<Pixel (Sprite::*)(int, int, Sprite::WrapMethod) const>(&Sprite::GetPixel),
			static_cast<Pixel (Sprite::*)(const Vector2i&, Sprite::WrapMethod) const>(&Sprite::GetPixel)
		),
		"SetPixelData", &Sprite::SetPixelData,
		"Sample", sol::overload(
			static_cast<Pixel (Sprite::*)(float, float, Sprite::SampleMethod, Sprite::WrapMethod) const>(&Sprite::Sample),
			static_cast<Pixel (Sprite::*)(const Vector2f&, Sprite::SampleMethod, Sprite::WrapMethod) const>(&Sprite::Sample)
		)
	);
}

void InitialiseTextureType(sol::state& lua)
{
	lua.new_usertype<Texture>("Texture",
		sol::constructors<Texture(Sprite*), Texture(std::string_view)>(),
		"id", sol::property(&Texture::id, &Texture::id),
		"uv_scale", sol::property(&Texture::uvScale, &Texture::uvScale),
		"size", sol::property(&Texture::size, &Texture::size),

		"Load", &Texture::Load,
		"Update", &Texture::Update
	);
}

void InitialiseGraphicType(sol::state& lua)
{
	auto graphicType = lua.new_usertype<Graphic>("Graphic",
		sol::constructors<Graphic(), Graphic(std::string_view), Graphic(const Vector2i&)>(),
		"texture", sol::property(&Graphic::texture, &Graphic::texture),
		"sprite", sol::property(&Graphic::sprite, &Graphic::sprite),

		"Load", sol::overload(
			static_cast<void (Graphic::*)(std::string_view)>(&Graphic::Load),
			static_cast<void (Graphic::*)(const Vector2i&)>(&Graphic::Load)
		),

		"Save", &Graphic::Save,
		"UpdateTexture", &Graphic::UpdateTexture
	);
}

void InitialiseKeyboardConstants(sol::state& lua)
{
	lua.new_enum("Key",
		"Space", Key::SPACE,
		"Apostrophe", Key::APOSTROPHE,
		"Comma", Key::COMMA,
		"Minus", Key::MINUS,
		"Period", Key::PERIOD,
		"Slash", Key::SLASH,

		"K0", Key::K0,
		"K1", Key::K1,
		"K2", Key::K2,
		"K3", Key::K3,
		"K4", Key::K4,
		"K5", Key::K5,
		"K6", Key::K6,
		"K7", Key::K7,
		"K8", Key::K8,
		"K9", Key::K9,

		"Semicolon", Key::SEMICOLON,
		"Equal", Key::EQUAL,

		"A", Key::A,
		"B", Key::B,
		"C", Key::C,
		"D", Key::D,
		"E", Key::E,
		"F", Key::F,
		"G", Key::G,
		"H", Key::H,
		"I", Key::I,
		"J", Key::J,
		"K", Key::K,
		"L", Key::L,
		"M", Key::M,
		"N", Key::N,
		"O", Key::O,
		"P", Key::P,
		"Q", Key::Q,
		"R", Key::R,
		"S", Key::S,
		"T", Key::T,
		"U", Key::U,
		"V", Key::V,
		"W", Key::W,
		"X", Key::X,
		"Y", Key::Y,
		"Z", Key::Z,

		"LeftBracket", Key::LEFT_BRACKET,
		"Backslash", Key::BACKSLASH,
		"RightBracket", Key::RIGHT_BRACKET,

		"Escape", Key::ESCAPE,
		"Enter", Key::ENTER,
		"Tab", Key::TAB,
		"Backspace", Key::BACKSPACE,
		"Insert", Key::INSERT,
		"Del", Key::DEL,
		"Right", Key::RIGHT,
		"Left", Key::LEFT,
		"Down", Key::DOWN,
		"Up", Key::UP,
		"PageUp", Key::PAGE_UP,
		"PageDown", Key::PAGE_DOWN,
		"Home", Key::HOME,
		"End", Key::END,
		"CapsLock", Key::CAPS_LOCK,
		"ScrollLock", Key::SCROLL_LOCK,
		"NumClock", Key::NUM_LOCK,
		"PrintScreen", Key::PRINT_SCREEN,
		"Pause", Key::PAUSE,

		"F1", Key::F1,
		"F2", Key::F2,
		"F3", Key::F3,
		"F4", Key::F4,
		"F5", Key::F5,
		"F6", Key::F6,
		"F7", Key::F7,
		"F8", Key::F8,
		"F9", Key::F9,
		"F10", Key::F10,
		"F11", Key::F11,
		"F12", Key::F12,
		"F13", Key::F13,
		"F14", Key::F14,
		"F15", Key::F15,
		"F16", Key::F16,
		"F17", Key::F17,
		"F18", Key::F18,
		"F19", Key::F19,
		"F20", Key::F20,
		"F21", Key::F21,
		"F22", Key::F22,
		"F23", Key::F23,
		"F24", Key::F24,

		"Np0", Key::NP_0,
		"Np1", Key::NP_1,
		"Np2", Key::NP_2,
		"Np3", Key::NP_3,
		"Np4", Key::NP_4,
		"Np5", Key::NP_5,
		"Np6", Key::NP_6,
		"Np7", Key::NP_7,
		"Np8", Key::NP_8,
		"Np9", Key::NP_9,
		"NpDecimal", Key::NP_DECIMAL,
		"NpDivide", Key::NP_DIVIDE,
		"NpMultiply", Key::NP_MULTIPLY,
		"NpSubtract", Key::NP_SUBTRACT,
		"NpAdd", Key::NP_ADD,
		"NpEnter", Key::NP_ENTER,
		"NpEqual", Key::NP_EQUAL,

		"LeftShift", Key::LEFT_SHIFT,
		"LeftControl", Key::LEFT_CONTROL,
		"LeftAlt", Key::LEFT_ALT,
		"LeftSuper", Key::LEFT_SUPER,
		"RightShift", Key::RIGHT_SHIFT,
		"RightControl", Key::RIGHT_CONTROL,
		"RightAlt", Key::RIGHT_ALT,
		"RightSuper", Key::RIGHT_SUPER,
		"Menu", Key::MENU,

		"None", Key::NONE
	);
}

void InitialiseMouseConstants(sol::state& lua)
{
	lua.new_enum("Button",
		"Left", Button::LEFT,
		"Right", Button::RIGHT,
		"Wheel", Button::WHEEL,
		"Mouse4", Button::MOUSE4,
		"Mouse5", Button::MOUSE5
	);
}
