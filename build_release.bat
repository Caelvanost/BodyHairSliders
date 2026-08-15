@echo off
setlocal
cd /d "%~dp0"

if "%VCPKG_ROOT%"=="" (
  echo [ERROR] VCPKG_ROOT is not defined.
  exit /b 1
)

if exist build rmdir /s /q build

echo [1/4] Configuring...
cmake -S . -B build ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
if errorlevel 1 exit /b %errorlevel%

echo [2/4] Building...
cmake --build build --config Release
if errorlevel 1 exit /b %errorlevel%

echo [3/4] Preparing package...
if not exist "package\SKSE\Plugins" mkdir "package\SKSE\Plugins"
if not exist "dist" mkdir "dist"

if exist "build\Release\BodyHairSliders.dll" (
  copy /y "build\Release\BodyHairSliders.dll" "package\SKSE\Plugins\BodyHairSliders.dll" >nul
) else if exist "build\BodyHairSliders.dll" (
  copy /y "build\BodyHairSliders.dll" "package\SKSE\Plugins\BodyHairSliders.dll" >nul
) else (
  echo [ERROR] BodyHairSliders.dll was not found after build.
  exit /b 1
)

echo [4/4] Creating Vortex archive in dist...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop'; $zip='dist/BodyHairSliders-v0.1.0.zip'; if(Test-Path $zip){Remove-Item $zip}; Compress-Archive -Path 'package/*' -DestinationPath $zip -CompressionLevel Optimal"
if errorlevel 1 exit /b 1

echo.
echo Done: dist\BodyHairSliders-v0.1.0.zip
endlocal
