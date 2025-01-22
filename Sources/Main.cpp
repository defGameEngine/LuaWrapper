#include "defGameEngine.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#ifdef _WIN32
#pragma comment(lib, "lua547.lib")
#endif

#include "../Include/HelpersLua.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Constants.hpp"
#include "../Include/Types.hpp"

using namespace def;

inline sol::state lua(sol::c_call<decltype(&LuaPanic), &LuaPanic>);

class Application : public GameEngine
{
public:
	Application()
	{

	}

protected:
	bool OnUserCreate() override
	{
		if (!lua["OnCreate"].valid())
		{
			std::cerr << "[LUA] OnUserCreate function must be provided" << std::endl;
			return false;
		}

		if (!lua["OnUpdate"].valid())
		{
			std::cerr << "[LUA] OnUserUpdate function must be provided" << std::endl;
			return false;
		}

		return lua["OnCreate"]();
	}

	bool OnUserUpdate(float deltaTime) override
	{
		return lua["OnUpdate"](deltaTime);
	}
};

bool CreateApp(Vector2i& screenSize, Vector2i& pixelSize, std::string& title, bool& fullScreen, bool& vsync, bool& dirtyPixel)
{
	sol::function luaCreateApp = lua["CreateApp"];

	if (!luaCreateApp.valid())
	{
		std::cerr << "[LUA] CreateApp function must be provided" << std::endl;
		return false;
	}

	auto funcResult = luaCreateApp();

	if (funcResult.get_type() != sol::type::table)
	{
		std::cerr << "[LUA] CreateApp function must return a table" << std::endl;
		return false;
	}

	sol::table init = funcResult;

	title = init["title"].get_or<std::string_view>("Undefined");

	if (init["size"].valid())
	{
		screenSize.x = init["size"][1].get_or(256);
		screenSize.y = init["size"][2].get_or(240);

		pixelSize.x = init["size"][3].get_or(4);
		pixelSize.y = init["size"][4].get_or(4);
	}
	else
	{
		screenSize = { 256, 240 };
		pixelSize = { 4, 4 };
	}

	fullScreen = init["full_screen"].get_or(false);
	vsync = init["vsync"].get_or(false);
	dirtyPixel = init["dirty_pixel"].get_or(false);

	return true;
}

void RegisterKeyState()
{
	auto keyType = lua.new_usertype<KeyState>("KeyState",
		sol::constructors<KeyState(), KeyState(bool, bool, bool)>(),
		"held", sol::property(&KeyState::held, &KeyState::held),
		"released", sol::property(&KeyState::released, &KeyState::released),
		"pressed", sol::property(&KeyState::pressed, &KeyState::pressed)
	);
}

void RegisterPixel()
{
	InitialisePixelConstants(lua);
	InitialisePixelType(lua);
}

void RegisterSprite()
{
	InitialiseSpriteConstants(lua);
	InitialiseSpriteType(lua);
}

void RegisterTexture()
{
	InitialiseTextureConstants(lua);
	InitialiseTextureType(lua);
}

