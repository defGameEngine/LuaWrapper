---@diagnostic disable: undefined-global

--[[-----------------------------------------------------------------
     Copyright 2026 defini7 and J-Starling. All rights reserved.
     Licensed under the GNU General Public License v3.0.
     See LICENSE file in the project root for license information.
 -----------------------------------------------------------------]]

newoption
{
    trigger = "msys2",
    value = "PATH",
    description = "Path to MSYS2 MinGW64 root (default: C:/msys64/mingw64)"
}

workspace "LuaWrapper"
    startproject "LuaWrapper"

    configurations
    {
        "Debug",
        "Release"
    }

    filter "system:windows"
        architecture "x64"

    filter {}

OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
MSYS2 = _OPTIONS["msys2"] or "C:/msys64/mingw64"

include "Vendor/defGameEngine/Engine/Vendor/glfw"

project "Engine"
    location "Vendor/defGameEngine/Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    defines { "DGE_PLATFORM_GLFW3" }

    files
    {
        "Vendor/defGameEngine/Engine/Include/*.hpp",
        "Vendor/defGameEngine/Engine/Sources/*.cpp"
    }

    removefiles
    {
        "Vendor/defGameEngine/Engine/Sources/Utils.cpp",
        "Vendor/defGameEngine/Engine/Sources/PlatformEmscripten.cpp",
        "Vendor/defGameEngine/Engine/Sources/PlatformSDL3.cpp"
    }

    includedirs
    {
        "Vendor/defGameEngine/Engine/Include",
        "Vendor/defGameEngine/Engine/Sources",
        "Vendor/defGameEngine/Engine/Vendor/stb",
        "Vendor/defGameEngine/Engine/Vendor/glfw/include"
    }

    links { "GLFW3" }

    filter "system:windows"
        links
        {
            "opengl32", "glu32", "gdi32", "user32", "kernel32",
            "winmm", "imm32", "version", "setupapi"
        }
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter {}

    postbuildcommands
    {
        "{COPY} %{cfg.buildtarget.relpath} \"%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/LuaWrapper/\""
    }

project "LuaWrapper"
    location "."
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    defines
    {
        "DGE_PLATFORM_GLFW3",
        "SOL_ALL_SAFETIES_ON=1"
    }

    files
    {
        "Include/*.hpp",
        "Sources/*.cpp"
    }

    includedirs
    {
        "Include",
        "Vendor/defGameEngine/Engine/Include",
        "Vendor/defGameEngine/Engine/Vendor/stb",
        "Vendor/sol2/include",
        MSYS2 .. "/include"
    }

    libdirs { MSYS2 .. "/lib" }

    links
    {
        "Engine",
        "GLFW3",
        "lua",
        "opengl32", "glu32", "gdi32", "user32", "kernel32",
        "winmm", "imm32", "version", "setupapi"
    }

    filter "system:windows"
        systemversion "latest"

        postbuildcommands
        {
            "{COPY} " .. MSYS2 .. "/bin/lua55.dll %{cfg.targetdir}",
            "{COPY} " .. MSYS2 .. "/bin/glfw3.dll %{cfg.targetdir}",
            "{COPY} " .. MSYS2 .. "/bin/libstdc++-6.dll %{cfg.targetdir}",
            "{COPY} " .. MSYS2 .. "/bin/libgcc_s_seh-1.dll %{cfg.targetdir}",
            "{COPY} " .. MSYS2 .. "/bin/libwinpthread-1.dll %{cfg.targetdir}"
        }

    -- sol2 template instantiations require at least -O1 to avoid a
    -- g++/MinGW-PE COMDAT bug where COMDAT sections are silently omitted at -O0
    filter "configurations:Debug"
        symbols "On"
        buildoptions { "-O1" }

    filter "configurations:Release"
        optimize "On"

    filter {}
