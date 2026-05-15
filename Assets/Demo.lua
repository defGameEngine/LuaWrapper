local SCREEN_W = 320
local SCREEN_H = 240
local PAGE_COUNT = 4

local page        = 1
local ball_pos    = Vector2f:new(SCREEN_W / 2, SCREEN_H / 2)
local ball_vel    = Vector2f:new(60, 45)
local ball_radius = 8

local hue_t   = 0.0
local blink_t = 0.0

local function lerp(a, b, t)
    return a + (b - a) * t
end

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

local function draw_border(col)
    Dge:DrawRectangle(0, 0, SCREEN_W - 1, SCREEN_H - 1, col)
end

local function draw_title(text)
    Dge:FillRectangle(0, 0, SCREEN_W, 10, Colour.DarkBlue)
    Dge:DrawString(2, 1, text, Colour.Yellow, 1, 1)
    Dge:DrawString(SCREEN_W - 16, 1, "F1-F4", Colour.Grey, 1, 1)
end

local function page_shapes(dt)
    draw_title("Page 1: Shapes")

    Dge:DrawLine(10, 20, 80, 60, Colour.White)
    Dge:DrawString(10, 62, "Line", Colour.Grey, 1, 1)

    Dge:DrawRectangle(90, 20, 50, 35, Colour.Cyan)
    Dge:FillRectangle(150, 20, 50, 35, Colour.DarkCyan)
    Dge:DrawString(90, 57, "Rect  FillRect", Colour.Grey, 1, 1)

    Dge:DrawTriangle(10, 80, 40, 130, 80, 80, Colour.Green)
    Dge:FillTriangle(90, 80, 130, 130, 170, 80, Colour.DarkGreen)
    Dge:DrawString(10, 133, "Tri   FillTri", Colour.Grey, 1, 1)

    Dge:DrawCircle(30, 170, 20, Colour.Red)
    Dge:FillCircle(90, 170, 20, Colour.DarkRed)
    Dge:DrawString(10, 193, "Circle  FillCircle", Colour.Grey, 1, 1)

    Dge:DrawEllipse(160, 150, 50, 25, Colour.Magenta)
    Dge:FillEllipse(260, 150, 40, 20, Colour.DarkMagenta)
    Dge:DrawString(155, 178, "Ellipse  FillEllipse", Colour.Grey, 1, 1)

    for x = 210, 310, 5 do
        for y = 20, 120, 5 do
            local col = hue_to_pixel(((x + y) / 130 + hue_t) % 1)
            Dge:Draw(x, y, col)
        end
    end
    Dge:DrawString(210, 122, "Draw pixels", Colour.Grey, 1, 1)

    draw_border(Colour.DarkGrey)
end



local function page_text_pixel(dt)
    draw_title("Page 2: Text & Pixel ops")

    
    Dge:DrawString(4, 14, "Scale x1", Colour.White,  1, 1)
    Dge:DrawString(4, 24, "Scale x2", Colour.Yellow, 2, 2)
    Dge:DrawString(4, 42, "x3",       Colour.Orange, 3, 3)

    
    local base  = Pixel:new(128, 64, 32, 255)
    local added = base + 64          
    local multp = base * 2           
    local mixed = base + Pixel:new(0, 128, 200, 0)  

    Dge:DrawString(4, 70, "Pixel ops:", Colour.Grey, 1, 1)

    local px = 4
    local labels = {
        { col = base,  label = "base(128,64,32)" },
        { col = added, label = "base+64" },
        { col = multp, label = "base*2" },
        { col = mixed, label = "base+Pixel" },
    }
    for i, v in ipairs(labels) do
        local rx = px + (i-1) * 78
        Dge:FillRectangle(rx, 80, 70, 14, v.col)
        Dge:DrawString(rx, 96, v.label, Colour.Grey, 1, 1)
    end

    
    Dge:DrawString(4, 108, "Lerp (White→Blue):", Colour.Grey, 1, 1)
    for i = 0, 30 do
        local t = i / 30
        local col = Colour.White:Lerp(Colour.Blue, t)
        Dge:FillRectangle(4 + i * 10, 118, 10, 14, col)
    end

    
    Dge:DrawString(4, 136, tostring(base), Colour.Lime, 1, 1)

    
    Dge:DrawString(4, 148, "Colour palette:", Colour.Grey, 1, 1)
    local palette = {
        Colour.Red,   Colour.Green,   Colour.Blue,    Colour.Yellow,
        Colour.Cyan,  Colour.Magenta, Colour.White,   Colour.Orange,
        Colour.Gold,  Colour.Pink,    Colour.Lime,    Colour.Violet,
        Colour.Brown, Colour.Beige,   Colour.Maroon,  Colour.Purple,
    }
    for i, col in ipairs(palette) do
        local rx = 4 + ((i-1) % 8) * 39
        local ry = 158 + math.floor((i-1) / 8) * 16
        Dge:FillRectangle(rx, ry, 37, 14, col)
    end

    
    local vi = Vector2i:new(3, 7)
    local vf = Vector2f:new(1.5, -2.5)
    local vd = Vector2d:new(math.pi, math.exp(1))
    Dge:DrawString(4, 194, "vi=" .. tostring(vi), Colour.Cyan,    1, 1)
    Dge:DrawString(4, 204, "vf=" .. tostring(vf), Colour.Yellow,  1, 1)
    Dge:DrawString(4, 214, "vd=" .. tostring(vd), Colour.Magenta, 1, 1)
    Dge:DrawString(4, 224, "vi+vf=" .. tostring(vi + vi), Colour.White, 1, 1)

    draw_border(Colour.DarkGrey)
