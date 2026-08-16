@echo off
setlocal EnableExtensions
cd /d "%~dp0"

if "%VCPKG_ROOT%"=="" (
  echo [ERROR] VCPKG_ROOT is not defined.
  exit /b 1
)

if "%SKYRIM_DIR%"=="" set "SKYRIM_DIR=C:\Games\Steam\steamapps\common\Skyrim Special Edition"
if not exist "%SKYRIM_DIR%\Papyrus Compiler\PapyrusCompiler.exe" (
  echo [ERROR] PapyrusCompiler.exe was not found under:
  echo   %SKYRIM_DIR%
  echo.
  set /p "SKYRIM_DIR=Enter your Skyrim Special Edition folder: "
)

set "PAPYRUS_COMPILER=%SKYRIM_DIR%\Papyrus Compiler\PapyrusCompiler.exe"
set "PAPYRUS_FLAGS=%SKYRIM_DIR%\Data\Source\Scripts\TESV_Papyrus_Flags.flg"
set "PAPYRUS_VANILLA=%SKYRIM_DIR%\Data\Source\Scripts"
set "PAPYRUS_SRC=%CD%\papyrus"
set "PAPYRUS_STUBS=%CD%\compiler_stubs"
set "PAPYRUS_OUT=%CD%\package\Scripts"

if not exist "%PAPYRUS_COMPILER%" (
  echo [ERROR] PapyrusCompiler.exe not found:
  echo   %PAPYRUS_COMPILER%
  exit /b 1
)
if not exist "%PAPYRUS_FLAGS%" (
  echo [ERROR] TESV_Papyrus_Flags.flg not found:
  echo   %PAPYRUS_FLAGS%
  exit /b 1
)

if exist build rmdir /s /q build

if exist "package\SKSE\Plugins\BodyHairSliders.dll" del /q "package\SKSE\Plugins\BodyHairSliders.dll"
if exist "package\Scripts\BodyHairSliders.pex" del /q "package\Scripts\BodyHairSliders.pex"
if exist "package\Scripts\ak_all_in_one_script.pex" del /q "package\Scripts\ak_all_in_one_script.pex"

if not exist "package\SKSE\Plugins" mkdir "package\SKSE\Plugins"
if not exist "package\Scripts" mkdir "package\Scripts"
if not exist "package\Scripts\Source" mkdir "package\Scripts\Source"
if not exist "dist" mkdir "dist"

echo [1/5] Configuring C++...
cmake -S . -B build ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" || exit /b 1

echo [2/5] Building C++...
cmake --build build --config Release || exit /b 1

for /f "delims=" %%F in ('dir /b /s "build\Release\BodyHairSliders.dll" 2^>nul') do copy /y "%%F" "package\SKSE\Plugins\BodyHairSliders.dll" >nul
if not exist "package\SKSE\Plugins\BodyHairSliders.dll" (
  echo [ERROR] BodyHairSliders.dll was not found after build.
  exit /b 1
)

echo [3/5] Compiling Papyrus API declarations...
"%PAPYRUS_COMPILER%" "%PAPYRUS_SRC%\BodyHairSliders.psc" -f="%PAPYRUS_FLAGS%" -i="%PAPYRUS_SRC%;%PAPYRUS_STUBS%;%PAPYRUS_VANILLA%" -o="%PAPYRUS_OUT%"
if errorlevel 1 exit /b 1

echo [4/5] Compiling RaceMenu frontend...
"%PAPYRUS_COMPILER%" "%PAPYRUS_SRC%\ak_all_in_one_script.psc" -f="%PAPYRUS_FLAGS%" -i="%PAPYRUS_SRC%;%PAPYRUS_STUBS%;%PAPYRUS_VANILLA%" -o="%PAPYRUS_OUT%"
if errorlevel 1 exit /b 1

copy /Y "%PAPYRUS_SRC%\BodyHairSliders.psc" "package\Scripts\Source\BodyHairSliders.psc" >nul
copy /Y "%PAPYRUS_SRC%\ak_all_in_one_script.psc" "package\Scripts\Source\ak_all_in_one_script.psc" >nul

echo [5/5] Creating Vortex archive in dist...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop'; $zip='dist/BodyHairSliders-v0.1.0.zip'; if(Test-Path $zip){Remove-Item $zip}; Compress-Archive -Path 'package/*' -DestinationPath $zip -CompressionLevel Optimal"
if errorlevel 1 exit /b 1

echo.
echo Build complete:
echo   package\SKSE\Plugins\BodyHairSliders.dll
echo   package\Scripts\BodyHairSliders.pex
echo   package\Scripts\ak_all_in_one_script.pex
echo   dist\BodyHairSliders-v0.1.0.zip
endlocal
