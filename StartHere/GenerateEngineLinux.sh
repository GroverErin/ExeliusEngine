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

# call the other script using absolute path
./Tools/ThirdParty/Premake/Linux/premake5 gmake

if [ "$1" == "-d" ] then
    echo Running debug Configuration.
    export LD_LIBRARY_PATH=..\Tools\ThirdParty\SFML\lib\Linux\Debug\ && ../Builds/Debug_x86_64/EXESandbox/EXESandbox
fi

if [ "$1" == "-t" ] then
    echo Running Test Configuration.
    export LD_LIBRARY_PATH=..\Tools\ThirdParty\SFML\lib\Linux\Release\ && ../Builds/Test_x86_64/EXESandbox/EXESandbox
fi

if [ "$1" == "-r" ] then
    echo Running Release Configuration.
    export LD_LIBRARY_PATH=..\Tools\ThirdParty\SFML\lib\Linux\Release\ && ../Builds/Release_x86_64/EXESandbox/EXESandbox
fi
