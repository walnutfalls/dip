@echo off

SET "VCPKG_DIR=bin/vcpkg"


:: Check if the directory does not exist
IF NOT EXIST "%VCPKG_DIR%" (
    cd /d bin
    git clone https://github.com/microsoft/vcpkg.git
    cd /d vcpkg
     .\bootstrap-vcpkg.bat
) ELSE (
    echo Directory already exists: "%VCPKG_DIR%"
)


pause