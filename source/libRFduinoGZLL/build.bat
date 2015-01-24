@echo off

set arduino=C:\arduino-1.5.8
set nordic="C:\Nordic Semiconductor\nRF51 SDK_v4.2.0.25053\Nordic\nrf51822"

if not exist %nordic% (
echo Error: The Nordic SDK is required to build libRFduinoGZLL.
goto end
)

cd %arduino%\hardware\arduino\RFduino\source\libRFduinoGZLL

if not exist _build\nul mkdir _build
if exist _build\* del /q _build\*

set tools=%arduino%\hardware\tools\gcc-arm-none-eabi-4.8.3-2014q1
set RFduino=%arduino%\hardware\arduino\RFduino

set gcc=%tools%\bin\arm-none-eabi-gcc
set ar=%tools%\bin\arm-none-eabi-ar

set includes=-I%nordic%\Include
set includes=%includes% -I%nordic%\Include\gzll
set includes=%includes% -I%nordic%\Include\gcc

set cflags=-Os -w -mcpu=cortex-m0 -mthumb -DNRF51 -DBOARD_PCA10001 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fno-builtin %includes%

echo compiling...

echo libRFduinoGZLL.c
%gcc% %cflags% -c -o _build\libRFduinoGZLL.o libRFduinoGZLL.c

echo creating library...

set objs=_build\libRFduinoGZLL.o

cd _build
%ar% -x %nordic%\lib\gzll\gcc\gzll_gcc.a
cd ..
set objs=%objs% _build\nrf_assert_internal.o
set objs=%objs% _build\nrf_channel_manager.o
set objs=%objs% _build\nrf_fifo.o
set objs=%objs% _build\nrf_flywheel.o
set objs=%objs% _build\nrf_gzll.o
set objs=%objs% _build\nrf_impala.o
set objs=%objs% _build\nrf_link_core.o
set objs=%objs% _build\nrf_notification_manager.o
set objs=%objs% _build\nrf_packet_manager.o
set objs=%objs% _build\nrf_state_machine.o

for %%f in (%objs%) do %ar% rcs _build/libRFduinoGZLL.a %%f

echo copying libray and header to variants...
copy _build\libRFduinoGZLL.a %RFduino%\variants\RFduino\libRFduinoGZLL.a
copy libRFduinoGZLL.h %RFduino%\variants\RFduino\libRFduinoGZLL.h

:end
pause
