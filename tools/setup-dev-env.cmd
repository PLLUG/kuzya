@echo off
echo Detecting permissions...
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Success.
) else (
    echo Failure: This script should be run with administrative permissions.
    pause
    exit 1
)
set ChocolateyInstall=C:\KuzyaChocolateyInstall
echo Installing Chocolatey in %ChocolateyInstall%
powershell -NoProfile -ExecutionPolicy unrestricted -Command "(iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))) >$null 2>&1" && SET PATH="%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"
set PATH=%PATH%;%ChocolateyInstall%
echo Installing required software...
rem choco install vcbuildtools  -y
choco install visualcppbuildtools -y
choco install git-lfs.install -y
choco install nsis -y
pause