end



local function page_input(dt)
    draw_title("Page 3: Input & Timer")

    
    local fps_str = string.format("FPS: %d   dt: %.4f s", Dge:GetFPS(), dt)
    Dge:DrawString(4, 14, fps_str, Colour.Green, 1, 1)

    
    local mx = Dge:GetMouseX()
    local my = Dge:GetMouseY()
    Dge:DrawString(4, 26, string.format("Mouse: (%d, %d)", mx, my), Colour.Yellow, 1, 1)
    Dge:DrawString(4, 36, string.format("Wheel delta: %d", Dge:GetMouseWheelDelta()), Colour.Yellow, 1, 1)

    local lb = Dge:GetMouse(Button.Left)
    local rb = Dge:GetMouse(Button.Right)
    local mb = Dge:GetMouse(Button.Wheel)
    Dge:DrawString(4, 46, string.format(
        "LMB: held=%s pressed=%s  RMB: held=%s  MMB: held=%s",
        tostring(lb.held), tostring(lb.pressed),
        tostring(rb.held), tostring(mb.held)
    ), Colour.White, 1, 1)

    
    if mx >= 0 and mx < SCREEN_W and my >= 0 and my < SCREEN_H then
        Dge:DrawLine(mx - 5, my, mx + 5, my, Colour.Red)
        Dge:DrawLine(mx, my - 5, mx, my + 5, Colour.Red)
    end

    + стрелки
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
    Dge:DrawString(4, 60, "Keys:", Colour.Grey, 1, 1)
    for i, info in ipairs(keys_info) do
        local ks = Dge:GetKey(info.key)
        local col = Colour.DarkGrey
        if     ks.pressed  then col = Colour.Green
        elseif ks.held     then col = Colour.Yellow
        elseif ks.released then col = Colour.Red
        end
        local rx = 4  + ((i-1) % 4) * 78
        local ry = 70 + math.floor((i-1) / 4) * 14
        Dge:FillRectangle(rx, ry, 75, 12, col)
        Dge:DrawString(rx + 2, ry + 2, info.name, Colour.White, 1, 1)
    end

    
    local caps_col = Dge:IsCaps() and Colour.Green or Colour.DarkGrey
    Dge:FillRectangle(4, 126, 60, 12, caps_col)
    Dge:DrawString(6, 128, "CAPS", Colour.White, 1, 1)

    
    Dge:DrawString(4, 144, "Text capture (T to toggle):", Colour.Grey, 1, 1)
    if Dge:GetKey(Key.T).pressed then
        Dge:CaptureText(not Dge:IsCapturingText())
    end
    local capture_col = Dge:IsCapturingText() and Colour.DarkGreen or Colour.DarkRed
    Dge:FillRectangle(4, 154, SCREEN_W - 8, 14, capture_col)
    Dge:DrawString(6, 157, "> " .. Dge:GetCapturedText(), Colour.White, 1, 1)

    
    Dge:DrawString(4, 174, "Ball (WASD/arrows):", Colour.Grey, 1, 1)
    local speed = 80
    local bdir = Vector2f:new(0, 0)
    if Dge:GetKey(Key.W).held or Dge:GetKey(Key.Up).held    then bdir.y = bdir.y - 1 end
    if Dge:GetKey(Key.S).held or Dge:GetKey(Key.Down).held  then bdir.y = bdir.y + 1 end
    if Dge:GetKey(Key.A).held or Dge:GetKey(Key.Left).held  then bdir.x = bdir.x - 1 end
    if Dge:GetKey(Key.D).held or Dge:GetKey(Key.Right).held then bdir.x = bdir.x + 1 end

    local area_x, area_y, area_w, area_h = 4, 184, SCREEN_W - 8, 48
    ball_pos = ball_pos + bdir * (speed * dt)
    ball_pos.x = math.max(area_x + ball_radius, math.min(area_x + area_w - ball_radius, ball_pos.x))
    ball_pos.y = math.max(area_y + ball_radius, math.min(area_y + area_h - ball_radius, ball_pos.y))

    Dge:FillRectangle(area_x, area_y, area_w, area_h, Pixel:new(20, 20, 40, 255))
    Dge:FillCircle(math.floor(ball_pos.x), math.floor(ball_pos.y), ball_radius, hue_to_pixel(hue_t))

    draw_border(Colour.DarkGrey)
