@echo off
setlocal enabledelayedexpansion

if "%CLUBENGINE_ROOT%"=="" (
    echo ERROR: CLUBENGINE_ROOT is not set. Run club-init first.
    exit /b 1
)

set "PROJECT_ROOT=%CD%"
for %%I in ("%PROJECT_ROOT%") do set "PROJECT_NAME=%%~nxI"
set "REGISTRY=%CLUBENGINE_ROOT%\Saved\ProjectRegistry.json"

if not exist "%CLUBENGINE_ROOT%\Saved" mkdir "%CLUBENGINE_ROOT%\Saved"

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$registry = '%REGISTRY%';" ^
  "$name = '%PROJECT_NAME%';" ^
  "$root = '%PROJECT_ROOT:\=/%';" ^
  "$id = $name + '@' + $root;" ^
  "if (Test-Path $registry) { $data = Get-Content $registry -Raw | ConvertFrom-Json } else { $data = [pscustomobject]@{} }" ^
  "if (-not ($data.PSObject.Properties.Name -contains 'projects')) { $data | Add-Member -NotePropertyName projects -NotePropertyValue @() }" ^
  "$projects = @($data.projects | Where-Object { $_.rootPath -ne $root });" ^
  "$projects += [pscustomobject]@{ id = $id; name = $name; rootPath = $root };" ^
  "$out = [pscustomobject]@{ projects = $projects };" ^
  "$out | ConvertTo-Json -Depth 8 | Set-Content $registry"

if errorlevel 1 exit /b 1

echo Registered project "%PROJECT_NAME%"
echo Path: %PROJECT_ROOT%

endlocal
