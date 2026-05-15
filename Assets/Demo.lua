--[[
    defGameEngine Lua Wrapper — Demo
    Навигация: стрелки / WASD — двигать шарик
    F1-F4 — переключать страницы
    ESC — выход (закрыть окно)
--]]

-- ─── Константы ──────────────────────────────────────────────────────────────

local SCREEN_W = 320
local SCREEN_H = 240
local PAGE_COUNT = 4

-- ─── Состояние ──────────────────────────────────────────────────────────────

local page        = 1
local ball_pos    = Vector2f:new(SCREEN_W / 2, SCREEN_H / 2)
local ball_vel    = Vector2f:new(60, 45)        -- пикселей/сек
local ball_radius = 8

local hue_t   = 0.0   -- для плавной смены цвета
local blink_t = 0.0

-- ─── Утилиты ────────────────────────────────────────────────────────────────

local function lerp(a, b, t)
    return a + (b - a) * t
end

-- HSV → RGB (s=1, v=1), hue ∈ [0,1)
local function hue_to_pixel(h)
    local i = math.floor(h * 6)
    local f = h * 6 - i
    local q = 1 - f
    local r, g, b
    i = i % 6
    if     i == 0 then r, g, b = 255, math.floor(f*255), 0
    elseif i == 1 then r, g, b = math.floor(q*255), 255, 0
    elseif i == 2 then r, g, b = 0, 255, math.floor(f*255)
    elseif i == 3 then r, g, b = 0, math.floor(q*255), 255
    elseif i == 4 then r, g, b = math.floor(f*255), 0, 255
    else               r, g, b = 255, 0, math.floor(q*255)
    end
    return Pixel:new(r, g, b, 255)
end

-- Нарисовать рамку
local function draw_border(col)
    app:DrawRectangle(0, 0, SCREEN_W - 1, SCREEN_H - 1, col)
end

-- Заголовок страницы
local function draw_title(text)
    app:FillRectangle(0, 0, SCREEN_W, 10, Colour.DarkBlue)
    app:DrawString(2, 1, text, Colour.Yellow, 1, 1)
    app:DrawString(SCREEN_W - 16, 1, "F1-F4", Colour.Grey, 1, 1)
end

-- ─── Страница 1: Фигуры ─────────────────────────────────────────────────────

local function page_shapes(dt)
    draw_title("Page 1: Shapes")

    -- Линия
    app:DrawLine(10, 20, 80, 60, Colour.White)
    app:DrawString(10, 62, "Line", Colour.Grey, 1, 1)

    -- Прямоугольники
    app:DrawRectangle(90, 20, 50, 35, Colour.Cyan)
    app:FillRectangle(150, 20, 50, 35, Colour.DarkCyan)
    app:DrawString(90, 57, "Rect  FillRect", Colour.Grey, 1, 1)

    -- Треугольники
    app:DrawTriangle(10, 80, 40, 130, 80, 80, Colour.Green)
    app:FillTriangle(90, 80, 130, 130, 170, 80, Colour.DarkGreen)
    app:DrawString(10, 133, "Tri   FillTri", Colour.Grey, 1, 1)

    -- Окружности
    app:DrawCircle(30, 170, 20, Colour.Red)
    app:FillCircle(90, 170, 20, Colour.DarkRed)
    app:DrawString(10, 193, "Circle  FillCircle", Colour.Grey, 1, 1)

    -- Эллипсы
    app:DrawEllipse(160, 150, 50, 25, Colour.Magenta)
    app:FillEllipse(260, 150, 40, 20, Colour.DarkMagenta)
    app:DrawString(155, 178, "Ellipse  FillEllipse", Colour.Grey, 1, 1)

    -- Сетка точек (тест Draw)
    for x = 210, 310, 5 do
        for y = 20, 120, 5 do
            local col = hue_to_pixel(((x + y) / 130 + hue_t) % 1)
            app:Draw(x, y, col)
        end
    end
    app:DrawString(210, 122, "Draw pixels", Colour.Grey, 1, 1)

    draw_border(Colour.DarkGrey)
end

-- ─── Страница 2: Текст и Pixel ──────────────────────────────────────────────

