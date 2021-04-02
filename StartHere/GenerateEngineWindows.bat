@ECHO OFF
REM ---------------------------------------------------------------------------------------------------

REM Exelius Game Engine - Game Development Tool
REM Copyright (c) 2020 Exelius Entertainment
REM Created By: Erin "Ryukana" Grover

REM This Engine is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
REM License for more details. You should have received a copy of the
REM License along with this Engine.

REM ---------------------------------------------------------------------------------------------------
SETLOCAL
CD ..\
CALL Tools\ThirdParty\premake-core\Windows\premake5.exe vs2019

PAUSE