end



local function page_vectors(dt)
    draw_title("Page 4: Vector math")

    local vi1 = Vector2i:new(10, 3)
    local vi2 = Vector2i:new(4, 7)
    local vf1 = Vector2f:new(1.0, 0.0)
    local vf2 = Vector2f:new(0.0, 1.0)
    local vd1 = Vector2d:new(3.0, 4.0)

    
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
        Dge:DrawString(4, 14 + (i-1) * 9, row[1], col, 1, 1)
    end

    
    local angle = hue_t * math.pi * 2
    local origin = Vector2f:new(SCREEN_W - 50, SCREEN_H / 2)
    local tip = origin + Vector2f:new(math.cos(angle) * 35, math.sin(angle) * 35)
    Dge:DrawLine(
        math.floor(origin.x), math.floor(origin.y),
        math.floor(tip.x),    math.floor(tip.y),
        hue_to_pixel(hue_t)
    )
    Dge:DrawCircle(math.floor(origin.x), math.floor(origin.y), 3, Colour.White)
    Dge:DrawCircle(math.floor(tip.x),    math.floor(tip.y),    3, Colour.Yellow)
    Dge:DrawString(SCREEN_W - 95, SCREEN_H - 20, "Rotating vec", Colour.Grey, 1, 1)

    draw_border(Colour.DarkGrey)
end



function OnCreate()
    ball_pos = Vector2f:new(SCREEN_W / 2, SCREEN_H / 2)
    return true
end

function OnUpdate(dt)
    
    if Dge:GetKey(Key.F1).pressed then page = 1 end
    if Dge:GetKey(Key.F2).pressed then page = 2 end
    if Dge:GetKey(Key.F3).pressed then page = 3 end
    if Dge:GetKey(Key.F4).pressed then page = 4 end

    
    hue_t  = (hue_t + dt * 0.2) % 1.0
    blink_t = blink_t + dt

    
    Dge:Clear(Pixel:new(8, 8, 16, 255))

    
    if     page == 1 then page_shapes(dt)
    elseif page == 2 then page_text_pixel(dt)
    elseif page == 3 then page_input(dt)
    elseif page == 4 then page_vectors(dt)
    end

    
    local fps_col = Dge:GetFPS() >= 55 and Colour.Green or Colour.Red
    local bar = string.format("FPS:%d  Page:%d/4  [F1-F4]", Dge:GetFPS(), page)
    Dge:FillRectangle(0, SCREEN_H - 9, SCREEN_W, 9, Pixel:new(0, 0, 0, 200))
    Dge:DrawString(2, SCREEN_H - 8, bar, fps_col, 1, 1)

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
