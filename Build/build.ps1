$ErrorActionPreference = "Stop"

$GXX        = "C:\msys64\mingw64\bin\g++.exe"
$MSYS2      = "C:\msys64\mingw64"
$ROOT       = "$PSScriptRoot\.."
$DGE_ENGINE = "$ROOT\..\defGameEngine\Engine"
$OUT        = "$ROOT\Build\Target\LuaWrapper.exe"

# Убедимся что выходная директория существует
New-Item -ItemType Directory -Force "$ROOT\Build\Target" | Out-Null

$DEFINES = @(
    "-DDGE_PLATFORM_GL",
    "-DDGE_PLATFORM_GLFW3"
)

$FLAGS = @(
    "-std=c++20",
    "-O1",
    "-Wall",
    "-Wno-unknown-pragmas"
)

$INCLUDES = @(
    "-I$MSYS2\include",
    "-I$DGE_ENGINE\Include",
    "-I$DGE_ENGINE\Vendor\stb"
)

$LIBDIRS = @("-L$MSYS2\lib")

$LIBS = @(
    "-lglfw3",
    "-lopengl32",
    "-lglu32",
    "-lgdi32",
    "-luser32",
    "-lkernel32",
    "-llua",
    "-lwinmm",
    "-limm32",
    "-lversion",
    "-lsetupapi"
)

$ENGINE_SOURCES = @(
    "$DGE_ENGINE\Sources\defGameEngine.cpp",
    "$DGE_ENGINE\Sources\Console.cpp",
    "$DGE_ENGINE\Sources\Graphic.cpp",
    "$DGE_ENGINE\Sources\InputHandler.cpp",
    "$DGE_ENGINE\Sources\Layer.cpp",
    "$DGE_ENGINE\Sources\Pixel.cpp",
    "$DGE_ENGINE\Sources\Platform.cpp",
    "$DGE_ENGINE\Sources\PlatformGL.cpp",
    "$DGE_ENGINE\Sources\PlatformGLFW3.cpp",
    "$DGE_ENGINE\Sources\Sprite.cpp",
    "$DGE_ENGINE\Sources\State.cpp",
    "$DGE_ENGINE\Sources\StbImage.cpp",
    "$DGE_ENGINE\Sources\Texture.cpp",
    "$DGE_ENGINE\Sources\Timer.cpp",
    "$DGE_ENGINE\Sources\Window.cpp"
)

$WRAPPER_SOURCES = @(
    "$ROOT\Sources\Main.cpp",
    "$ROOT\Sources\Constants.cpp",
    "$ROOT\Sources\Types.cpp",
    "$ROOT\Sources\Utils.cpp"
)

$ALL_SOURCES = $WRAPPER_SOURCES + $ENGINE_SOURCES
$ALL_ARGS   = $FLAGS + $DEFINES + $INCLUDES + $LIBDIRS + @("-o", $OUT) + $ALL_SOURCES + $LIBS

Write-Host "Building LuaWrapper..." -ForegroundColor Cyan
Write-Host "Output: $OUT" -ForegroundColor Cyan

$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.FileName = $GXX
$psi.Arguments = ($ALL_ARGS | ForEach-Object { if ($_ -match ' ') { "`"$_`"" } else { $_ } }) -join " "
$psi.RedirectStandardOutput = $true
$psi.RedirectStandardError  = $true
$psi.UseShellExecute = $false
$proc = [System.Diagnostics.Process]::Start($psi)
$stderr = $proc.StandardError.ReadToEnd()
$proc.WaitForExit()

if ($proc.ExitCode -ne 0) {
    Write-Host "BUILD FAILED" -ForegroundColor Red
    Write-Host $stderr
    exit 1
}

if ($stderr) { Write-Host $stderr -ForegroundColor Yellow }

# Copy required runtime DLLs next to the exe
$dlls = @("glfw3.dll","lua55.dll","libstdc++-6.dll","libgcc_s_seh-1.dll","libwinpthread-1.dll")
foreach ($dll in $dlls) {
    $src = "$MSYS2\bin\$dll"
    if (Test-Path $src) { Copy-Item $src "$ROOT\Build\Target\" -Force }
}

Write-Host "BUILD OK -> $OUT" -ForegroundColor Green