local function page_text_pixel(dt)
    draw_title("Page 2: Text & Pixel ops")

    -- DrawString масштабы
    app:DrawString(4, 14, "Scale x1", Colour.White,  1, 1)
    app:DrawString(4, 24, "Scale x2", Colour.Yellow, 2, 2)
    app:DrawString(4, 42, "x3",       Colour.Orange, 3, 3)

    -- Pixel арифметика (r/g/b/a)
    local base  = Pixel:new(128, 64, 32, 255)
    local added = base + 64          -- прибавить скаляр к RGB
    local multp = base * 2           -- умножить на скаляр
    local mixed = base + Pixel:new(0, 128, 200, 0)  -- сложение пикселей

    app:DrawString(4, 70, "Pixel ops:", Colour.Grey, 1, 1)

    local px = 4
    local labels = {
        { col = base,  label = "base(128,64,32)" },
        { col = added, label = "base+64" },
        { col = multp, label = "base*2" },
        { col = mixed, label = "base+Pixel" },
    }
    for i, v in ipairs(labels) do
        local rx = px + (i-1) * 78
        app:FillRectangle(rx, 80, 70, 14, v.col)
        app:DrawString(rx, 96, v.label, Colour.Grey, 1, 1)
    end

    -- Lerp
    app:DrawString(4, 108, "Lerp (White→Blue):", Colour.Grey, 1, 1)
    for i = 0, 30 do
        local t = i / 30
        local col = Colour.White:Lerp(Colour.Blue, t)
        app:FillRectangle(4 + i * 10, 118, 10, 14, col)
    end

    -- tostring
    app:DrawString(4, 136, tostring(base), Colour.Lime, 1, 1)

    -- Colour таблица
    app:DrawString(4, 148, "Colour palette:", Colour.Grey, 1, 1)
    local palette = {
        Colour.Red,   Colour.Green,   Colour.Blue,    Colour.Yellow,
        Colour.Cyan,  Colour.Magenta, Colour.White,   Colour.Orange,
        Colour.Gold,  Colour.Pink,    Colour.Lime,    Colour.Violet,
        Colour.Brown, Colour.Beige,   Colour.Maroon,  Colour.Purple,
    }
    for i, col in ipairs(palette) do
        local rx = 4 + ((i-1) % 8) * 39
        local ry = 158 + math.floor((i-1) / 8) * 16
        app:FillRectangle(rx, ry, 37, 14, col)
    end

    -- Vector tostring
    local vi = Vector2i:new(3, 7)
    local vf = Vector2f:new(1.5, -2.5)
    local vd = Vector2d:new(math.pi, math.exp(1))
    app:DrawString(4, 194, "vi=" .. tostring(vi), Colour.Cyan,    1, 1)
    app:DrawString(4, 204, "vf=" .. tostring(vf), Colour.Yellow,  1, 1)
    app:DrawString(4, 214, "vd=" .. tostring(vd), Colour.Magenta, 1, 1)
    app:DrawString(4, 224, "vi+vf=" .. tostring(vi + vi), Colour.White, 1, 1)

    draw_border(Colour.DarkGrey)
end

-- ─── Страница 3: Ввод и таймер ──────────────────────────────────────────────

