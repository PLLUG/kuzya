rem ### Following environment variables should be set up for script to work
rem KUZYA_BUILD_ROOT (example: c:\projects\kuzya)
rem KUZYA_NSIS_ROOT (example: C:\Program Files (x86)\NSIS)
rem KUZYA_7Z_PATH (example: C:\Program Files\7-Zip\7z.exe)

rem ### Variables
set KUZYA_QSCINTILLA_VERSION=2.9.4
set KUZYA_MSYS2_DISTRIB_PACKAGE_NAME=msys2-x86_64-latest.tar.xz
set KUZYA_MSYS2_DISTRIB_PACKAGE_URL=http://repo.msys2.org/distrib

rem ### Preparing build dir
set KUZYA_PACKAGE_ROOT=%KUZYA_BUILD_ROOT%\package
mkdir %KUZYA_PACKAGE_ROOT%
cd %KUZYA_PACKAGE_ROOT%

rem ### Copy QScintilla DLL
xcopy %KUZYA_BUILD_ROOT%\3rdparty\QScintilla\%KUZYA_QSCINTILLA_VERSION%\qscintilla2.dll %KUZYA_BUILD_ROOT%\trunc\bin

windeployqt --release --compiler-runtime --angle %KUZYA_BUILD_ROOT%\trunc\bin

rem ### Preparing MSYS2 for distribution
curl -O %KUZYA_MSYS2_DISTRIB_PACKAGE_URL%/%KUZYA_MSYS2_DISTRIB_PACKAGE_NAME%
"%KUZYA_7Z_PATH%" x %KUZYA_MSYS2_DISTRIB_PACKAGE_NAME% -so | "%KUZYA_7Z_PATH%" x -aoa -si -ttar
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell.cmd exit
echo Waiting 10 seconds to be sure that MSYS2 initialization finished...
sleep 10
call %KUZYA_PACKAGE_ROOT%\msys64\msys2_shell.cmd -mingw64 -here  ./../tools/create-custom-repo.sh
echo Creating local MSYS2 repo...
:wait-for-local-repo
sleep 1
if not exist ./msys2-dist-repository/custom.db.tar.gz goto wait-for-local-repo

rem ### Prepare installing package
"%KUZYA_NSIS_ROOT%\makensis.exe" /X"SetCompressor /SOLID /FINAL lzma" /DKUZYA_BUILD_ROOT=%KUZYA_BUILD_ROOT% /DKUZYA_PACKAGE_ROOT=%KUZYA_PACKAGE_ROOT% %KUZYA_BUILD_ROOT%/tools/kuzya-win64-full.nsi
