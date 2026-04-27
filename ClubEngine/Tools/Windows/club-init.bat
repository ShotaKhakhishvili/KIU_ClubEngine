@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..\..") do set "ENGINE_ROOT=%%~fI"
set "TOOLS_DIR=%ENGINE_ROOT%\Tools\Windows"

if not exist "%ENGINE_ROOT%\Saved" mkdir "%ENGINE_ROOT%\Saved"

if not exist "%ENGINE_ROOT%\Saved\ProjectRegistry.json" (
    > "%ENGINE_ROOT%\Saved\ProjectRegistry.json" echo {
    >> "%ENGINE_ROOT%\Saved\ProjectRegistry.json" echo     "projects": []
    >> "%ENGINE_ROOT%\Saved\ProjectRegistry.json" echo }
)

setx CLUBENGINE_ROOT "%ENGINE_ROOT%" >nul

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$tools = '%TOOLS_DIR%';" ^
  "$old = [Environment]::GetEnvironmentVariable('PATH','User');" ^
  "$parts = @($old -split ';' | Where-Object { $_ -and ($_ -notmatch 'ClubEngine\\Tools') });" ^
  "$parts += $tools;" ^
  "$new = (($parts | Select-Object -Unique) -join ';');" ^
  "[Environment]::SetEnvironmentVariable('PATH', $new, 'User')"

echo ClubEngine initialized.
echo Root: %ENGINE_ROOT%
echo Tools: %TOOLS_DIR%
echo Restart terminal.

endlocal
