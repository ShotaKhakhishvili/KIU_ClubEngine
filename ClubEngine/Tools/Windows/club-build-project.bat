@echo off
setlocal

if "%CLUBENGINE_ROOT%"=="" (
    echo ERROR: CLUBENGINE_ROOT is not set.
    echo Run club-init first.
    exit /b 1
)

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Debug"

set "PROJECT_ROOT=%CD%"
for %%I in ("%PROJECT_ROOT%") do set "PROJECT_NAME=%%~nxI"

if not exist "%PROJECT_ROOT%\CMakeLists.txt" (
    echo ERROR: No CMakeLists.txt found. Run this from project root.
    exit /b 1
)

set "PLATFORM=Win64"
set "CLUBPROJECT_FILE="

for %%F in ("%PROJECT_ROOT%\*.clubproject") do (
    set "CLUBPROJECT_FILE=%%F"
)

if "%CLUBPROJECT_FILE%"=="" (
    echo ERROR: No .clubproject file found in project root.
    exit /b 1
)

set "VERSION_FILE=%TEMP%\clubengine_project_assoc.txt"

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$json = Get-Content '%CLUBPROJECT_FILE%' | Out-String | ConvertFrom-Json;" ^
  "$json.EngineAssociation | Set-Content '%VERSION_FILE%'"

if errorlevel 1 (
    echo ERROR: Failed to read EngineAssociation from .clubproject.
    exit /b 1
)

set /p ENGINE_ASSOCIATION=<"%VERSION_FILE%"
del "%VERSION_FILE%" >nul 2>nul

if "%ENGINE_ASSOCIATION%"=="" (
    echo ERROR: EngineAssociation is empty.
    exit /b 1
)

set "ENGINE_BIN_ROOT=%CLUBENGINE_ROOT%\Binaries\%PLATFORM%\%CONFIG%"

if not exist "%ENGINE_BIN_ROOT%" (
    echo ERROR: No built engines found for %PLATFORM%/%CONFIG%.
    echo Run: club-build-engine %CONFIG%
    exit /b 1
)

set "FOUND_ENGINE="
for /d %%D in ("%ENGINE_BIN_ROOT%\%ENGINE_ASSOCIATION%.*") do (
    set "FOUND_ENGINE=%%D"
)

if "%FOUND_ENGINE%"=="" (
    echo ERROR: No built engine matching EngineAssociation "%ENGINE_ASSOCIATION%" found.
    echo Expected something like:
    echo   %ENGINE_BIN_ROOT%\%ENGINE_ASSOCIATION%.x
    exit /b 1
)

for %%I in ("%FOUND_ENGINE%") do set "ENGINE_VERSION=%%~nxI"

set "BUILD_DIR=%PROJECT_ROOT%\Build\%PLATFORM%\%CONFIG%\%ENGINE_VERSION%"

echo Building project "%PROJECT_NAME%"
echo Config:            %CONFIG%
echo EngineAssociation: %ENGINE_ASSOCIATION%
echo EngineVersion:     %ENGINE_VERSION%
echo Build:             %BUILD_DIR%
echo.

cmake -S "%PROJECT_ROOT%" -B "%BUILD_DIR%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%" --target "%PROJECT_NAME%"
if errorlevel 1 exit /b 1

echo.
echo Project build complete.
echo Output:
echo %PROJECT_ROOT%\Binaries\%PLATFORM%\%CONFIG%\%ENGINE_VERSION%

endlocal