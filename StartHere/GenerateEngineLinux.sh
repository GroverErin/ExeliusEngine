#!/bin/bash

# ---------------------------------------------------------------------------------------------------

# Exelius Game Engine - Game Development Tool
# Copyright (c) 2020 Exelius Entertainment
# Created By: Erin "Ryukana" Grover

# This Engine is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# License for more details. You should have received a copy of the
# License along with this Engine.

# ---------------------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------------------
# Architecture Selection
# ---------------------------------------------------------------------------------------------------

echo
echo Please enter the desired architecture:
echo ---------------------------------------
echo "Command                    Architecture"
echo "-rpi, -rpi64          x64 RPi Linux/ARM"
echo "-rpi32, -rpi86        x86 RPi Linux/ARM"
echo "-x64*                         x64 Linux"
echo "-x86                          x86 Linux"
echo
echo "      * Default Architecture"
echo ---------------------------------------
echo
read -p 'Architecture: ' userArchitecture

# Error Checking and/or Set Default Value

if [ "$userArchitecture" == "-x64" ] || [ "$userArchitecture" == "" ]; then
    userArchitecture="--architecture=x64"
elif [ "$userArchitecture" == "-x86" ]; then
    userArchitecture="--architecture=x86"
elif [ "$userArchitecture" == "-rpi" ] || [ "$userArchitecture" == "-rpi64" ]; then
    userArchitecture="--architecture=rpi64"
elif [ "$userArchitecture" == "-rpi32" ] || [ "$userArchitecture" == "-rpi86" ]; then
    userArchitecture="--architecture=rpi86"
else
    echo "$userArchitecture is an invalid Architecture type."
    exit
fi

# ---------------------------------------------------------------------------------------------------
# Configuration Selection
# ---------------------------------------------------------------------------------------------------

echo
echo Please enter the desired configuration:
echo ---------------------------------------
echo "Command                   Configuration"
echo "-r, --release*      Release (Optimized)"
echo "-t, --test       Test (Optimized+Debug)"
echo "-d, --debug         Debug (Unoptimized)"
echo
echo "      * Default Configuration"
echo ---------------------------------------
echo
read -p 'Configuration: ' userConfig

# Error Checking and/or Set Default Value

if [ "$userConfig" == "-r" ] || [ "$userConfig" == "--release" ] || [ "$userConfig" == "" ]; then
    userConfig="--configuration=release"
elif [ "$userConfig" == "-t" ] || [ "$userConfig" == "--test" ]; then
    userConfig="--configuration=test"
elif [ "$userConfig" == "-d" ] || [ "$userConfig" == "--debug" ]; then
    userConfig="--configuration=debug"
else
    echo "$userConfig is an invalid Configuration type."
    exit
fi

# ---------------------------------------------------------------------------------------------------
# Run Premake with selected Architecture
# ---------------------------------------------------------------------------------------------------

if [ "$userArchitecture" == "--architecture=rpi64" ] || [ "$userArchitecture" == "--architecture=rpi86" ]; then
    echo Running Premake for Rapsberry Pi.
    #cd ../
    #./Tools/ThirdParty/premake-core/RaspberryPi/premake5 gmake $userArchitecture $userConfig
elif [ "$userArchitecture" == "--architecture=x64" ] || [ "$userArchitecture" == "--architecture=x86" ]; then
    echo Running Premake for Linux.
    #cd ../
    #./Tools/ThirdParty/premake-core/Linux/premake5 gmake $userArchitecture $userConfig
fi

# ---------------------------------------------------------------------------------------------------
# Build Makefile
# ---------------------------------------------------------------------------------------------------

#sudo make

# ---------------------------------------------------------------------------------------------------
# Run Built Executable
# ---------------------------------------------------------------------------------------------------

if [ "$userConfig" == "--configuration=release" ]; then
    echo Running Release Configuration.
    # export LD_LIBRARY_PATH=..\Tools\ThirdParty\SFML\lib\Linux\Release\ && ../Builds/Release_x86_64/EXESandbox/EXESandbox
elif [ "$userConfig" == "--configuration=test" ]; then
    echo Running Test Configuration.
    # export LD_LIBRARY_PATH=..\Tools\ThirdParty\SFML\lib\Linux\Release\ && ../Builds/Test_x86_64/EXESandbox/EXESandbox
elif [ "$userConfig" == "--configuration=debug" ]; then
    echo Running Debug Configuration.
    # export LD_LIBRARY_PATH=..\Tools\ThirdParty\SFML\lib\Linux\Debug\ && ../Builds/Debug_x86_64/EXESandbox/EXESandbox
fi