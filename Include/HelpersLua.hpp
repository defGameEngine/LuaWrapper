/*-----------------------------------------------------------------
 *  Copyright 2026 defini7 and J-Starling. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#ifndef HELPERS_LUA_HPP
#define HELPERS_LUA_HPP

const char* g_HelpersSource = R"(function AddVectorOperations(t)
	function t.__add(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x + rhs, lhs.y + rhs)
		end
		return t:new(lhs.x + rhs.x, lhs.y + rhs.y)
	end

	function t.__sub(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x - rhs, lhs.y - rhs)
		end
		return t:new(lhs.x - rhs.x, lhs.y - rhs.y)
	end

	function t.__mul(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x * rhs, lhs.y * rhs)
		end
		return t:new(lhs.x * rhs.x, lhs.y * rhs.y)
	end

	function t.__div(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x / rhs, lhs.y / rhs)
		end
		return t:new(lhs.x / rhs.x, lhs.y / rhs.y)
	end

	function t.__idiv(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x // rhs, lhs.y // rhs)
		end
		return t:new(lhs.x // rhs.x, lhs.y // rhs.y)
	end

	function t.__mod(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x % rhs, lhs.y % rhs)
		end
		return t:new(lhs.x % rhs.x, lhs.y % rhs.y)
	end

	function t.__pow(lhs, rhs)
		if type(rhs) == "number" then
			return t:new(lhs.x ^ rhs, lhs.y ^ rhs)
		end
		return t:new(lhs.x ^ rhs.x, lhs.y ^ rhs.y)
	end

	function t.__unm(lhs)
		return t:new(-lhs.x, -lhs.y)
	end

	function t.__tostring(lhs)
		return lhs:ToString()
	end

	function t.__eq(lhs, rhs)
		if type(rhs) == "number" then
			return lhs.x == rhs and lhs.y == rhs
		end
		return lhs.x == rhs.x and lhs.y == rhs.y
	end

	function t.__lt(lhs, rhs)
		if type(rhs) == "number" then
			return lhs.x < rhs and lhs.y < rhs
		end
		return lhs.x < rhs.x and lhs.y < rhs.y
	end

	function t.__le(lhs, rhs)
		if type(rhs) == "number" then
			return lhs.x <= rhs and lhs.y <= rhs
		end
		return lhs.x <= rhs.x and lhs.y <= rhs.y
	end
end

AddVectorOperations(Vector2i)
AddVectorOperations(Vector2f)
AddVectorOperations(Vector2d)

function clamp(n, min, max)
	if n > max then return max end
	if n < min then return min end
	return n
end

function Pixel.__tostring(lhs)
	return lhs:ToString()
end

function Pixel.__add(lhs, rhs)
	if type(rhs) == "number" then
		return Pixel:new(clamp(lhs.r + rhs, 0, 255), clamp(lhs.g + rhs, 0, 255), clamp(lhs.b + rhs, 0, 255), lhs.a)
	end
	return Pixel:new(clamp(lhs.r + rhs.r, 0, 255), clamp(lhs.g + rhs.g, 0, 255), clamp(lhs.b + rhs.b, 0, 255), clamp(lhs.a + rhs.a, 0, 255))
end

function Pixel.__sub(lhs, rhs)
	if type(rhs) == "number" then
		return Pixel:new(clamp(lhs.r - rhs, 0, 255), clamp(lhs.g - rhs, 0, 255), clamp(lhs.b - rhs, 0, 255), lhs.a)
	end
	return Pixel:new(clamp(lhs.r - rhs.r, 0, 255), clamp(lhs.g - rhs.g, 0, 255), clamp(lhs.b - rhs.b, 0, 255), clamp(lhs.a - rhs.a, 0, 255))
end

function Pixel.__mul(lhs, rhs)
	if type(rhs) == "number" then
		return Pixel:new(clamp(lhs.r * rhs, 0, 255), clamp(lhs.g * rhs, 0, 255), clamp(lhs.b * rhs, 0, 255), lhs.a)
	end
	return Pixel:new(clamp(lhs.r * rhs.r, 0, 255), clamp(lhs.g * rhs.g, 0, 255), clamp(lhs.b * rhs.b, 0, 255), clamp(lhs.a * rhs.a, 0, 255))
end

function Pixel.__div(lhs, rhs)
	if type(rhs) == "number" then
		return Pixel:new(clamp(lhs.r / rhs, 0, 255), clamp(lhs.g / rhs, 0, 255), clamp(lhs.b / rhs, 0, 255), lhs.a)
	end
	return Pixel:new(clamp(lhs.r / rhs.r, 0, 255), clamp(lhs.g / rhs.g, 0, 255), clamp(lhs.b / rhs.b, 0, 255), clamp(lhs.a / rhs.a, 0, 255))
end

function Pixel.__idiv(lhs, rhs)
	if type(rhs) == "number" then
		return Pixel:new(clamp(lhs.r // rhs, 0, 255), clamp(lhs.g // rhs, 0, 255), clamp(lhs.b // rhs, 0, 255), lhs.a)
	end
	return Pixel:new(clamp(lhs.r // rhs.r, 0, 255), clamp(lhs.g // rhs.g, 0, 255), clamp(lhs.b // rhs.b, 0, 255), clamp(lhs.a // rhs.a, 0, 255))
end)";

#endif
