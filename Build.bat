@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
echo %SCRIPT_DIR%

rem Define Visual Studio versions to search for
set VS_VERSIONS=2022 2019 2017

:main
call GetVcpkg.bat
cd %SCRIPT_DIR%

rem Loop through each specified version and attempt to set the environment
for %%v in (%VS_VERSIONS%) do (
    call :find_and_call_vcvars %%v
    if %ERRORLEVEL%==0 (
	    echo Found Visual Studio, called vcvarsall.bat.
	    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM="%SCRIPT_DIR%bin\ninja.exe" -DCMAKE_TOOLCHAIN_FILE="%SCRIPT_DIR%bin\vcpkg\scripts\buildsystems\vcpkg.cmake" -G Ninja -S . -B .\build
	    cmake --build .\build --target dip -j 14
	    goto eof
	)
)

goto eof


rem Function to find and call vcvarsall.bat for a specific version
:find_and_call_vcvars
set VERSION=%1

set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\%VERSION%\Community\VC\Auxiliary\Build"
set VS_PATH_PROFESSIONAL="C:\Program Files (x86)\Microsoft Visual Studio\%VERSION%\Professional\VC\Auxiliary\Build"
set VS_PATH_ENTERPRISE="C:\Program Files (x86)\Microsoft Visual Studio\%VERSION%\Enterprise\VC\Auxiliary\Build"

set VS_PATH_64="C:\Program Files\Microsoft Visual Studio\%VERSION%\Community\VC\Auxiliary\Build"
set VS_PATH_PROFESSIONAL_64="C:\Program Files\Microsoft Visual Studio\%VERSION%\Professional\VC\Auxiliary\Build"
set VS_PATH_ENTERPRISE_64="C:\Program Files\Microsoft Visual Studio\%VERSION%\Enterprise\VC\Auxiliary\Build"

echo Checking %VS_PATH%
echo Checking %VS_PATH_PROFESSIONAL%
echo Checking %VS_PATH_ENTERPRISE%

echo Checking %VS_PATH_64%
echo Checking %VS_PATH_PROFESSIONAL_64%
echo Checking %VS_PATH_ENTERPRISE_64%

rem Check Community edition
if exist %VS_PATH% (
    if exist %VS_PATH%\vcvarsall.bat (
        echo Found Visual Studio %VERSION% Community, calling vcvarsall.bat...
        call %VS_PATH%\vcvarsall.bat x64
        exit /b 0
    )
)
if exist %VS_PATH_64% (
    if exist %VS_PATH_64%\vcvarsall.bat (
        echo Found Visual Studio %VERSION% Community, calling vcvarsall.bat...
        call %VS_PATH_64%\vcvarsall.bat x64
        exit /b 0
    )
)

rem Check Professional edition
if exist %VS_PATH_PROFESSIONAL% (
    if exist %VS_PATH_PROFESSIONAL%\vcvarsall.bat (
        echo Found Visual Studio %VERSION% Professional, calling vcvarsall.bat...
        call %VS_PATH_PROFESSIONAL%\vcvarsall.bat x64
        exit /b 0
    )
)
if exist %VS_PATH_PROFESSIONAL_64% (
    if exist %VS_PATH_PROFESSIONAL_64%\vcvarsall.bat (
        echo Found Visual Studio %VERSION% Professional, calling vcvarsall.bat...
        call %VS_PATH_PROFESSIONAL_64%\vcvarsall.bat x64
        exit /b 0
    )
)

rem Check Enterprise edition
if exist %VS_PATH_ENTERPRISE% (
    if exist %VS_PATH_ENTERPRISE%\vcvarsall.bat (
        echo Found Visual Studio %VERSION% Enterprise, calling vcvarsall.bat...
        call %VS_PATH_ENTERPRISE%\vcvarsall.bat x64
        exit /b 0
    )
)
if exist %VS_PATH_ENTERPRISE_64% (
    if exist %VS_PATH_ENTERPRISE_64%\vcvarsall.bat (
        echo Found Visual Studio %VERSION% Enterprise, calling vcvarsall.bat...
        call %VS_PATH_ENTERPRISE_64%\vcvarsall.bat x64
        exit /b 0
    )
)

echo Visual Studio %VERSION% not found.
exit /b 1


:eof
echo Done
