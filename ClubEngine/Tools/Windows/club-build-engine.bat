@echo off
setlocal

if "%CLUBENGINE_ROOT%"=="" (
    echo ERROR: CLUBENGINE_ROOT is not set.
    echo Run club-init first.
    exit /b 1
)

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Debug"

set "PLATFORM=Win64"
set "VERSION_FILE=%TEMP%\clubengine_version.txt"

powershell -NoProfile -ExecutionPolicy Bypass -Command "$text = Get-Content $env:CLUBENGINE_ROOT\CMakeLists.txt -Raw; $m = [regex]::Match($text, 'project\s*\(\s*ClubEngine\s+VERSION\s+([0-9]+\.[0-9]+\.[0-9]+)'); if (-not $m.Success) { exit 1 }; $m.Groups[1].Value | Set-Content '%VERSION_FILE%'"

if errorlevel 1 (
    echo ERROR: Could not read ClubEngine version from CMakeLists.txt.
    exit /b 1
)

set /p ENGINE_VERSION=<"%VERSION_FILE%"
del "%VERSION_FILE%" >nul 2>nul

set "BUILD_DIR=%CLUBENGINE_ROOT%\Build\%PLATFORM%\%CONFIG%\%ENGINE_VERSION%"

echo Building ClubEngine
echo Config:  %CONFIG%
echo Version: %ENGINE_VERSION%
echo Build:   %BUILD_DIR%
echo.

cmake -S "%CLUBENGINE_ROOT%" -B "%BUILD_DIR%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%"
if errorlevel 1 exit /b 1

echo.
echo Engine build complete.
echo Output:
echo %CLUBENGINE_ROOT%\Binaries\%PLATFORM%\%CONFIG%\%ENGINE_VERSION%

endlocal