@echo off
for /r %%f in (*.cpp *.h) do clang-format -i "%%f"

:: redo this in python later