@echo off
curl -O %KUZYA_MSYS2_DISTRIB_PACKAGE_URL%/%KUZYA_MSYS2_DISTRIB_PACKAGE_NAME%
"C:\Program Files\7-Zip\7z.exe" x %KUZYA_MSYS2_DISTRIB_PACKAGE_NAME% -so | 7z x -aoa -si -ttar
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell exit
echo Waiting 10 seconds to be sure that MSYS2 initialization finished...
sleep 10
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell -mingw64 -here  ./../tools/update-msys2.sh
echo Updating MSYS2, installing gcc...
:wait-for-gcc-installed
sleep 1
if not exist %KUZYA_PACKAGE_ROOT%\msys64\mingw64\bin\g++.exe goto wait-for-gcc-installed
