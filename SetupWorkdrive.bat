@echo off

cd /D "%~dp0"

IF exist "P:\DayZDoom\" (
	echo Removing existing link P:\DayZDoom
	rmdir "P:\DayZDoom\"
)

echo Creating link P:\DayZDoom
mklink /J "P:\DayZDoom\" "%cd%\DayZDoom\"

echo Done