<h1 align="center">engine-2d</h1>
<p align="center">A 2D game engine with Lua scripting.<br>
<i><b>🗃️ Archived 🗃️</b></i>
</p>

This project is a cleaned-up version of an old assignment I gave myself. I just wanted to freshen it up a bit for everyone to see.

## Getting Started 🧰
The easiest way to get up and running on Ubuntu/Debian:

```bash
make setup && make run
```

This will:
1. Install dependencies and build tools
2. Build the project
3. Launch the default demo

## Running 🎮
The engine expects a game directory with `main.lua` (and optionally `config.lua`):

```bash
# Run with default game directory (demos/demo-snake)
./build/bin/engine2d

# Or specify a custom game directory
./build/bin/engine2d /path/to/game/directory
```

Example game directories:
- `demos/demo-squares/` — Squares demo
- `demos/demo-snake/` — Snake demo
- `demos/demo-dogs/` — Dogs demo
- `demos/demo-platformer/` — Infinite platformer demo

## Make Commands 🛠️
- `make setup` — Install dependencies and build tools (Ubuntu/Debian)
- `make build` — Configure and build the project
- `make run` — Build and run (defaults to `demos/demo-snake`)
- `make clean` — Remove build artifacts
- `make help` — Show available commands

## Development 🧑‍💻
- C++17
- CMake 3.15+ (with Make on Linux)