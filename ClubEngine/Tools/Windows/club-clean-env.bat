@echo off

echo Cleaning ClubEngine environment...

reg delete HKCU\Environment /v CLUBENGINE_ROOT /f >nul 2>nul

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$old = [Environment]::GetEnvironmentVariable('PATH','User');" ^
  "if ($old) {" ^
  "  $parts = $old -split ';' | Where-Object { $_ -and ($_ -notmatch 'ClubEngine\\Tools') -and ($_ -notmatch 'KIU_ClubEngine\\ClubEngine\\Tools') };" ^
  "  [Environment]::SetEnvironmentVariable('PATH', ($parts -join ';'), 'User')" ^
  "}"

echo Done. Restart terminal.
