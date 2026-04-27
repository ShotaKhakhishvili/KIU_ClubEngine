@echo off
setlocal enabledelayedexpansion

if "%CLUBENGINE_ROOT%"=="" (
    echo ERROR: CLUBENGINE_ROOT is not set.
    echo Run club-init first.
    exit /b 1
)

if "%~1"=="" (
    echo Usage: club-new-project ^<ProjectPath^> ^<EngineAssociation^>
    echo Example: club-new-project Game1 0.1
    exit /b 1
)

if "%~2"=="" (
    echo ERROR: Missing EngineAssociation.
    echo Example: club-new-project Game1 0.1
    exit /b 1
)

set "PROJECT_ROOT=%~f1"
set "ENGINE_ASSOCIATION=%~2"

for %%I in ("%PROJECT_ROOT%") do set "PROJECT_NAME=%%~nxI"

set "TEMPLATE_ROOT=%CLUBENGINE_ROOT%\Templates\Project"

if not exist "%TEMPLATE_ROOT%" (
    echo ERROR: Project template not found:
    echo %TEMPLATE_ROOT%
    exit /b 1
)

if exist "%PROJECT_ROOT%" (
    echo ERROR: Project folder already exists:
    echo %PROJECT_ROOT%
    exit /b 1
)

set "PLATFORM=Win64"
set "CONFIG=Debug"
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

echo Creating project "%PROJECT_NAME%"
echo Path:              %PROJECT_ROOT%
echo EngineAssociation: %ENGINE_ASSOCIATION%
echo Template:          %TEMPLATE_ROOT%
echo Found engine:      %FOUND_ENGINE%
echo.

xcopy "%TEMPLATE_ROOT%" "%PROJECT_ROOT%" /E /I /Y >nul
if errorlevel 1 (
    echo ERROR: Failed to copy template.
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$root = '%PROJECT_ROOT%';" ^
  "$name = '%PROJECT_NAME%';" ^
  "$assoc = '%ENGINE_ASSOCIATION%';" ^
  "$items = Get-ChildItem -LiteralPath $root -Recurse -Force | Sort-Object { $_.FullName.Length } -Descending;" ^
  "foreach ($item in $items) {" ^
  "  $newName = $item.Name.Replace('[PROJ_NAME]', $name);" ^
  "  if ($newName -ne $item.Name) {" ^
  "    Rename-Item -LiteralPath $item.FullName -NewName $newName;" ^
  "  }" ^
  "}" ^
  "$files = Get-ChildItem -LiteralPath $root -Recurse -File -Force;" ^
  "foreach ($file in $files) {" ^
  "  $content = [System.IO.File]::ReadAllText($file.FullName);" ^
  "  $content = $content.Replace('[PROJ_NAME]', $name).Replace('[ENGINE_ASSOCIATION]', $assoc);" ^
  "  [System.IO.File]::WriteAllText($file.FullName, $content);" ^
  "}"

if errorlevel 1 (
    echo ERROR: Failed to replace placeholders.
    exit /b 1
)

pushd "%PROJECT_ROOT%"
call "%CLUBENGINE_ROOT%\Tools\Windows\club-register-project.bat"
if errorlevel 1 (
    popd
    echo ERROR: Project created, but registration failed.
    exit /b 1
)
popd

echo.
echo Project created and registered.
echo To build:
echo   cd /d "%PROJECT_ROOT%"
echo   club-build-project Debug

endlocal