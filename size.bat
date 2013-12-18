@echo off
set size=0
for /f "skip=2 tokens=1,2" %%i in ('%*') do (
if "%%i"==".text" set /a size = size + %%j
if "%%i"==".ARM.exidx" set /a size = size + %%j
if "%%i"==".relocate" set /a size = size + %%j
)
echo size=%size%
