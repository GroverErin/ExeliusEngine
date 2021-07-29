<p align="center">
  <img src="https://github.com/GroverErin/ExeliusEngine/blob/main/Tools/Images/Exelius_Engine.png?raw=true" alt="Exelius Engine Logo"/>
</p>

# Exelius Engine - A Simple Multiplatform 2D Game Engine
[![License](https://img.shields.io/github/license/GroverErin/ExeliusEngine.svg)](https://github.com/GroverErin/ExeliusEngine/blob/main/LICENSE)

Exelius written primarily with C++, as part of the Academy of Art GAP program.

This is the second version of the Engine that was written as a part of GAP 301 (Game Engine Programming).
The primary goal for this engine is to be educational, but there are a series of minor goals that will be attempted to be met as part of this 3 semester long project.
  - Platform Agnosticism — Builds for both Windows, Linux, and ARM in both x86 and x64 architectures.
  - Strong Fundamentals — Allow for further development with ease.

Exelius currently supports the following platforms:
  - Windows (Tested with Windows 10 and Visual Studio 2019)
  - Raspberry Pi 4 (Tested with PiOS x64)

| [Installation](#installation) | [Plans](#plans)         |  [Learn](#learn)               | [Author and Contributors](#author-and-contributors) | [Screenshots](#screenshots) |
|:------------------------------|-------------------------|--------------------------------|-----------------------------------------------------|-----------------------------|
|[Windows](#windows)            | [Upcoming](#upcoming)   | [Tutorials](#tutorials)        | [Further Credits](#further-credits)                 |                             |
|[Linux](#linux)                | [Future](#future)       | [Documentation](#documentation)| [Contributing](#contributing)                       |                             |
___
## Plans
### Trello Board
Check out the [Trello Board](https://trello.com/b/AYUYeK4x) to see the specific roadmap for this project.
### Complete/Testing
  - Windows Build System
  - Raspberry Pi x64 Build System (Tested with PiOS x64)
  - 2D Rendering on Separate Dedicated Thread
  - Resource Managed - Reference counted assets.
  - Fast Debug Logging System
  - Game Object and Component System (ECS and Inheretance Hybrid)
  - Keyboard and Gamepad Input
### Upcoming
Summer/Fall 2021 goals:
  - File Saving & Loading
  - Improved Debugging Tools
  - Profiler
  - Utility and Math Layer
  - Thread Pool/Job System
  - Improved/Rewritten Build system
  - Linux Build System
  - Memory Manager
  - Scripting (Lua and Native C++)
### Future
Spring 2022 goals:
  - Flipbook Animations
  - Audio
  - New Project Generation and Templates
  - Networking Layer
  - GUI Layer
Further goals:
  - Runtime Editor
  - 3D Rendering
  - Lighting & Shading
  - Particle system
  - [Spine Skeletal Animations](http://esotericsoftware.com)
  - Android support
  - Visual Studio Code IDE support
___
## Installation
### Windows
#### Requirements:
  - [Visual Studio 2019](https://visualstudio.microsoft.com/vs/)
  - [Git for Windows (Optional)](https://gitforwindows.org)
#### Installation and Building:
  - Clone this repository into an empty directory using this command:
    - `git clone https://github.com/GroverErin/ExeliusEngine`
    - Alternatively, you can just download the source by selecting the Code dropdown and the Download ZIP option.
<p align="center">
  <img src="https://github.com/GroverErin/ExeliusEngine/blob/main/Tools/Images/DownloadRepo.png?raw=true" alt="Alternate Download Method"/>
</p>

  - Navigate to the `ExeliusEngine/StartHere/` directory.
  - Run the `GenerateEngineWindows.bat` script.
  - Open the `ExeliusEngine.sln` in the root directory with Visual Studio 2019.
  - Build desired configuration - Outputs to Builds/ directory.
  - You can run the build in either the Visual Studio IDE or navigate to the `ExeliusEngine/Builds/[Config]_x86_64/EXESandbox/` directory and run the executable.
    - Is it recommended to run the engine from the Visual Studio IDE, as running the executable requires copying some assets. To Be Patched.
### Linux
### Raspberry Pi
___
## Learn
### Tutorials
TODO
These are some tutorials on the basics of using the engine.
### Documentation
TODO
This is the documentation for the engine. (Use Doxygen here?)
___
## Author and Contributors
  - Erin "Ryukana" Grover — Lead developer
  - David "Rez" Graham — Instructor and Mentor
### Further Credits
  - Josh Stefanski — Instructor, gave tips and feedback on threaded renderer.
  - Dylan "Xeros" Wijnen — Helped with Premake/Lua build system, Game Object and Component systems, and other minor tips and tricks.
  - Kirill "sizkirill" Sizykh — Helped with Premake/Lua build system for Linux.
  - Yan "The Cherno" Chernikov — YouTube series that I've used to evaluate different design decisions.
### Contributing
Currently the Exelius Engine is open for use by the public under the Apache License, but contributions are not being accepted as this is a student project. This may be revisited in the future. However, if you do run into any bug while testing or using this engine, please let me know so I can fix it.
___
## Screenshots
