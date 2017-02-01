curl -O %KUZYA_MSYS2_DISTRIB_PACKAGE_URL%/%KUZYA_MSYS2_DISTRIB_PACKAGE_NAME%
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell.cmd -mingw64 -here bsdtar xf %KUZYA_MSYS2_DISTRIB_PACKAGE_NAME%
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell.cmd exit
echo Waiting 10 seconds to be sure that MSYS2 initialization finished...
sleep 10
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell.cmd -mingw64 -here  ./../tools/create-custom-repo.sh
echo Creating local MSYS2 repo...
:wait-for-local-repo
sleep 1
if not exist ./msys2-dist-repository/custom.db.tar.gz goto wait-for-local-repo