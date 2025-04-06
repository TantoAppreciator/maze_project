@echo off
if exist "build\maze_generator.exe" (
    "build\maze_generator.exe"
) else (
    echo Executable not found. Please run build.bat first
    exit /b 1
)