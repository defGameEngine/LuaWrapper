# LuaWrapper

A Lua scripting layer for [defGameEngine](https://github.com/defGameEngine/defGameEngine). Write your game logic entirely in Lua — the wrapper exposes the full engine API as Lua types, enums, and a global `app` object.

**Copyright 2026 defini7 and J-Starling.** Licensed under the GNU General Public License v3.0.

---

## Prerequisites

| Dependency | Version | Source |
|---|---|---|
| MSYS2 MinGW-w64 g++ | 16.x | https://www.msys2.org |
| Lua | 5.5 | MSYS2: `pacman -S mingw-w64-x86_64-lua` |
| GLFW3 | 3.x | MSYS2: `pacman -S mingw-w64-x86_64-glfw` |
| sol2 | 3.3.0 (patched) | https://github.com/defGameEngine/sol2 |

The sol2 fork at [defGameEngine/sol2](https://github.com/defGameEngine/sol2) includes the patches required for Lua 5.5 compatibility. Install it by copying the `include/sol` folder into your MSYS2 include path, e.g. `C:\msys64\mingw64\include\sol`.

---

## Building

```powershell
cd LuaWrapper\Build
.\build.ps1
```

Output: `LuaWrapper\Build\Target\LuaWrapper.exe` (and the required runtime DLLs alongside it).

---

## Running a Script

```
LuaWrapper.exe <path\to\script.lua>
```

Example:

```
LuaWrapper.exe Assets\Demo.lua
```

---

## Script Structure

Every script must define three global functions. The engine calls them in this order:

```lua
-- Called once before the first frame.
-- Return true to continue, false to abort.
function OnCreate()
    return true
end

-- Called every frame. dt is delta time in seconds.
-- Return true to keep running, false to exit.
function OnUpdate(dt)
    return true
end

-- Called once at startup to configure the window.
-- Must return a configuration table.
function CreateApp()
    return {
        title       = "My Game",   -- window title
        size        = { 320, 240,  -- screen width, height in pixels
                        3,   3 },  -- pixel size (each logical pixel = NxN screen pixels)
        full_screen = false,
        vsync       = false,
        dirty_pixel = false,       -- only redraw changed pixels
    }
end
```

The global `app` object (type `Application`) is available from `OnCreate` onward.

---

## Types

### Vector2i / Vector2f / Vector2d

Two-component integer, float, and double vectors.

```lua
local v = Vector2i:new(10, 20)    -- constructor
v.x = 5
v.y = 15
```

**Methods** (identical for all three types):

| Method | Returns | Description |
|---|---|---|
| `Clamp(min, max)` | self type | Clamp both components |
| `Lerp(other, t)` | self type | Linear interpolation |
| `Distance(other)` | number | Euclidean distance |
| `DotProduct(other)` | number | Dot product |
| `CrossProduct(other)` | number | Cross product (Z component) |
| `Angle(other)` | number | Angle between vectors (radians) |
| `Length()` | number | Euclidean length |
| `Length2()` | number | Squared length |
| `ManhattanDistance(other)` | number | Manhattan distance |
| `Max(other)` | self type | Component-wise maximum |
| `Min(other)` | self type | Component-wise minimum |
| `Swap()` | self type | Swap x and y |
| `Norm()` | self type | Normalised copy |
| `Abs()` | self type | Absolute value of each component |
| `Perpendicular()` | self type | Perpendicular vector |
| `Floor()` | self type | Floor each component |
| `Ceil()` | self type | Ceil each component |
| `Round()` | self type | Round each component |
| `Cartesian()` | self type | Polar → Cartesian (x=angle, y=radius) |
| `Polar()` | self type | Cartesian → Polar |
| `ToString()` | string | `"(x, y)"` |

**Operators** (defined by the Lua helpers layer):

`+`, `-`, `*`, `/`, `//`, `%`, `^`, unary `-`, `==`, `<`, `<=`, `tostring`

Each operator accepts a number scalar on the right-hand side or another vector of the same type.

---

### Pixel

An RGBA colour value. Each channel is a `uint8` (0–255).

```lua
local p = Pixel:new(255, 128, 0, 255)  -- r, g, b, a
p.r = 200
```

**Methods:**

| Method | Returns | Description |
|---|---|---|
| `Lerp(other, t)` | Pixel | Linear interpolation between two colours |
| `ToString()` | string | `"(r, g, b, a)"` |

**Free function:**

```lua
local p = PixelFloat(0.5, 0.0, 1.0, 1.0)  -- channels in [0.0, 1.0]
```

**Operators:** `+`, `-`, `*`, `/`, `//`, `tostring` — channels are clamped to [0, 255].

---

### KeyState

Returned by `app:GetKey()` and `app:GetMouse()`.

| Field | Type | Description |
|---|---|---|
| `held` | bool | Key is held down this frame |
| `pressed` | bool | Key was just pressed (first frame) |
| `released` | bool | Key was just released |

---

### Sprite

Pixel-based image (CPU-side).

```lua
local s = Sprite:new()                  -- empty
local s = Sprite:new(Vector2i:new(64, 64))  -- blank 64x64
local s = Sprite:new("Assets/tile.png")    -- load from file
```

| Member | Description |
|---|---|
| `size` | `Vector2i` — dimensions |
| `pixels` | Raw pixel array |

| Method | Description |
|---|---|
| `Create(size)` | Allocate blank sprite |
| `Load(path)` | Load from file |
| `Save(path, FileType)` | Save to file |
| `SetPixel(x, y, Pixel)` / `SetPixel(Vector2i, Pixel)` | Write a pixel |
| `GetPixel(x, y, WrapMethod)` / `GetPixel(Vector2i, WrapMethod)` | Read a pixel |
| `SetPixelData(pixels)` | Bulk-write pixel array |
| `Sample(u, v, SampleMethod, WrapMethod)` / `Sample(Vector2f, …)` | Sample at UV (0–1) |

---

### Texture

GPU-side texture built from a Sprite.

```lua
local t = Texture:new(sprite)
local t = Texture:new("Assets/tile.png")
```

| Member | Description |
|---|---|
| `id` | Internal GL texture ID |
| `uv_scale` | `Vector2f` — UV tiling scale |
| `size` | `Vector2i` — texture dimensions |

| Method | Description |
|---|---|
| `Load(path)` | Load from file |
| `Update(sprite)` | Re-upload from Sprite |

---

### Graphic

A combined CPU Sprite + GPU Texture, used as a render target.

```lua
local g = Graphic:new()
local g = Graphic:new("Assets/bg.png")
local g = Graphic:new(Vector2i:new(320, 240))
```

| Member | Description |
|---|---|
| `texture` | `Texture*` |
| `sprite` | `Sprite*` |

| Method | Description |
|---|---|
| `Load(path)` / `Load(Vector2i)` | Load or allocate |
| `Save(path)` | Save sprite to file |
| `UpdateTexture()` | Upload sprite pixels to GPU |

---

### Layer

A rendering layer returned by `app:GetLayerByIndex()`.

| Property | Type | Description |
|---|---|---|
| `visible` | bool | Whether the layer is drawn |
| `update` | bool | Whether the layer is updated |
| `tint` | Pixel | Tint colour applied over the layer |
| `offset` | `Vector2i` | Layer screen offset |
| `size` | `Vector2i` | Layer size (read-only) |
| `textureStructure` | TextureStructure | Geometry mode for texture draws |
| `pixelMode` | PixelMode | Pixel blending mode for this layer |

---

## Enums

### Key

Used with `app:GetKey(Key.X)`.

```
Space Apostrophe Comma Minus Period Slash
K0–K9 Semicolon Equal
A–Z
LeftBracket Backslash RightBracket
Escape Enter Tab Backspace Insert Del
Right Left Down Up PageUp PageDown Home End
CapsLock ScrollLock NumClock PrintScreen Pause
F1–F24
Np0–Np9 NpDecimal NpDivide NpMultiply NpSubtract NpAdd NpEnter NpEqual
LeftShift LeftControl LeftAlt LeftSuper
RightShift RightControl RightAlt RightSuper
Menu None
```

### Button

Used with `app:GetMouse(Button.X)`.

```
Left  Right  Wheel  Mouse4  Mouse5  Mouse6  Mouse7  Mouse8
```

### PixelMode

Used with `app:SetPixelMode(PixelMode.X)`.

```
Default  Alpha  Mask  Custom
```

### FileType

Used with `sprite:Save(path, FileType.X)`.

```
Bmp  Png  Jpg  Tga  TgaRle
```

### SampleMethod

Used with `app:SetSampleMethod` and `sprite:Sample`.

```
Linear  Bilinear  Trilinear
```

### WrapMethod

Used with `app:SetWrapMethod` and `sprite:GetPixel` / `sprite:Sample`.

```
None  Repeat  Mirror  Clamp
```

### TextureStructure

Used with `app:SetTextureStructure`, `app:DrawTexturePolygon`.

```
Default  Fan  Strip  Lines  LineStrip  Wireframe
```

---

## Colour Table

Pre-defined `Pixel` constants accessible as `Colour.Name`:

```
Black  DarkBlue  DarkGreen  DarkCyan  DarkRed  DarkMagenta
DarkGrey  DarkOrange  DarkBrown  DarkPurple
Orange  Grey  Blue  Green  Cyan  Red  Magenta  Yellow  White
Gold  Pink  Maroon  Lime  Brown  Beige  Violet  Purple  None
```

---

## Application API

All calls go through the global `app` object.

### Drawing — Pixels & Primitives

```lua
app:Draw(x, y, Pixel)                                        -- draw a single pixel
app:Draw(Vector2i, Pixel)
app:DrawLine(x1, y1, x2, y2, Pixel)
app:DrawLine(Vector2i, Vector2i, Pixel)
app:DrawTriangle(x1,y1, x2,y2, x3,y3, Pixel)
app:DrawTriangle(Vector2i, Vector2i, Vector2i, Pixel)
app:FillTriangle(...)
app:DrawRectangle(x, y, w, h, Pixel)
app:DrawRectangle(Vector2i, Vector2i, Pixel)
app:FillRectangle(...)
app:DrawCircle(x, y, r, Pixel)
app:DrawCircle(Vector2i, r, Pixel)
app:FillCircle(...)
app:DrawEllipse(x, y, rx, ry, Pixel)
app:DrawEllipse(Vector2i, Vector2i, Pixel)
app:FillEllipse(...)
app:DrawString(x, y, text, Pixel, scaleX, scaleY)
app:DrawString(Vector2i, text, Pixel, Vector2i)
app:Clear(Pixel)                                             -- clear pixel layer
app:ClearTexture(Pixel)                                      -- clear texture layer
```

### Drawing — Sprites

```lua
app:DrawSprite(x, y, Sprite*)
app:DrawSprite(Vector2i, Sprite*)
app:DrawPartialSprite(x, y, fx, fy, fw, fh, Sprite*)
app:DrawPartialSprite(Vector2i, Vector2i, Vector2i, Sprite*)
```

### Drawing — Textures

```lua
app:DrawTexture(Vector2f, Texture*, scale_Vector2f, tint_Pixel)
app:DrawPartialTexture(Vector2f, Texture*, fp_Vector2f, fs_Vector2f, scale_Vector2f, tint_Pixel)
app:DrawWarpedTexture(points_table, Texture*, tint_Pixel)    -- points = {Vector2f, ...}
app:DrawRotatedTexture(Vector2f, Texture*, angle, center_Vector2f, scale_Vector2f, tint_Pixel)
app:DrawPartialRotatedTexture(Vector2f, Texture*, fp, fs, angle, center, scale, tint)
```

### Drawing — Texture Geometry

```lua
app:DrawTextureLine(Vector2i, Vector2i, Pixel)
app:DrawTextureTriangle(Vector2i, Vector2i, Vector2i, Pixel)
app:FillTextureTriangle(Vector2i, Vector2i, Vector2i, Pixel)
app:DrawTextureRectangle(Vector2i, Vector2i, Pixel)
app:FillTextureRectangle(Vector2i, Vector2i, Pixel)
app:DrawTextureCircle(Vector2i, r, Pixel)
app:FillTextureCircle(Vector2i, r, Pixel)
app:DrawTextureString(Vector2i, text, Pixel, scale_Vector2f)
app:DrawTexturePolygon(vertices_table, colours_table, TextureStructure)
app:GradientTextureTriangle(Vector2i, Vector2i, Vector2i, Pixel, Pixel, Pixel)
app:GradientTextureRectangle(Vector2i, Vector2i, cTL, cTR, cBR, cBL)
```

### Drawing — Wireframe Models

```lua
app:DrawWireFrameModel(model_table, x, y, rotation, scale, Pixel)
app:DrawWireFrameModel(model_table, Vector2f, rotation, scale, Pixel)
app:FillWireFrameModel(...)
-- model_table: array of Vector2f defining the shape outline
```

### Render State

```lua
app:SetDrawTarget(Graphic*)          -- redirect drawing to a Graphic (nil = screen)
app:GetDrawTarget()                  -- returns Graphic* or nil
app:SetPixelMode(PixelMode)
app:GetPixelMode()                   -- returns PixelMode
app:SetWrapMethod(WrapMethod)
app:SetSampleMethod(SampleMethod)
app:SetTextureStructure(TextureStructure)
app:GetTextureStructure()
app:UseOnlyTextures(bool)            -- disable pixel-layer rendering
app:SetFont(path)                    -- custom bitmap font (BMP)
app:SetShader(fn)                    -- fn(Vector2i, Pixel src, Pixel dst) -> Pixel
```

### Input

```lua
local ks = app:GetKey(Key.A)         -- returns KeyState
local bs = app:GetMouse(Button.Left) -- returns KeyState
app:GetMousePos()                    -- returns Vector2i
app:GetMouseX()                      -- returns int
app:GetMouseY()                      -- returns int
app:GetMouseWheelDelta()             -- returns int

app:CaptureText(bool)                -- start/stop text capture mode
app:IsCapturingText()                -- returns bool
app:GetCapturedText()                -- returns string
app:GetCursorPos()                   -- returns cursor position in captured text
app:IsCaps()                         -- returns bool
app:ClearCapturedText()
```

### Window

```lua
app:GetScreenSize()                  -- returns Vector2i (logical pixels)
app:GetWindowSize()                  -- returns Vector2i (physical pixels)
app:ScreenWidth()                    -- returns int
app:ScreenHeight()                   -- returns int
app:GetInvertedScreenSize()          -- returns Vector2f (1/w, 1/h)
app:IsFullScreen()
app:IsVSync()
app:IsFocused()
app:IsDirtyPixel()
app:IsWindowClosed()
app:SetTitle(string)
app:SetIcon(path)
app:EnableVSync(bool)
app:EnableFullscreen(bool)
app:GetDropped()                     -- returns table of dropped file paths
```

### Timer

```lua
app:GetDeltaTime()                   -- returns float (seconds since last frame)
app:GetFPS()                         -- returns int
```

### Console

```lua
app:SetConsoleBackgroundColour(Pixel)
app:ShowConsole(bool)
app:IsConsoleEnabled()
app:ClearConsole()
```

### Layers

```lua
local id = app:CreateLayer(Vector2i offset, Vector2i size)
local id = app:CreateLayer(Vector2i offset, Vector2i size, bool update, bool visible, Pixel tint)
app:PickLayer(id)                    -- set active drawing layer
app:GetPickedLayer()                 -- returns current layer id
local layer = app:GetLayerByIndex(id)  -- returns Layer object
```

### States

```lua
app:PickState(id)
app:GetPickedState()
local state = app:GetStateByIndex(id)
```

---

## Helper Functions

These are injected into the Lua environment automatically before your script runs.

```lua
clamp(n, min, max)  -- clamp n to [min, max]

-- Installs arithmetic operators on a vector type table.
-- Called automatically for Vector2i, Vector2f, Vector2d.
AddVectorOperations(t)
```

---

## Minimal Example

```lua
local W, H = 256, 240

function CreateApp()
    return { title = "Hello", size = { W, H, 3, 3 } }
end

function OnCreate()
    return true
end

function OnUpdate(dt)
    app:Clear(Colour.Black)
    app:DrawString(10, 10, "Hello, World!", Colour.White, 1, 1)
    return not app:GetKey(Key.Escape).pressed
end
```

---

## Example Scripts

| File | Description |
|---|---|
| [Assets/Demo.lua](Assets/Demo.lua) | Four-page feature demo: shapes, text/pixel ops, input, vector math |
| [Assets/Test.lua](Assets/Test.lua) | Snake game |
