@echo off
for /r ./ %%f in (*.cpp;*.h;) do astyle --style=java -p -x "%%f"
for /r ./ %%f in (*.cpp.orig;*.h.orig;) do del "%%f"