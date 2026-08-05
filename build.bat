@echo off

cd /d "%~dp0"

set "BUILDFILENAME=CPUGraphicsEngine"

if "%~1" NEQ "" (
	set "BUILDFILENAME=%~1"
)

echo "Building source file into local directory ./bin/%BUILDFILENAME%.exe..."
g++ -I./include ./src/Main.cpp ./src/Engine.cpp -L./lib -lSDL2main -lSDL2 -o ./bin/%BUILDFILENAME%.exe
echo "Build completed."
cd "./bin"
"%BUILDFILENAME%.exe"
pause