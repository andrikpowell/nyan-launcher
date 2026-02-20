@ECHO OFF
setlocal

set "REPO=andrikpowell/nyan-doom"

set "DEST=%~dp0"
if not "%~1"=="" set "DEST=%~1"
if "%DEST:~-1%"=="\" set "DEST=%DEST:~0,-1%"
echo Destination folder is "%DEST%"

:: Get latest version
for /f %%i in ('powershell -NoProfile -command "(Invoke-WebRequest https://api.github.com/repos/%REPO%/releases | ConvertFrom-Json)[0].tag_name"') do set "TAG=%%i"
if errorlevel 1 goto :fail

echo Latest Nyan Doom version is %TAG%
set "VERSION=%TAG:~1%"

:: File name
set "v_win64=nyan-doom-%VERSION%-win-x64"

set "TMPDIR=%temp%\nyan-doom-temp"
mkdir "%TMPDIR%" 2>nul

:: Download zip
echo Downloading latest release
powershell -NoProfile -Command "Invoke-WebRequest -OutFile '%TMPDIR%\%v_win64%.zip' -Uri 'https://github.com/%REPO%/releases/download/%TAG%/%v_win64%.zip'"
if errorlevel 1 goto :fail
if not exist "%TMPDIR%\%v_win64%.zip" goto :fail

:: Extract zip
echo Extracting files
powershell -NoProfile -Command "Expand-Archive -Force '%TMPDIR%\%v_win64%.zip' '%TMPDIR%'"
if errorlevel 1 goto :fail

:: Copy to destination folder
echo Copying files to destination
robocopy "%TMPDIR%\%v_win64%" "%DEST%" * /nfl /ndl /njh /njs /nc /ns /np
set "RC=%ERRORLEVEL%"
if %RC% GEQ 8 goto :fail

:: Delete temp files
rmdir /s /q "%TMPDIR%" 2>nul

echo Nyan Doom %VERSION% has been installed successfully!
exit /b 0

:: Failure? - do pause
:fail
echo.
echo Update failed.
echo.

:: Best-effort cleanup
if defined TMPDIR if exist "%TMPDIR%" rmdir /s /q "%TMPDIR%" 2>nul

pause
exit /b 1
