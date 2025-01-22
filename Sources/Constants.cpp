#include "../Include/Constants.hpp"
#include "defGameEngine.hpp"

using namespace def;

void InitialisePixelConstants(sol::state& lua)
{
	lua.new_enum("PixelMode",
		"Default", Pixel::Mode::DEFAULT,
		"Alpha", Pixel::Mode::ALPHA,
		"Mask", Pixel::Mode::MASK,
		"Custom", Pixel::Mode::CUSTOM
	);

	lua["Colour"] = lua.create_table_with(
		"Black", BLACK,
		"DarkBlue", DARK_BLUE,
		"DarkGreen", DARK_GREEN,
		"DarkCyan", DARK_CYAN,
		"DarkRed", DARK_RED,
		"DarkMagenta", DARK_MAGENTA,
		"DarkGrey", DARK_GREY,
		"DarkOrange", DARK_ORANGE,
		"DarkBrown", DARK_BROWN,
		"DarkPurple", DARK_PURPLE,
		"Orange", ORANGE,
		"Grey", GREY,
		"Blue", BLUE,
		"Green", GREEN,
		"Cyan", CYAN,
		"Red", RED,
		"Magenta", MAGENTA,
		"Yellow", YELLOW,
		"White", WHITE,
		"Gold", GOLD,
		"Pink", PINK,
		"Maroon", MAROON,
		"Lime", LIME,
		"Brown", BROWN,
		"Beige", BEIGE,
		"Violet", VIOLET,
		"Purple", PURPLE,
		"None", NONE
	);
}

void InitialiseSpriteConstants(sol::state& lua)
{
	lua.new_enum("FileType",
		"Bmp", Sprite::FileType::BMP,
		"Png", Sprite::FileType::PNG,
		"Jpg", Sprite::FileType::JPG,
		"Tga", Sprite::FileType::TGA,
		"TgaRle", Sprite::FileType::TGA_RLE
	);

	lua.new_enum("SampleMethod",
		"Linear", Sprite::SampleMethod::LINEAR,
		"Bilinear", Sprite::SampleMethod::BILINEAR,
		"Trilinear", Sprite::SampleMethod::TRILINEAR
	);

	lua.new_enum("WrapMethod",
		"None", Sprite::WrapMethod::NONE,
		"Repeat", Sprite::WrapMethod::REPEAT,
		"Mirror", Sprite::WrapMethod::MIRROR,
		"Clamp", Sprite::WrapMethod::CLAMP
	);
}

void InitialiseTextureConstants(sol::state& lua)
{
	lua.new_enum("TextureStructure",
		"Default", Texture::Structure::DEFAULT,
		"Fan", Texture::Structure::FAN,
		"Strip", Texture::Structure::STRIP
	);
}

void InitialiseWindowState(sol::state& lua)
{
	lua.new_enum("WindowState",
		"None", WindowState::NONE,
		"Maximized", WindowState::MAXIMIZED,
		"Focused", WindowState::FOCUSED
	);
}
