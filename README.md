# MyFirstGame

### Folder Structure

```
MyFirstGame/
├── assets/                # Game assets such as images, sounds, fonts, etc.
│   ├── images/            # Images / Textures / Sprites
│   ├── sounds/            # Sounds
│   ├── fonts/             # Fonts
│   ├── shaders/           # Shaders
│   |   ├── quad.vert      #
│   |   └── quad.frag      #
│   └── worlds/            # Worlds (Tiled Application Files)
|
├── build/                 # Build output directory
|
├── cmake/                 # CMake modules and scripts
|
├── include/               # Header files
│   ├── engine/            # Engine-specific headers
│   │   ├── renderer.hpp   #
│   │   ├── input.hpp      #
│   ├── game/              # Game-specific headers
│   |   ├── player.hpp     #
│   |   └── game.hpp       #
│   └── shared/            # Shared headers
|
├── src/                   # Source files
│   ├── engine/            # Engine-specific source files
│   │   ├── renderer.cpp   #
│   │   ├── input.cpp      #
│   ├── game/              # Game-specific source files
│   |   ├── player.cpp     #
│   |   └── game.cpp       #
│   └── shared/            # Shared source files
|
├── third_party/           # Third-party libraries
|
├── CMakeLists.txt         # CMake build script
├── main.cpp               # Main entry point of the game
├── README.md              # Project documentation
└── README.todo            # Todo List
```

- **assets/**: This directory contains all the game assets such as images, sounds, fonts, and shaders. Organizing assets into subdirectories helps keep them organized and easy to find.
  - **images/**: Contains image files (e.g., PNG, JPEG) used in the game.
  - **sounds/**: Contains sound files (e.g., WAV, MP3) used in the game.
  - **fonts/**: Contains font files (e.g., TTF) used in the game.
  - **shaders/**: Contains shader files (e.g., GLSL) used in the game.
- **build/**: This directory is used for build output. It is common to separate the build output from the source code to keep the project directory clean.
- **cmake/**: This directory contains CMake modules and scripts. If you use CMake for your build system, you can place custom CMake modules and scripts here.
- **include/**: This directory contains header files. It is common to separate header files from source files to improve organization.
  - **engine/**: Contains engine-specific headers. These are headers for the game engine or framework you are using or developing.
  - **game/**: Contains game-specific headers. These are headers for the game logic and components.
- **src/**: This directory contains source files. It is common to separate source files from header files to improve organization.
  - **engine/**: Contains engine-specific source files. These are source files for the game engine or framework you are using or developing.
  - **game/**: Contains game-specific source files. These are source files for the game logic and components.
  - **shared/**: Contains files that are used by both the engine and the game.
- **third_party/**: This directory contains third-party libraries. It is common to place third-party libraries in a separate directory to keep them organized and separate from your own code.
- **CMakeLists.txt**: This is the CMake build script. It defines how the project should be built using CMake.
- **README.md**: This file contains project documentation. It is common to include a README file with information about the project, how to build it, and how to use it.
- **main.cpp**: This is the main entry point of the game. It contains the main function that initializes the game and starts the game loop.