local function page_input(dt)
    draw_title("Page 3: Input & Timer")

    -- FPS и delta
    local fps_str = string.format("FPS: %d   dt: %.4f s", app:GetFPS(), dt)
    app:DrawString(4, 14, fps_str, Colour.Green, 1, 1)

    -- Мышь
    local mx = app:GetMouseX()
    local my = app:GetMouseY()
    app:DrawString(4, 26, string.format("Mouse: (%d, %d)", mx, my), Colour.Yellow, 1, 1)
    app:DrawString(4, 36, string.format("Wheel delta: %d", app:GetMouseWheelDelta()), Colour.Yellow, 1, 1)

    local lb = app:GetMouse(Button.Left)
    local rb = app:GetMouse(Button.Right)
    local mb = app:GetMouse(Button.Wheel)
    app:DrawString(4, 46, string.format(
        "LMB: held=%s pressed=%s  RMB: held=%s  MMB: held=%s",
        tostring(lb.held), tostring(lb.pressed),
        tostring(rb.held), tostring(mb.held)
    ), Colour.White, 1, 1)

    -- Крестик мыши
    if mx >= 0 and mx < SCREEN_W and my >= 0 and my < SCREEN_H then
        app:DrawLine(mx - 5, my, mx + 5, my, Colour.Red)
        app:DrawLine(mx, my - 5, mx, my + 5, Colour.Red)
    end

    -- Клавиши WASD + стрелки
    local keys_info = {
        { key=Key.W,     name="W"    },
        { key=Key.A,     name="A"    },
        { key=Key.S,     name="S"    },
        { key=Key.D,     name="D"    },
        { key=Key.Up,    name="Up"   },
        { key=Key.Down,  name="Down" },
        { key=Key.Left,  name="Left" },
        { key=Key.Right, name="Right"},
        { key=Key.Space, name="Space"},
        { key=Key.Enter, name="Enter"},
        { key=Key.LeftShift, name="LShift"},
        { key=Key.LeftControl, name="LCtrl"},
    }
    app:DrawString(4, 60, "Keys:", Colour.Grey, 1, 1)
    for i, info in ipairs(keys_info) do
        local ks = app:GetKey(info.key)
        local col = Colour.DarkGrey
        if     ks.pressed  then col = Colour.Green
        elseif ks.held     then col = Colour.Yellow
        elseif ks.released then col = Colour.Red
        end
        local rx = 4  + ((i-1) % 4) * 78
        local ry = 70 + math.floor((i-1) / 4) * 14
        app:FillRectangle(rx, ry, 75, 12, col)
        app:DrawString(rx + 2, ry + 2, info.name, Colour.White, 1, 1)
    end

    -- CapsLock
    local caps_col = app:IsCaps() and Colour.Green or Colour.DarkGrey
    app:FillRectangle(4, 126, 60, 12, caps_col)
    app:DrawString(6, 128, "CAPS", Colour.White, 1, 1)

    -- Захват текста
    app:DrawString(4, 144, "Text capture (T to toggle):", Colour.Grey, 1, 1)
    if app:GetKey(Key.T).pressed then
        app:CaptureText(not app:IsCapturingText())
    end
    local capture_col = app:IsCapturingText() and Colour.DarkGreen or Colour.DarkRed
    app:FillRectangle(4, 154, SCREEN_W - 8, 14, capture_col)
    app:DrawString(6, 157, "> " .. app:GetCapturedText(), Colour.White, 1, 1)

    -- Движущийся шарик (WASD)
    app:DrawString(4, 174, "Ball (WASD/arrows):", Colour.Grey, 1, 1)
    local speed = 80
    local bdir = Vector2f:new(0, 0)
    if app:GetKey(Key.W).held or app:GetKey(Key.Up).held    then bdir.y = bdir.y - 1 end
    if app:GetKey(Key.S).held or app:GetKey(Key.Down).held  then bdir.y = bdir.y + 1 end
    if app:GetKey(Key.A).held or app:GetKey(Key.Left).held  then bdir.x = bdir.x - 1 end
    if app:GetKey(Key.D).held or app:GetKey(Key.Right).held then bdir.x = bdir.x + 1 end

    local area_x, area_y, area_w, area_h = 4, 184, SCREEN_W - 8, 48
    ball_pos = ball_pos + bdir * (speed * dt)
    ball_pos.x = math.max(area_x + ball_radius, math.min(area_x + area_w - ball_radius, ball_pos.x))
    ball_pos.y = math.max(area_y + ball_radius, math.min(area_y + area_h - ball_radius, ball_pos.y))

    app:FillRectangle(area_x, area_y, area_w, area_h, Pixel:new(20, 20, 40, 255))
    app:FillCircle(math.floor(ball_pos.x), math.floor(ball_pos.y), ball_radius, hue_to_pixel(hue_t))

    draw_border(Colour.DarkGrey)
end

-- ─── Страница 4: Векторная математика ───────────────────────────────────────

