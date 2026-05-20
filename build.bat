@echo off
echo "Building source file into local directory ./bin/CPUGraphics.exe..."
g++ -I./include ./src/Main.cpp -L./lib -lSDL2main -lSDL2 -o ./bin/CPUGraphicsEngine.exe
echo "Build completed."
pause