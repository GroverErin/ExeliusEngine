<p align="center">
  <img src="https://github.com/GroverErin/ExeliusEngine/blob/main/Tools/Images/Exelius_Engine.png?raw=true" alt="Exelius Engine Logo"/>
</p>

# Exelius Engine - A Simple Multiplatform 2D Game Engine
[![License](https://img.shields.io/github/license/GroverErin/ExeliusEngine.svg)](https://github.com/GroverErin/ExeliusEngine/blob/main/LICENSE)

Exelius written primarily with C++, as part of the Academy of Art GAP program.

This is the second version of the Engine that was written as a part of GAP 301 (Game Engine Programming).
The primary goal for this engine is to be educational, but there are a series of minor goals that will be attempted to be met as part of this 3 semester long project.
  - Platform Agnosticism — Builds for both Windows and Linux
  - Strong Fundamentals — allow for further development with ease.

Exelius currently supports the following platforms:
  - Windows ( Tested with Windows 10 and Visual Studio 2019)
  - Linux (Tested with Ubuntu 20.04 LTS)
  - Raspberry Pi 4 (Tested with PiOS x64)

| [Installation](#installation) | [Plans](#plans)         |  [Learn](#learn)               | [Author and Contributors](#author-and-contributors) |
|:------------------------------|-------------------------|--------------------------------|-----------------------------------------------------|
|[Windows](#windows)            | [Upcomming](#upcomming) | [Tutorials](#tutorials)        | [Further Credits](#further-credits)                 |
|[Linux](#linux)                | [Future](#future)       | [Documentation](#documentation)| [Contributing](#contributing)                       |

## Plans
___
### Trello Board
Check out the [![Trello Board](https://trello.com/b/AYUYeK4x)] for this project to see the specific roadmap for this project.
### Upcomming
Spring 2021 goals:
  - Linux & Windows Build System
  - File Saving & Loading
  - 2D Rendering
  - Game Object System
  - Component System
  - Separate Rendering Thread
  - Debugging Tools
  - Profiler
  - Utility Layer
  - Input
  - More...
### Future
Summer/Fall 2021 goals:
  - Thread Pool/Job System
  - Networking Layer
  - GUI Layer
  - Memory Manager
  - Resource Manager
  - More...
  
Spring 2022 goals:
  - Animations
  - Audio
  - Scripting (Lua and Native C++)
  - New Project Generation and Templates
  - More...

Further goals:
  - Runtime Editor
  - 3D Rendering
  - Lighting & Shading
  - Particle systems
  - Skeletal and Framed Animations
  - Networking
  - GUI
  - Android support
  - Apple support (maybe)
  - 32 bit architecture support (Not likely)
___
## Installation
### Windows
Building on windows:
  - Clone this repository into an empty directory using this command:
    - `git clone --recursive https://github.com/GroverErin/ExeliusEngine`
  - Run the GenerateExeliusEngine.bat script.
  - Open the ExeliusEngine.sln with Visual Studio.
  - Build desired configuration - Outputs to Builds/ directory.
  - You can run in either the Visual Studio IDE or navigate to the Builds directory and run the executable.
### Linux
Building on linux (tested on Ubuntu):
  - Clone this repository into an empty directory using this command:
    - `git clone --recursive https://github.com/GroverErin/ExeliusEngine`
  - Open the Terminal in the directory containing the cloned repository.
  - Run this command in the terminal:
    - `./GenerateLinuxFiles.sh`
  - Run this command in the terminal:
    - `sudo make && sudo make install`
  - Run one of the following commands to run the engine:
    - `./LaunchDebug.sh`
    - `./LaunchRelease.sh`
    - `./LaunchDistribution.sh`
___
## Learn
### Tutorials
TODO
These are some tutorials on the basics of using the engine.
### Documentation
TODO
This is the documentation for the engine... (Use Doxygen here?)
___
## Author and Contributors
  - Erin "Ryukana" Grover — Lead developer
  - David "Rez" Graham — Instructor and Mentor
  
### Further Credits
  - Dylan "Xeros" Wijnen — Helped with Premake/Lua build system.
  - Kirill "sizkirill" Sizykh — Helped with Premake/Lua build system for Linux.
  - Yan "The Cherno" Chernikov — YouTube series that I've used to evaluate different design decisions.
  
### Contributing
Currently the Exelius Engine is open for use by the public under the Apache License, but contributions are not being accepted as this is a student project. This may be revisited in the future.
