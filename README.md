<p align="center">
  <img src="https://github.com/GroverErin/ExeliusEngine/blob/main/tools/images/Exelius_Engine.png?raw=true" alt="Exelius Engine Logo"/>
</p>

# Exelius Engine - A Simple Multiplatform 2D Game Engine
[![License](https://img.shields.io/github/license/GroverErin/ExeliusEngine.svg)](https://github.com/GroverErin/ExeliusEngine/blob/main/LICENSE)

Exelius is a 2D game engine written primarily with C++, as part of the Academy of Art GAP program.

This is the second version of the engine that was written as a part of GAP 301 (Game Engine Programming).

Note: This is a project that is in active development, and information displayed here may be out-of-date or incorrect.
Updates are *usually* only checked in when the build is in a working state, however, stuff happens. Eventually I will figure
out how to get a badge that will tell you if the build is working. Feel free to reach out if you have an issue!

Exelius currently supports the following platforms:
  - Windows (Tested with Windows 10 and Visual Studio 2019)
  - Linux (Tested with Ubuntu 20.04 and gcc)
  - Raspberry Pi 4 (Tested with PiOS x64 and gcc)

|[Installation](#installation) |[Goals](#goals)         |[Learn](#learn)                |[Acknowledgements](#acknowledgements)              |[Gallery](#screenshots)    |
|:-----------------------------|------------------------|-------------------------------|---------------------------------------------------|---------------------------|
|[Windows](#windows)           |[Plans & Trello](#plans)|[FAQ](#faq)                    |[Author and Contributors](#author-and-contributors)|[Screenshots](#screenshots)|
|[Linux](#linux)               |[Upcoming](#upcoming)   |[Tutorials](#tutorials)        |[Further Credits](#further-credits)                |                           |
|[Raspberry Pi](#raspberry-pi) |[Future](#future)       |[Documentation](#documentation)|[Third-Party Tools](#third-party-tools)            |                           |
___
## Goals
The design philosophy of the engine stands on the three pillars below. Will I accomplish them all? In 3 semesters... no. Maybe eventually though!
  - Platform Agnosticism — Build for Windows, Linux, Android, Intel, AMD, ARM, x86, x86, Web, and small kitchen appliances!
  - Strong Fundamentals — I don't want to have to *rewrite* this engine more often than I ship a game.
  - Simplicity — An engine so easy a 4 year old could use it!

### Why make another Unity/Unreal/Godot clone?
Well, education mostly. Outside of the educational benefit, I wanted to make a project that I can:
  - Run on my raspberry pi, for cross-platform dev experience, but also so my pi doesn't gather dust on my shelf.
  - Say I made from scratch (or as close as I can reasonably get on my own!).
  - Use to make a small game that I can play with my wife and son (So multiplayer is a must!).
  - Use as tool in which I can teach my son game development at a young age.
___
## Plans
### Trello Board
Check out the [Trello Board](https://trello.com/b/AYUYeK4x) to see the specific roadmap for this project.
You can also see the overall progress, as well as the features currently in production.
### Complete/Testing
  - Windows Build System
  - Linux Build System
  - Raspberry Pi x64 Build System (Tested with PiOS x64 and gcc)
  - 2D Rendering on Separate Dedicated Thread
  - Resource Managed - Reference counted assets.
  - Fast & Flexible Debug Logging System
  - Game Object and Component System (ECS and Inheretance Hybrid)
  - Keyboard and Gamepad Input
  - Engine Configuration from Data
  - File Saving & Loading
  - Improved/Rewritten Build system
  - Memory Manager
  - Thread Pool/Job System
### Upcoming
Summer/Fall 2021 goals:
  - GUI Layer
  - Message/Event System (To prep for networking)
  - Networking Layer
### Future
Spring 2022 goals:
  - Profiler
  - Scripting (Lua and Native C++)
  - Flipbook/Sprite Animations
  - Tilemaps
Stretch goals:
  - New Project Generation and Templates
  - Utility Tools and Math Layer
  - Texture Atlasing
  - Audio
  - [Runtime Editor w/ Hot-reloading and debugging](https://youtu.be/Qbt-1rcSqZc)
  - [Visual Node Graph Scripting](https://youtu.be/pnjDL-axeIU)
  - [Visual Code Block Scripting](https://developers.google.com/blockly/)
  - [Lighting & Shading](https://youtu.be/K4Gr1sXnl9A)
  - [Particle system](https://youtu.be/RMfsdUKEmDo)
  - [Spine 2D Skeletal Animations](http://esotericsoftware.com)
  - Android support
  - Visual Studio Code IDE support
  - Splash screen w/ audio cue
  - 3D Rendering
___
## Installation
### Windows
#### Requirements:
  - [Visual Studio 2019](https://visualstudio.microsoft.com/vs/)
  - [Python 3.3+](https://www.python.org/downloads/) (Typing the `python` command in windows terminals will open the windows store to download the requirements. Easy!)
  - [Git for Windows (Optional)](https://gitforwindows.org)
#### Installation and Building:
  - Clone this repository into an empty directory using this command:
    - `git clone https://github.com/GroverErin/ExeliusEngine`
    - Alternatively, you can just download the source by selecting the Code dropdown and the `Download ZIP` option.
<p align="center">
  <img src="https://github.com/GroverErin/ExeliusEngine/blob/main/tools/images/DownloadRepo.png?raw=true" alt="Alternate Download Method"/>
</p>

  - Inside of a terminal window, navigate to the `ExeliusEngine/` directory.
    - The fastest way to accomplish this is to type `cmd` into the address bar (not the Search bar) and press Enter/Return.
  - Run the command `python Setup.py` in the terminal.
  - Open the `ExeliusEngine.sln` in the root directory with Visual Studio 2019.
  - Build desired configuration - Outputs to `bin/` and `obj/` directories.
  - You can run the build in either the Visual Studio IDE or navigate to the `ExeliusEngine/bin/[Config]_[Architecture]/exeliuseditor/` directory and run the executable.
### Linux
Note: May not be entirely accurate, as my development OS is Windows 10, so other OS's are not always up to date.
#### Requirements:
  - [Python 3.3+](https://www.python.org/downloads/)
#### Installation and Building:
  - Clone this repository into an empty directory using the same methods as shown in [Windows](#windows).

  - Inside of a terminal window, navigate to the `ExeliusEngine/` directory.
  - Run the command `python3 Setup.py` in the terminal.
  - Run the command `make` in the terminal.
    - Currently the only configuration available in non-Windows OS's is Release.
  - Navigate to the `ExeliusEngine/bin/[Config]_[Architecture]/exeliuseditor/` directory and run the executable.
### Raspberry Pi
Note: May not be entirely accurate, as my development OS is Windows 10, so other OS's are not always up to date.
#### Requirements:
  - [Python 3.3+](https://www.python.org/downloads/)
#### Installation and Building:
  - Follow the same steps found in [Linux](#linux). The build system will automatically handle building for ARM processors.
___
## Learn
### FAQ
  - Q: Do people really ask you questions frequently enough to need a FAQ?
    - A: Nope. I'm usually the one asking questions. "Why am I doing this?", "Why doesn't this work?", "Why are you in my kitchen again?"
  - Q: Don't you think your scope is a little... broad?
    - A: Yep! :) I'm writing this engine for educational and personal purposes. If I think of something that would be fun to write, I add it to my planned features. Do I intend to ship a game in the near future with this engine... probably not. Someday maybe. :)
### Tutorials
The plan is to start including some tutorials once the engine is in a more finalized state. Engine... final... pffffft.
### Documentation
This will be the documentation for the engine. Use of [Doxygen](https://www.doxygen.nl/index.html) is intended here, possibly to generate the github Wiki as well.
It won't happen for a while longer, as I don't really feel like regenerating the docs for every commit. The source code itself is fairly well commented, but if you
have questions feel free to reach out to me.
___
## Acknowledgements
### Author and Contributors
  - Erin Grover — Lead developer
  - David "Rez" Graham — Instructor and Mentor
  - Josh Stefanski — Instructor and Mentor.
### Further Credits
  - Dylan Wijnen — Helped with Premake/Lua build system, Game Object and Component systems, and other minor tips and tricks.
  - Kirill Sizykh — Helped with the first iteration of the Linux build system. Taught me how Linux works for building complex C++ projects.
  - [Yan "The Cherno" Chernikov](https://www.youtube.com/c/TheChernoProject/featured) — YouTube series that I've used to evaluate different design decisions. See note below on this.
  - Jason Gregory — Wrote the book [Game Engine Architecture](https://www.gameenginebook.com), which I have "skimmed" cover to cover. Never met Jason though...

  Note on "The Cherno": There are some design decisions that are very similar between the Exelius and Hazel Engines. My intention is not to claim this code as my own, and I have compiled a list of his ideas that I have liked and used in my project.
  
    - The design of the core Application class and how it hooks into a client application. I have made changes to his original method to allow much simpler hooking for users.
    - I wrote my python build system by following the build system found in his project. I have made quite a few changes to this, and have made it python 2.7 compatable (that might change though).
    - I am currently using his event system to handle window events from SFML. I will be stripping this system out shortly, to be replaced with an entirely new system.
    - I might use his LayerStack design for applications. I haven't yet, but I am exploring the idea and the implications it would have in Exelius.
### Third-Party Tools
Here is a list of all the third party tools currently being used by Exelius. The goal is to make this list as short as possible. Other libraries may be included but are currently unused.
  - [Premake 5](https://premake.github.io) — Used for the build system.
  - [SFML](https://www.sfml-dev.org) — Used for window creation, rendering, and input events.
  - [EASTL](https://github.com/electronicarts/EASTL) — Used as the main STL for Exelius.
  - [rapidjson](https://github.com/Tencent/rapidjson) — Used to parse gameobjects and the config file (which should not be json but whatever... subject to change blah blah).
  - [spdlog](https://github.com/gabime/spdlog) — Used for multithreaded debug logging to console and files.
  - [Dear ImGui](https://github.com/ocornut/imgui) — Not currently used. Will be used to create an editor GUI. This will be separate from the in-game (retained) gui.
  - [zlib](https://github.com/madler/zlib) — Not currently used. Will be used to pack and unpack assets in the resource system.
___
## Gallery
### Screenshots
Not really much to see at the moment... unless you really like to look at log messages on a console.
