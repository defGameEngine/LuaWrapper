--[[-----------------------------------------------------------------
     HUD — helper module.
     Connection in script:
         local _DIR = (debug.getinfo(1,'S').source:sub(2)):match("(.*[/\\])") or "./"
         local Hud  = dofile(_DIR .. "HUD.lua")
         local hud  = Hud:new(Dge)
 -----------------------------------------------------------------]]

local Hud = {}
Hud.__index = Hud

function Hud:new(dge)
    return setmetatable({ _dge = dge }, Hud)
end

function Hud:Panel(x, y, w, h, fill, border)
    self._dge:FillRectangle(x, y, w, h, fill)
    if border then
        self._dge:DrawRectangle(x, y, w, h, border)
    end
end

function Hud:Bar(x, y, w, h, value, max_value, fill, bg, border)
    bg = bg or Pixel:new(30, 30, 30, 200)

    self._dge:FillRectangle(x, y, w, h, bg)

    local ratio  = math.max(0, math.min(value, max_value)) / max_value
    local fill_w = math.floor(w * ratio)

    if fill_w > 0 then
        self._dge:FillRectangle(x, y, fill_w, h, fill)
    end

    if border then
        self._dge:DrawRectangle(x, y, w, h, border)
    end
end

function Hud:Label(x, y, text, col, scale, bg)
    scale = scale or 1

    if bg then
        local tw = #text * 8 * scale
        local th = 8 * scale
        self._dge:FillRectangle(x - 1, y - 1, tw + 2, th + 2, bg)
    end

    self._dge:DrawString(x, y, text, col, scale, scale)
end

return Hud