local function page_vectors(dt)
    draw_title("Page 4: Vector math")

    local vi1 = Vector2i:new(10, 3)
    local vi2 = Vector2i:new(4, 7)
    local vf1 = Vector2f:new(1.0, 0.0)
    local vf2 = Vector2f:new(0.0, 1.0)
    local vd1 = Vector2d:new(3.0, 4.0)

    -- Арифметика
    local rows = {
        { "vi1 = " .. tostring(vi1) },
        { "vi2 = " .. tostring(vi2) },
        { "vi1 + vi2 = " .. tostring(vi1 + vi2) },
        { "vi1 - vi2 = " .. tostring(vi1 - vi2) },
        { "vi1 * 3   = " .. tostring(vi1 * 3) },
        { "vi1 // 3  = " .. tostring(vi1 // 3) },
        { "-vi1      = " .. tostring(-vi1) },
        { "vi1 == vi1: " .. tostring(vi1 == vi1) },
        { "vi1 <  vi2: " .. tostring(vi1 < vi2) },
        { "" },
        { "vf1 = " .. tostring(vf1) },
        { "vf2 = " .. tostring(vf2) },
        { "Dot(vf1,vf2) = " .. string.format("%.4f", vf1:DotProduct(vf2)) },
        { "Cross(vf1,vf2)= " .. string.format("%.4f", vf1:CrossProduct(vf2)) },
        { "Angle(vf1,vf2)= " .. string.format("%.4f rad", vf1:Angle(vf2)) },
        { "Lerp(vf1,vf2,0.5)= " .. tostring(vf1:Lerp(vf2, 0.5)) },
        { "Norm(vf1)= " .. tostring(vf1:Norm()) },
        { "Perp(vf1)= " .. tostring(vf1:Perpendicular()) },
        { "" },
        { "vd1 = " .. tostring(vd1) },
        { "Length(vd1) = " .. string.format("%.4f", vd1:Length()) },
        { "Abs(-vd1) = " .. tostring((-vd1):Abs()) },
        { "ManhDist = " .. string.format("%.1f", vd1:ManhattanDistance(Vector2d:new(0,0))) },
    }

    for i, row in ipairs(rows) do
        local col = Colour.White
        if row[1]:sub(1, 2) == "vf" then col = Colour.Yellow
        elseif row[1]:sub(1, 2) == "vd" then col = Colour.Cyan
        elseif row[1] == "" then col = Colour.Black
        end
        app:DrawString(4, 14 + (i-1) * 9, row[1], col, 1, 1)
    end

    -- Анимированный вектор
    local angle = hue_t * math.pi * 2
    local origin = Vector2f:new(SCREEN_W - 50, SCREEN_H / 2)
    local tip = origin + Vector2f:new(math.cos(angle) * 35, math.sin(angle) * 35)
    app:DrawLine(
        math.floor(origin.x), math.floor(origin.y),
        math.floor(tip.x),    math.floor(tip.y),
        hue_to_pixel(hue_t)
    )
    app:DrawCircle(math.floor(origin.x), math.floor(origin.y), 3, Colour.White)
    app:DrawCircle(math.floor(tip.x),    math.floor(tip.y),    3, Colour.Yellow)
    app:DrawString(SCREEN_W - 95, SCREEN_H - 20, "Rotating vec", Colour.Grey, 1, 1)

    draw_border(Colour.DarkGrey)
end

-- ─── Callbacks ──────────────────────────────────────────────────────────────

function OnCreate()
    ball_pos = Vector2f:new(SCREEN_W / 2, SCREEN_H / 2)
    return true
end

function OnUpdate(dt)
    -- Смена страниц
    if app:GetKey(Key.F1).pressed then page = 1 end
    if app:GetKey(Key.F2).pressed then page = 2 end
    if app:GetKey(Key.F3).pressed then page = 3 end
    if app:GetKey(Key.F4).pressed then page = 4 end

    -- Обновление общих таймеров
    hue_t  = (hue_t + dt * 0.2) % 1.0
    blink_t = blink_t + dt

    -- Очистка
    app:Clear(Pixel:new(8, 8, 16, 255))

    -- Рендер текущей страницы
    if     page == 1 then page_shapes(dt)
    elseif page == 2 then page_text_pixel(dt)
    elseif page == 3 then page_input(dt)
    elseif page == 4 then page_vectors(dt)
    end

    -- Статус-бар (всегда внизу)
    local fps_col = app:GetFPS() >= 55 and Colour.Green or Colour.Red
    local bar = string.format("FPS:%d  Page:%d/4  [F1-F4]", app:GetFPS(), page)
    app:FillRectangle(0, SCREEN_H - 9, SCREEN_W, 9, Pixel:new(0, 0, 0, 200))
    app:DrawString(2, SCREEN_H - 8, bar, fps_col, 1, 1)

    return true
end

function CreateApp()
    return {
        title       = "defGameEngine Lua Wrapper Demo",
        size        = { SCREEN_W, SCREEN_H, 3, 3 },
        full_screen = false,
        vsync       = false,
        dirty_pixel = false,
    }
end
