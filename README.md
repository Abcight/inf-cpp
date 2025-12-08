# inf-cpp

A 2D game engine built with C++, OpenGL, GLFW, Lua, and OpenAL.

## Quick Start (Ubuntu/Debian)

The easiest way to get started:

```bash
make setup && make run
```

This will:
1. Install all dependencies and build tools
2. Build the project
3. Run the game

## Available Make Commands

- `make setup` - Install all dependencies and build tools (Ubuntu/Debian)
- `make build` - Configure and build the project
- `make run` - Build and run the project (defaults to `demos/demo-snake` directory)
- `make clean` - Remove build artifacts
- `make help` - Show available commands

## Running

The engine expects a game directory with `main.lua` and optionally `config.lua`:

```bash
# Run with default game directory
./build/bin/inf-cpp

# Or specify a custom game directory
./build/bin/inf-cpp /path/to/game/directory
```

Example game directories are provided in:
- `demos/demo-squares/` - Squares demo
- `demos/demo-snake/` - Snake demo
- `demos/demo-dogs/` - Dogs demo
- `demos/demo-platformer/` - Infinite platformer demo

## Development

The project uses:
- **C++17** standard
- **CMake 3.15+** for build system
- **Make** as the generator (default on Linux)