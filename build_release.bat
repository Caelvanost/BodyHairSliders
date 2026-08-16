@echo off
setlocal EnableExtensions
cd /d "%~dp0"

if not exist "VERSION" (
  echo [ERROR] VERSION file not found.
  exit /b 1
)
set /p BHS_VERSION=<VERSION
if "%BHS_VERSION%"=="" (
  echo [ERROR] VERSION file is empty.
  exit /b 1
)

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
set "ZIP_PATH=dist\BodyHairSliders-v%BHS_VERSION%-FOMOD.zip"

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
if not exist "package\BodyHairSliders.esp" (
  echo [ERROR] package\BodyHairSliders.esp is missing.
  exit /b 1
)

if exist build rmdir /s /q build

if exist "package\SKSE\Plugins\BodyHairSliders.dll" del /q "package\SKSE\Plugins\BodyHairSliders.dll"
if exist "package\SKSE\Plugins\BodyHairSliders\providers" rmdir /s /q "package\SKSE\Plugins\BodyHairSliders\providers"
if exist "package\Scripts\BodyHairSliders.pex" del /q "package\Scripts\BodyHairSliders.pex"
if exist "package\Scripts\BodyHairSlidersRaceMenu.pex" del /q "package\Scripts\BodyHairSlidersRaceMenu.pex"
if exist "package\Scripts\ak_all_in_one_script.pex" del /q "package\Scripts\ak_all_in_one_script.pex"
if exist "package\Scripts\Source\ak_all_in_one_script.psc" del /q "package\Scripts\Source\ak_all_in_one_script.psc"

if not exist "package\SKSE\Plugins" mkdir "package\SKSE\Plugins"
if not exist "package\Scripts" mkdir "package\Scripts"
if not exist "package\Scripts\Source" mkdir "package\Scripts\Source"
if not exist "dist" mkdir "dist"

echo Building BodyHairSliders v%BHS_VERSION%
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

echo [4/5] Compiling dedicated RaceMenu frontend...
"%PAPYRUS_COMPILER%" "%PAPYRUS_SRC%\BodyHairSlidersRaceMenu.psc" -f="%PAPYRUS_FLAGS%" -i="%PAPYRUS_SRC%;%PAPYRUS_STUBS%;%PAPYRUS_VANILLA%" -o="%PAPYRUS_OUT%"
if errorlevel 1 exit /b 1

copy /Y "%PAPYRUS_SRC%\BodyHairSliders.psc" "package\Scripts\Source\BodyHairSliders.psc" >nul
copy /Y "%PAPYRUS_SRC%\BodyHairSlidersRaceMenu.psc" "package\Scripts\Source\BodyHairSlidersRaceMenu.psc" >nul

echo [5/5] Creating and verifying FOMOD archive...
powershell -NoProfile -ExecutionPolicy Bypass -File "%CD%\build_fomod.ps1"
if errorlevel 1 exit /b 1

echo.
echo Build complete:
echo   package\BodyHairSliders.esp
echo   package\SKSE\Plugins\BodyHairSliders.dll
echo   package\Scripts\BodyHairSliders.pex
echo   package\Scripts\BodyHairSlidersRaceMenu.pex
echo   %ZIP_PATH%
endlocal