void RegisterApp()
{
	lua.new_usertype<Application>("Application",
		"Draw", sol::overload(
			static_cast<bool (Application::*)(int, int, const Pixel&)>(&Application::Draw),
			static_cast<bool (Application::*)(const Vector2i&, const Pixel&)>(&Application::Draw)
		),
		"DrawLine", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, const Pixel&)>(&Application::DrawLine),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Pixel&)>(&Application::DrawLine)
		),
		"DrawTriangle", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, int, int, const Pixel&)>(&Application::DrawTriangle),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Vector2i&, const Pixel&)>(&Application::DrawTriangle)
		),
		"FillTriangle", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, int, int, const Pixel&)>(&Application::FillTriangle),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Vector2i&, const Pixel&)>(&Application::FillTriangle)
		),
		"DrawRectangle", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, const Pixel&)>(&Application::DrawRectangle),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Pixel&)>(&Application::DrawRectangle)
		),
		"FillRectangle", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, const Pixel&)>(&Application::FillRectangle),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Pixel&)>(&Application::FillRectangle)
		),
		"DrawCircle", sol::overload(
			static_cast<void (Application::*)(int, int, int, const Pixel&)>(&Application::DrawCircle),
			static_cast<void (Application::*)(const Vector2i&, int, const Pixel&)>(&Application::DrawCircle)
		),
		"FillCircle", sol::overload(
			static_cast<void (Application::*)(int, int, int, const Pixel&)>(&Application::FillCircle),
			static_cast<void (Application::*)(const Vector2i&, int, const Pixel&)>(&Application::FillCircle)
		),
		"DrawEllipse", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, const Pixel&)>(&Application::DrawEllipse),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Pixel&)>(&Application::DrawEllipse)
		),
		"FillEllipse", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, const Pixel&)>(&Application::FillEllipse),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Pixel&)>(&Application::FillEllipse)
		),
		"DrawSprite", sol::overload(
			static_cast<void (Application::*)(int, int, const Sprite*)>(&Application::DrawSprite),
			static_cast<void (Application::*)(const Vector2i&, const Sprite*)>(&Application::DrawSprite)
		),
		"DrawPartialSprite", sol::overload(
			static_cast<void (Application::*)(int, int, int, int, int, int, const Sprite*)>(&Application::DrawPartialSprite),
			static_cast<void (Application::*)(const Vector2i&, const Vector2i&, const Vector2i&, const Sprite*)>(&Application::DrawPartialSprite)
		),
		"DrawTexture", &Application::DrawTexture,
		"DrawPartialTexture", &Application::DrawPartialTexture,
		"DrawWarpedTexture", &Application::DrawWarpedTexture,
		"DrawRotatedTexture", &Application::DrawRotatedTexture,
		"DrawPartialRotatedTexture", &Application::DrawPartialRotatedTexture,
		"DrawWireFrameModel", sol::overload(
			static_cast<void (Application::*)(const std::vector<Vector2f>&, float, float, float, float, const Pixel&)>(&Application::DrawWireFrameModel),
			static_cast<void (Application::*)(const std::vector<Vector2f>&, const Vector2f&, float, float, const Pixel&)>(&Application::DrawWireFrameModel)
		),
		"FillWireFrameModel", sol::overload(
			static_cast<void (Application::*)(const std::vector<Vector2f>&, float, float, float, float, const Pixel&)>(&Application::FillWireFrameModel),
			static_cast<void (Application::*)(const std::vector<Vector2f>&, const Vector2f&, float, float, const Pixel&)>(&Application::FillWireFrameModel)
		),
		"DrawTexturePolygon", &Application::DrawTexturePolygon,
		"DrawTextureLine", &Application::DrawTextureLine,
		"DrawTextureTriangle", &Application::DrawTextureTriangle,
		"FillTextureTriangle", &Application::FillTextureTriangle,
		"DrawTextureRectangle", &Application::DrawTextureRectangle,
		"FillTextureRectangle", &Application::FillTextureRectangle,
		"DrawTextureCircle", &Application::DrawTextureCircle,
		"FillTextureCircle", &Application::FillTextureCircle,
		"DrawTextureString", &Application::DrawTextureString,
		"GradientTextureTriangle", &Application::GradientTextureTriangle,
		"GradientTextureRectangle", &Application::GradientTextureRectangle,
		"DrawString", sol::overload(
			static_cast<void (Application::*)(int, int, std::string_view, const Pixel&, int, int)>(&Application::DrawString),
			static_cast<void (Application::*)(const Vector2i&, std::string_view, const Pixel&, const Vector2i&)>(&Application::DrawString)
		),
		"Clear", &Application::Clear,
		"ClearTexture", &Application::ClearTexture,
		
		"SetDrawTarget", &Application::SetDrawTarget,
		"GetDrawTarget", &Application::GetDrawTarget,
		
		"SetPixelMode", &Application::SetDrawTarget,
		"GetPixelMode", &Application::GetDrawTarget,
		
		"SetTextureStructure", &Application::SetTextureStructure,
		"GetTextureStructure", &Application::GetTextureStructure,
		
		"SetShader", &Application::SetShader,
		"UseOnlyTextures", &Application::UseOnlyTextures,
		
		"GetKey", &Application::GetKey,
		"GetMouse", &Application::GetMouse,
		
		"GetMousePos", &Application::GetMousePos,
		"GetMouseX", &Application::GetMouseX,
		"GetMouseY", &Application::GetMouseY,
		"GetMouseWheelDelta", &Application::GetMouseWheelDelta,
		
		"GetDeltaTime", &Application::GetDeltaTime,
		
		"SetTitle", &Application::SetTitle,
		
		"GetScreenSize", &Application::GetScreenSize,
		"GetWindowSize", &Application::GetWindowSize,
		
		"ScreenWidth", &Application::ScreenWidth,
		"ScreenHeight", &Application::ScreenHeight,
		
		"IsFullScreen", &Application::IsFullScreen,
		"IsVSync", &Application::IsVSync,
		"IsFocused", &Application::IsFocused,
		
		"SetIcon", &Application::SetIcon,
		
		"GetDropped", &Application::GetDropped,
		
		"CaptureText", &Application::CaptureText,
		"IsCapturingText", &Application::IsCapturingText,
		"GetCapturedText", &Application::GetCapturedText,
		"GetCursorPos", &Application::GetCursorPos,
		
		"SetConsoleBackgroundColour", &Application::SetConsoleBackgroundColour,
		"ShowConsole", &Application::ShowConsole,
		"IsConsoleEnabled", &Application::IsConsoleEnabled,
		"ClearCapturedText", &Application::ClearCapturedText,
		"ClearConsole", &Application::ClearConsole,
		
		"IsCaps", &Application::IsCaps,
		
		"CreateLayer", &Application::CreateLayer,
		"PickLayer", &Application::PickLayer,
		"GetPickedLayer", &Application::GetPickedLayer,
		"GetLayerByIndex", &Application::GetLayerByIndex
	);
}

void RegisterAll(std::shared_ptr<Application> app)
{
	InitialiseVec2dType<Vector2i>(lua, "Vector2i");
	InitialiseVec2dType<Vector2f>(lua, "Vector2f");

	RegisterKeyState();
	InitialiseKeyboardConstants(lua);
	InitialiseMouseConstants(lua);
	RegisterApp();
	RegisterPixel();
	RegisterSprite();
	RegisterTexture();
	InitialiseGraphicType(lua);
	InitialiseWindowState(lua);

	lua["app"] = app;
}

bool RunApplication(std::shared_ptr<Application> app)
{
	Vector2i screenSize, pixelSize;
	std::string title;

	bool fullScreen, vsync, dirtyPixel;

	if (!CreateApp(screenSize, pixelSize, title, fullScreen, vsync, dirtyPixel))
		return false;

	app->SetTitle(title);

	if (app->Construct(screenSize.x, screenSize.y, pixelSize.x, pixelSize.y, fullScreen, vsync, dirtyPixel))
	{
		app->Run();
		return true;
	}

	return false;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "[C++] No filename has been provided" << std::endl;
		return 1;
	}

	lua.open_libraries();

	auto app = std::make_shared<Application>();
	RegisterAll(app);

	if (!lua.script(g_HelpersSource).valid())
		return 1;

	if (!lua.script_file(argv[1]).valid())
		return 1;

	return RunApplication(app) ? EXIT_SUCCESS : EXIT_FAILURE;
}