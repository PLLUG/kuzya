echo Setting up environment...
set KUZYA_BUILD_ROOT=c:\projects\kuzya
set KUZYA_MSYS64_ROOT=c:\msys64
set KUZYA_QSCINTILLA_VERSION=2.9.4
set KUZYA_MSYS2_DISTRIB_PACKAGE_NAME=msys2-x86_64-latest.tar.xz
set KUZYA_MSYS2_DISTRIB_PACKAGE_URL=http://repo.msys2.org/distrib
set KUZYA_NSIS_ROOT=C:\Program Files (x86)\NSIS

echo Preparing build dir
set KUZYA_PACKAGE_ROOT=%KUZYA_BUILD_ROOT%\package
mkdir %KUZYA_PACKAGE_ROOT%
cd %KUZYA_PACKAGE_ROOT%

echo Copy QScintilla DLL
xcopy %KUZYA_BUILD_ROOT%\3rdparty\QScintilla\%KUZYA_QSCINTILLA_VERSION%\qscintilla2.dll %KUZYA_BUILD_ROOT%\bin\qscintilla2.dll

windeployqt --release --compiler-runtime --angle %KUZYA_BUILD_ROOT%\bin

echo Preparing MSYS2 for distribution
call %KUZYA_BUILD_ROOT%\tools\prepare-msys2.bat

echo Prepare installing package
"%KUZYA_NSIS_ROOT%\makensis.exe" /X"SetCompressor /SOLID /FINAL lzma" /DKUZYA_BUILD_ROOT=%KUZYA_BUILD_ROOT% /DKUZYA_PACKAGE_ROOT=%KUZYA_PACKAGE_ROOT% %KUZYA_BUILD_ROOT%/tools/kuzya-win64-full.nsi
