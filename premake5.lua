---@diagnostic disable: undefined-global

--[[-----------------------------------------------------------------
     Copyright 2026 defini7 and J-Starling. All rights reserved.
     Licensed under the GNU General Public License v3.0.
     See LICENSE file in the project root for license information.
 -----------------------------------------------------------------]]

workspace "LuaWrapper"
    startproject "LuaWrapper"

    configurations
    {
        "Debug",
        "Release"
    }

    filter "system:windows or system:linux"
        architecture "x64"

    filter "system:macosx"
        architecture "ARM64"

    filter {}

defines { "DGE_NEED_AUDIO" }

OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Vendor/defGameEngine/Engine/Vendor/glfw"

project "Lua"
    location "Vendor/lua"
    kind "SharedLib"
    language "C"
    
    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    staticruntime "On"

    defines { "LUA_BUILD_AS_DLL" }

    files
    {
        "Vendor/lua/l*.c"
    }

    removefiles
    {
        "Vendor/lua/ltests.c"
    }

    includedirs
    {
        "Vendor/lua"
    }

    filter "system:windows"
        links { "kernel32" }
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

project "Engine"
    location "Vendor/defGameEngine/Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    staticruntime "On"

    defines { "DGE_PLATFORM_GLFW3" }

    pchheader "Pch.hpp"
    pchsource "Vendor/defGameEngine/Engine/Sources/Pch.cpp"

    files
    {
        "Vendor/defGameEngine/Engine/Include/*.hpp",
        "Vendor/defGameEngine/Engine/Sources/*.cpp"
    }

    removefiles
    {
        "Vendor/defGameEngine/Engine/Include/PlatformEmscripten.hpp",
        "Vendor/defGameEngine/Engine/Sources/PlatformEmscripten.cpp",
        "Vendor/defGameEngine/Engine/Include/PlatformSDL3.hpp",
        "Vendor/defGameEngine/Engine/Sources/PlatformSDL3.cpp",
        "Vendor/defGameEngine/Engine/Sources/Utils.cpp"
    }

    includedirs
    {
        "Vendor/defGameEngine/Engine/Include",
        "Vendor/defGameEngine/Engine/Sources",
        "Vendor/defGameEngine/Engine/Vendor/stb",
        "Vendor/defGameEngine/Engine/Vendor/glfw/include",
        "Vendor/defGameEngine/Engine/Vendor/miniaudio",
        "Vendor/sol2/include",
        "Vendor/lua"
    }

    links { "GLFW3" }

    filter "system:windows"
        links
        {
            "opengl32", "glu32", "gdi32", "user32", "kernel32",
            "winmm", "imm32", "version", "setupapi"
        }
        systemversion "latest"

    filter "system:linux"
        links
        {
            "GL", "GLU", "glut", "GLEW", "X11",
            "Xxf86vm", "Xrandr", "pthread", "Xi", "dl",
            "Xinerama", "Xcursor", "m"
        }

    filter "system:macosx"
        links
        {
            "Metal.framework", "QuartzCore.framework",
            "Cocoa.framework", "OpenGL.framework",
            "IOKit.framework", "CoreVideo.framework"
        }

    filter {}

    postbuildcommands
    {
        "{COPY} %{cfg.buildtarget.relpath} \"%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/LuaWrapper/\""
    }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter {}

project "LuaWrapper"
    location "."
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/Build/Target/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/Obj/" .. OUTPUT_DIR .. "/%{prj.name}")

    staticruntime "On"

    defines
    {
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
        "Vendor/defGameEngine/Engine/Vendor/glfw/include",
        "Vendor/defGameEngine/Engine/Vendor/miniaudio",
        "Vendor/sol2/include",
        "Vendor/lua"
    }

    links
    {
        "Engine",
        "GLFW3",
        "Lua"
    }

    filter "system:windows"
        links
        {
            "opengl32", "glu32", "gdi32", "user32", "kernel32",
            "winmm", "imm32", "version", "setupapi"
        }
        systemversion "latest"
        buildoptions { "/bigobj" }

    filter "system:linux"
        links
        {
            "GL", "GLU", "glut", "GLEW", "X11",
            "Xxf86vm", "Xrandr", "pthread", "Xi", "dl",
            "Xinerama", "Xcursor", "m"
        }

    filter "system:macosx"
        links
        {
            "Metal.framework", "QuartzCore.framework",
            "Cocoa.framework", "OpenGL.framework",
            "IOKit.framework", "CoreVideo.framework"
        }

    filter {}

    postbuildcommands
    {
        "{COPY} %{cfg.buildtarget.relpath} \"%{wks.location}/Build/Target/\""
    }

    -- sol2 template instantiations require at least -O1 to avoid a
    -- g++/MinGW-PE COMDAT bug where COMDAT sections are silently omitted at -O0
    filter "configurations:Debug"
        symbols "On"
        filter "system:windows"
            buildoptions { "-O1" }
        filter {}

    filter "configurations:Release"
        optimize "On"

    filter {}
