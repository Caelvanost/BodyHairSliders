@echo off
setlocal
cd /d "%~dp0"

if "%VCPKG_ROOT%"=="" (
  echo [ERROR] VCPKG_ROOT is not defined.
  exit /b 1
)

echo [1/4] Configuring...
cmake --preset release || exit /b 1

echo [2/4] Building...
cmake --build --preset release || exit /b 1

echo [3/4] Preparing package...
if not exist "package\SKSE\Plugins" mkdir "package\SKSE\Plugins"
if not exist "dist" mkdir "dist"

for /f "delims=" %%F in ('dir /b /s "build\Release\NordicBodyHairSliders.dll" 2^>nul') do copy /y "%%F" "package\SKSE\Plugins\NordicBodyHairSliders.dll" >nul
if not exist "package\SKSE\Plugins\NordicBodyHairSliders.dll" (
  echo [ERROR] NordicBodyHairSliders.dll was not found after build.
  exit /b 1
)

echo [4/4] Creating Vortex archive in dist...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop'; $zip='dist/NordicBodyHairSliders-v0.1.0.zip'; if(Test-Path $zip){Remove-Item $zip}; Compress-Archive -Path 'package/*' -DestinationPath $zip -CompressionLevel Optimal"
if errorlevel 1 exit /b 1

echo.
echo Done: dist\NordicBodyHairSliders-v0.1.0.zip
endlocal
