/*-----------------------------------------------------------------
 *  Copyright 2026 defini7 and J-Starling. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "defGameEngine.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "../Include/HelpersLua.hpp"
#include "../Include/Utils.hpp"
#include "../Include/Constants.hpp"
#include "../Include/Types.hpp"

using namespace def;

static int wrap_LuaPanic(lua_State* L)
{
	const char* msg = lua_tostring(L, -1);
	LuaPanic(msg ? std::optional<std::string>{msg} : std::nullopt);
	return 0;
}

sol::state g_Lua(wrap_LuaPanic);

class Application : public GameEngine
{
public:
	Application() {}

	// Input

	KeyState GetKey(Key key) { return Input().GetKeyState(key); }
	KeyState GetMouse(Button btn) { return Input().GetButtonState(btn); }
	Vector2i GetMousePos() { return Input().GetMousePosition(); }
	int GetMouseX() { return Input().GetMouseX(); }
	int GetMouseY() { return Input().GetMouseY(); }
	int GetMouseWheelDelta() { return Input().GetScrollDelta(); }
	void CaptureText(bool enable) { Input().CaptureText(enable); }
	bool IsCapturingText() { return Input().IsCapturingText(); }
	std::string GetCapturedText() { return Input().GetCapturedText(); }
	size_t GetCursorPos() { return Input().GetCapturedTextCursorPosition(); }
	bool IsCaps() { return Input().IsCaps(); }
	void ClearCapturedText() { Input().ClearCapturedText(); }

	// Window

	Vector2i GetScreenSize() { return GameEngine::Window().GetScreenSize(); }
	Vector2i GetWindowSize() { return GameEngine::Window().GetWindowSize(); }
	int ScreenWidth() { return GameEngine::Window().GetScreenWidth(); }
	int ScreenHeight() { return GameEngine::Window().GetScreenHeight(); }
	Vector2f GetInvertedScreenSize() { return GameEngine::Window().GetInvertedScreenSize(); }
	bool IsFullScreen() { return GameEngine::Window().IsFullScreen(); }
	bool IsVSync() { return GameEngine::Window().IsVSync(); }
	bool IsFocused() { return GameEngine::Window().IsFocused(); }
	bool IsDirtyPixel() { return GameEngine::Window().IsDirtyPixel(); }
	bool IsWindowClosed() { return GameEngine::Window().IsWindowClosed(); }
	void SetTitle(const std::string& title) { GameEngine::Window().SetTitle(title); }
	void SetIcon(const std::string& path) { GameEngine::Window().SetIcon(path); }
	std::vector<std::string> GetDropped() { return GameEngine::Window().GetDroppedFiles(); }
	void EnableVSync(bool enable) { GameEngine::Window().EnableVSync(enable); }
	void EnableFullscreen(bool enable) { GameEngine::Window().EnableFullscreen(enable); }

	// Timer

	float GetDeltaTime() { return GameEngine::Timer().GetDeltaTime(); }
	int GetFPS() { return GameEngine::Timer().GetFPS(); }

	// Console

	void SetConsoleBackgroundColour(const Pixel& col) { GameEngine::Console().SetBackgroundColour(col); }
	void ShowConsole(bool show) { GameEngine::Console().Show(show); }
	bool IsConsoleEnabled() { return GameEngine::Console().IsShown(); }
	void ClearConsole() { GameEngine::Console().Clear(); }

	// Layers

	size_t CreateLayerSimple(const Vector2i& offset, const Vector2i& size)
	{
		return GameEngine::CreateLayer(offset, size);
	}

	size_t CreateLayerFull(const Vector2i& offset, const Vector2i& size, bool update, bool visible, const Pixel& tint)
	{
		return GameEngine::CreateLayer(offset, size, update, visible, tint);
	}

	void PickLayer(size_t index) { GameEngine::SetLayer(index); }
	size_t GetPickedLayer() { return GameEngine::GetCurrentLayer(); }
	Layer* GetLayerByIndex(size_t index) { return GameEngine::GetLayer(index); }

	// States

	void PickState(size_t index) { GameEngine::SetState(index); }
	size_t GetPickedState() { return GameEngine::GetCurrentState(); }
	State* GetStateByIndex(size_t index) { return GameEngine::GetState(index); }

	// Audio

	bool AudioLoad(uint32_t id, const std::string& path) { return GameEngine::Audio().Load(id, path); }
	void AudioUnload(uint32_t id) { GameEngine::Audio().Unload(id); }
	void AudioPlay(uint32_t id, bool loop, float volume) { GameEngine::Audio().Play(id, loop, volume); }
	void AudioPlayShot(const std::string& path) { GameEngine::Audio().Play(path); }
	void AudioStop(uint32_t id) { GameEngine::Audio().Stop(id); }
	void AudioPause(uint32_t id) { GameEngine::Audio().Pause(id); }
	void AudioResume(uint32_t id) { GameEngine::Audio().Resume(id); }
	bool AudioIsPlaying(uint32_t id) { return GameEngine::Audio().IsPlaying(id); }
	void AudioSetVolume(uint32_t id, float volume) { GameEngine::Audio().SetVolume(id, volume); }
	float AudioGetVolume(uint32_t id) { return GameEngine::Audio().GetVolume(id); }
	void AudioSetMasterVolume(float volume) { GameEngine::Audio().SetMasterVolume(volume); }
	float AudioGetMasterVolume() { return GameEngine::Audio().GetMasterVolume(); }
	void AudioStopAll() { GameEngine::Audio().StopAll(); }

protected:
	bool OnUserCreate() override
	{
		if (!g_Lua["OnCreate"].valid())
		{
			std::cerr << "[LUA] OnCreate function must be provided" << std::endl;
			return false;
		}

		if (!g_Lua["OnUpdate"].valid())
		{
			std::cerr << "[LUA] OnUpdate function must be provided" << std::endl;
			return false;
		}

		return g_Lua["OnCreate"]();
	}

	bool OnUserUpdate(float deltaTime) override
	{
		return g_Lua["OnUpdate"](deltaTime);
	}
};

bool CreateApp(Vector2i& screenSize, Vector2i& pixelSize, std::string& title, bool& fullScreen, bool& vsync, bool& dirtyPixel)
{
	sol::function luaCreateApp = g_Lua["CreateApp"];

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
	g_Lua.new_usertype<KeyState>("KeyState",
		sol::constructors<KeyState(), KeyState(bool, bool, bool)>(),
		"held", sol::property(&KeyState::held, &KeyState::held),
		"released", sol::property(&KeyState::released, &KeyState::released),
		"pressed", sol::property(&KeyState::pressed, &KeyState::pressed)
	);
}

void RegisterPixel()
{
	InitialisePixelConstants(g_Lua);
	InitialisePixelType(g_Lua);
}

void RegisterSprite()
{
	InitialiseSpriteConstants(g_Lua);
	InitialiseSpriteType(g_Lua);
}

void RegisterTexture()
{
	InitialiseTextureConstants(g_Lua);
	InitialiseTextureType(g_Lua);
}

void RegisterApp()
{
#define L [_=0]

	g_Lua.new_usertype<Application>("Application",
		// Drawing
		"Draw", sol::overload(
			L(Application& a, int x, int y, const Pixel& c) -> bool { return a.Draw(x, y, c); },
			L(Application& a, const Vector2i& p, const Pixel& c) -> bool { return a.Draw(p, c); }
		),
		"DrawLine", sol::overload(
			L(Application& a, int x1, int y1, int x2, int y2, const Pixel& c) { a.DrawLine(x1, y1, x2, y2, c); },
			L(Application& a, const Vector2i& p1, const Vector2i& p2, const Pixel& c) { a.DrawLine(p1, p2, c); }
		),
		"DrawTriangle", sol::overload(
			L(Application& a, int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& c) { a.DrawTriangle(x1, y1, x2, y2, x3, y3, c); },
			L(Application& a, const Vector2i& p1, const Vector2i& p2, const Vector2i& p3, const Pixel& c) { a.DrawTriangle(p1, p2, p3, c); }
		),
		"FillTriangle", sol::overload(
			L(Application& a, int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& c) { a.FillTriangle(x1, y1, x2, y2, x3, y3, c); },
			L(Application& a, const Vector2i& p1, const Vector2i& p2, const Vector2i& p3, const Pixel& c) { a.FillTriangle(p1, p2, p3, c); }
		),
		"DrawRectangle", sol::overload(
			L(Application& a, int x, int y, int w, int h, const Pixel& c) { a.DrawRectangle(x, y, w, h, c); },
			L(Application& a, const Vector2i& p, const Vector2i& s, const Pixel& c) { a.DrawRectangle(p, s, c); }
		),
		"FillRectangle", sol::overload(
			L(Application& a, int x, int y, int w, int h, const Pixel& c) { a.FillRectangle(x, y, w, h, c); },
			L(Application& a, const Vector2i& p, const Vector2i& s, const Pixel& c) { a.FillRectangle(p, s, c); }
		),
		"DrawCircle", sol::overload(
			L(Application& a, int x, int y, int r, const Pixel& c) { a.DrawCircle(x, y, r, c); },
			L(Application& a, const Vector2i& p, int r, const Pixel& c) { a.DrawCircle(p, r, c); }
		),
		"FillCircle", sol::overload(
			L(Application& a, int x, int y, int r, const Pixel& c) { a.FillCircle(x, y, r, c); },
			L(Application& a, const Vector2i& p, int r, const Pixel& c) { a.FillCircle(p, r, c); }
		),
		"DrawEllipse", sol::overload(
			L(Application& a, int x, int y, int rx, int ry, const Pixel& c) { a.DrawEllipse(x, y, rx, ry, c); },
			L(Application& a, const Vector2i& p, const Vector2i& r, const Pixel& c) { a.DrawEllipse(p, r, c); }
		),
		"FillEllipse", sol::overload(
			L(Application& a, int x, int y, int rx, int ry, const Pixel& c) { a.FillEllipse(x, y, rx, ry, c); },
			L(Application& a, const Vector2i& p, const Vector2i& r, const Pixel& c) { a.FillEllipse(p, r, c); }
		),
		"DrawSprite", sol::overload(
			L(Application& a, int x, int y, const Sprite* s) { a.DrawSprite(x, y, s); },
			L(Application& a, const Vector2i& p, const Sprite* s) { a.DrawSprite(p, s); }
		),
		"DrawPartialSprite", sol::overload(
			L(Application& a, int x, int y, int fx, int fy, int fw, int fh, const Sprite* s) { a.DrawPartialSprite(x, y, fx, fy, fw, fh, s); },
			L(Application& a, const Vector2i& p, const Vector2i& fp, const Vector2i& fs, const Sprite* s) { a.DrawPartialSprite(p, fp, fs, s); }
		),
		"DrawTexture", L(Application& a, const Vector2f& p, const Texture* t, const Vector2f& sc, const Pixel& tint) {
			a.DrawTexture(p, t, sc, tint);
		},
		"DrawPartialTexture", L(Application& a, const Vector2f& p, const Texture* t, const Vector2f& fp, const Vector2f& fs, const Vector2f& sc, const Pixel& tint) {
			a.DrawPartialTexture(p, t, fp, fs, sc, tint);
		},
		"DrawWarpedTexture", L(Application& a, const std::vector<Vector2f>& pts, const Texture* t, const Pixel& tint) {
			a.DrawWarpedTexture(pts, t, tint);
		},
		"DrawRotatedTexture", L(Application& a, const Vector2f& p, const Texture* t, float rot, const Vector2f& ctr, const Vector2f& sc, const Pixel& tint) {
			a.DrawRotatedTexture(p, t, rot, ctr, sc, tint);
		},
		"DrawPartialRotatedTexture", L(Application& a, const Vector2f& p, const Texture* t, const Vector2f& fp, const Vector2f& fs, float rot, const Vector2f& ctr, const Vector2f& sc, const Pixel& tint) {
			a.DrawPartialRotatedTexture(p, t, fp, fs, rot, ctr, sc, tint);
		},
		"DrawWireFrameModel", sol::overload(
			L(Application& a, const std::vector<Vector2f>& m, float x, float y, float r, float s, const Pixel& c) { a.DrawWireFrameModel(m, x, y, r, s, c); },
			L(Application& a, const std::vector<Vector2f>& m, const Vector2f& p, float r, float s, const Pixel& c) { a.DrawWireFrameModel(m, p, r, s, c); }
		),
		"FillWireFrameModel", sol::overload(
			L(Application& a, const std::vector<Vector2f>& m, float x, float y, float r, float s, const Pixel& c) { a.FillWireFrameModel(m, x, y, r, s, c); },
			L(Application& a, const std::vector<Vector2f>& m, const Vector2f& p, float r, float s, const Pixel& c) { a.FillWireFrameModel(m, p, r, s, c); }
		),
		"DrawTexturePolygon", L(Application& a, const std::vector<Vector2f>& v, const std::vector<Pixel>& c, Texture::Structure s) {
			a.DrawTexturePolygon(v, c, s);
		},
		"DrawTextureLine", L(Application& a, const Vector2i& p1, const Vector2i& p2, const Pixel& c) { a.DrawTextureLine(p1, p2, c); },
		"DrawTextureTriangle", L(Application& a, const Vector2i& p1, const Vector2i& p2, const Vector2i& p3, const Pixel& c) { a.DrawTextureTriangle(p1, p2, p3, c); },
		"FillTextureTriangle", L(Application& a, const Vector2i& p1, const Vector2i& p2, const Vector2i& p3, const Pixel& c) { a.FillTextureTriangle(p1, p2, p3, c); },
		"DrawTextureRectangle", L(Application& a, const Vector2i& p, const Vector2i& s, const Pixel& c) { a.DrawTextureRectangle(p, s, c); },
		"FillTextureRectangle", L(Application& a, const Vector2i& p, const Vector2i& s, const Pixel& c) { a.FillTextureRectangle(p, s, c); },
		"DrawTextureCircle", L(Application& a, const Vector2i& p, int r, const Pixel& c) { a.DrawTextureCircle(p, r, c); },
		"FillTextureCircle", L(Application& a, const Vector2i& p, int r, const Pixel& c) { a.FillTextureCircle(p, r, c); },
		"DrawTextureString", L(Application& a, const Vector2i& p, std::string_view t, const Pixel& c, const Vector2f& s) {
			a.DrawTextureString(p, t, c, s);
		},
		"GradientTextureTriangle", L(Application& a, const Vector2i& p1, const Vector2i& p2, const Vector2i& p3, const Pixel& c1, const Pixel& c2, const Pixel& c3) {
			a.GradientTextureTriangle(p1, p2, p3, c1, c2, c3);
		},
		"GradientTextureRectangle", L(Application& a, const Vector2i& p, const Vector2i& s, const Pixel& cTL, const Pixel& cTR, const Pixel& cBR, const Pixel& cBL) {
			a.GradientTextureRectangle(p, s, cTL, cTR, cBR, cBL);
		},
		"DrawString", sol::overload(
			L(Application& a, int x, int y, std::string_view t, const Pixel& c, int sx, int sy) { a.DrawString(x, y, t, c, sx, sy); },
			L(Application& a, const Vector2i& p, std::string_view t, const Pixel& c, const Vector2i& s) { a.DrawString(p, t, c, s); }
		),
		"Clear", L(Application& a, const Pixel& c) { a.Clear(c); },
		"ClearTexture", L(Application& a, const Pixel& c) { a.ClearTexture(c); },

		// Render targets
		"SetDrawTarget", L(Application& a, Graphic* g) { a.SetDrawTarget(g); },
		"GetDrawTarget", L(Application& a) -> Graphic* { return a.GetDrawTarget(); },

		// Pixel mode
		"SetPixelMode", L(Application& a, Pixel::Mode m) { a.SetPixelMode(m); },
		"GetPixelMode", L(const Application& a) -> Pixel::Mode { return a.GetPixelMode(); },

		// Texture settings
		"SetWrapMethod", L(Application& a, Sprite::WrapMethod m) { a.SetWrapMethod(m); },
		"SetSampleMethod", L(Application& a, Sprite::SampleMethod m) { a.SetSampleMethod(m); },
		"SetTextureStructure", L(Application& a, Texture::Structure s) { a.SetTextureStructure(s); },
		"GetTextureStructure", L(const Application& a) -> Texture::Structure { return a.GetTextureStructure(); },

		// Shaders and modes
		"SetShader", L(Application& a, Pixel (*fn)(const Vector2i&, const Pixel&, const Pixel&)) { a.SetShader(fn); },
		"UseOnlyTextures", L(Application& a, bool e) { a.UseOnlyTextures(e); },
		"SetFont", L(Application& a, std::string_view f) { a.SetFont(f); },

		// Input
		"GetKey", &Application::GetKey,
		"GetMouse", &Application::GetMouse,
		"GetMousePos", &Application::GetMousePos,
		"GetMouseX", &Application::GetMouseX,
		"GetMouseY", &Application::GetMouseY,
		"GetMouseWheelDelta", &Application::GetMouseWheelDelta,
		"CaptureText", &Application::CaptureText,
		"IsCapturingText", &Application::IsCapturingText,
		"GetCapturedText", &Application::GetCapturedText,
		"GetCursorPos", &Application::GetCursorPos,
		"IsCaps", &Application::IsCaps,
		"ClearCapturedText", &Application::ClearCapturedText,

		// Timer
		"GetDeltaTime", &Application::GetDeltaTime,
		"GetFPS", &Application::GetFPS,

		// Window
		"SetTitle", &Application::SetTitle,
		"SetIcon", &Application::SetIcon,
		"GetScreenSize", &Application::GetScreenSize,
		"GetWindowSize", &Application::GetWindowSize,
		"GetInvertedScreenSize", &Application::GetInvertedScreenSize,
		"ScreenWidth", &Application::ScreenWidth,
		"ScreenHeight", &Application::ScreenHeight,
		"IsFullScreen", &Application::IsFullScreen,
		"IsVSync", &Application::IsVSync,
		"IsFocused", &Application::IsFocused,
		"IsDirtyPixel", &Application::IsDirtyPixel,
		"IsWindowClosed", &Application::IsWindowClosed,
		"EnableVSync", &Application::EnableVSync,
		"EnableFullscreen", &Application::EnableFullscreen,
		"GetDropped", &Application::GetDropped,

		// Console
		"SetConsoleBackgroundColour", &Application::SetConsoleBackgroundColour,
		"ShowConsole", &Application::ShowConsole,
		"IsConsoleEnabled", &Application::IsConsoleEnabled,
		"ClearConsole", &Application::ClearConsole,

		// Layers
		"CreateLayer", sol::overload(
			&Application::CreateLayerSimple,
			&Application::CreateLayerFull
		),
		"PickLayer", &Application::PickLayer,
		"GetPickedLayer", &Application::GetPickedLayer,
		"GetLayerByIndex", &Application::GetLayerByIndex,

		// States
		"PickState", &Application::PickState,
		"GetPickedState", &Application::GetPickedState,
		"GetStateByIndex", &Application::GetStateByIndex,

		// Audio
		"LoadSound", &Application::AudioLoad,
		"UnloadSound", &Application::AudioUnload,
		"Play", &Application::AudioPlay,
		"PlayShot", &Application::AudioPlayShot,
		"StopSound", &Application::AudioStop,
		"PauseSound", &Application::AudioPause,
		"ResumeSound", &Application::AudioResume,
		"IsSoundPlaying", &Application::AudioIsPlaying,
		"SetSoundVolume", &Application::AudioSetVolume,
		"GetSoundVolume", &Application::AudioGetVolume,
		"SetMasterVolume", &Application::AudioSetMasterVolume,
		"GetMasterVolume", &Application::AudioGetMasterVolume,
		"StopAllSounds", &Application::AudioStopAll
	);
#undef L
}

void RegisterAll(std::shared_ptr<Application> app)
{
	InitialiseVec2dType<Vector2i>(g_Lua, "Vector2i");
	InitialiseVec2dType<Vector2f>(g_Lua, "Vector2f");
	InitialiseVec2dType<Vector2d>(g_Lua, "Vector2d");

	RegisterKeyState();
	InitialiseKeyboardConstants(g_Lua);
	InitialiseMouseConstants(g_Lua);
	RegisterApp();
	RegisterPixel();
	RegisterSprite();
	RegisterTexture();
	InitialiseGraphicType(g_Lua);
	InitialiseLayerType(g_Lua);

	g_Lua["Dge"] = app;
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

	g_Lua.open_libraries();

	// sol2's default traceback handler calls unqualified_check_get<string_view> on stack index 1,
	// which panics under Lua 5.5 when the error object is not a plain string. Disable it so our
	// wrap_LuaPanic remains the sole error sink.
	sol::protected_function::set_default_handler(sol::object{});

	auto app = std::make_shared<Application>();
	RegisterAll(app);

	if (!g_Lua.script(g_HelpersSource).valid())
		return 1;

	if (!g_Lua.script_file(argv[1]).valid())
		return 1;

	return RunApplication(app) ? EXIT_SUCCESS : EXIT_FAILURE;
}
