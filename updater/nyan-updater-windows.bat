@ECHO OFF

set REPO=andrikpowell/nyan-doom

set DEST="%~dp0"
if not [%1]==[] set DEST="%1"
echo Destination folder is %DEST%

:: Get latest version
for /f %%i in ('powershell -command "(Invoke-WebRequest https://api.github.com/repos/%REPO%/releases | ConvertFrom-Json)[0].tag_name"') do set TAG=%%i
echo Latest Nyan Doom version is %TAG%
set VERSION=%TAG:~1%

:: File name
set v_win64=nyan-doom-%VERSION%-win64

mkdir "%temp%\nyan-doom-temp"

:: Download zip
echo Downloading latest release
powershell -command "Invoke-WebRequest -OutFile \"%temp%\nyan-doom-temp\%v_win64%.zip\" -Uri https://github.com/%REPO%/releases/download/%TAG%/%v_win64%.zip"

:: Extract zip
if exist "%temp%\nyan-doom-temp\%v_win64%.zip" (
  echo Extracting files
  powershell -command "Expand-Archive -Force \"%temp%\nyan-doom-temp\%v_win64%.zip\" \"%temp%\nyan-doom-temp\""
) else (
  echo "Error downloading update"
  del /q "%temp%\nyan-doom-temp\%v_win64%.zip"
  rmdir /s /q "%temp%\nyan-doom-temp\%v_win64%"
  pause
  exit
)

:: Copy to destination folder
echo Copying files to destination
robocopy "%temp%\nyan-doom-temp\%v_win64%" "%DEST% " * /nfl /ndl /njh /njs /nc /ns /np

:: Delete temp files
del /q "%temp%\nyan-doom-temp\%v_win64%.zip"
rmdir /s /q "%temp%\nyan-doom-temp\%v_win64%"

echo Nyan Doom %VERSION% has been installed succesfully!

pause
exit
