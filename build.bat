@echo off
echo ============================================================
echo LBYARCH Build Script
echo ============================================================
echo.

REM Find Visual Studio installation
set "VSPATH=C:\Program Files\Microsoft Visual Studio\18\Community"

if not exist "%VSPATH%" (
    echo ERROR: Visual Studio not found at expected location
    echo Please open Developer Command Prompt and run: msbuild LBYARCH.sln /p:Configuration=Debug /p:Platform=x64
    pause
    exit /b 1
)

echo Setting up Visual Studio environment...
call "%VSPATH%\Common7\Tools\VsDevCmd.bat"

echo.
echo Building Debug configuration...
msbuild LBYARCH.sln /p:Configuration=Debug /p:Platform=x64 /v:minimal

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Debug build completed!
    echo.
) else (
    echo.
    echo [FAILED] Debug build failed!
    pause
    exit /b 1
)

echo Building Release configuration...
msbuild LBYARCH.sln /p:Configuration=Release /p:Platform=x64 /v:minimal

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Release build completed!
    echo.
) else (
    echo.
    echo [FAILED] Release build failed!
    pause
    exit /b 1
)

echo ============================================================
echo Build Complete!
echo ============================================================
echo.
echo Executables created:
echo   Debug:   x64\Debug\LBYARCH.exe
echo   Release: x64\Release\LBYARCH.exe
echo.

set /p RUN_DEBUG="Do you want to run the Debug version now? (y/n): "
if /i "%RUN_DEBUG%"=="y" (
    echo.
    echo Running Debug version...
    echo.
    x64\Debug\LBYARCH.exe
    echo.
)

set /p RUN_RELEASE="Do you want to run the Release version now? (y/n): "
if /i "%RUN_RELEASE%"=="y" (
    echo.
    echo Running Release version...
    echo.
    x64\Release\LBYARCH.exe
)

echo.
pause
