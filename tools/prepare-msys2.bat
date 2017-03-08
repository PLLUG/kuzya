@echo on
curl -O %KUZYA_MSYS2_DISTRIB_PACKAGE_URL%/%KUZYA_MSYS2_DISTRIB_PACKAGE_NAME%
call %KUZYA_MSYS64_ROOT%\msys2_shell.bat -mingw64 -here bsdtar xf %KUZYA_MSYS2_DISTRIB_PACKAGE_NAME%
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell.cmd exit
echo Waiting 10 seconds to be sure that MSYS2 initialization finished...
sleep 10
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell.cmd -mingw64 -here  ./../tools/update-msys2.sh
echo Updating MSYS2, installing gcc...
:wait-for-gcc-installed
sleep 1
if not exist %KUZYA_PACKAGE_ROOT%\mingw64\bin\g++.exe goto wait-for-gcc-installed
