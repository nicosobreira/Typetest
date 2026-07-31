# Typetest

A minimalist terminal-based type trainer written in C, powered by `ncurses`.

![Demo](.github/demo.gif)

## Features

- **Scene-based architecture** driven by a fixed-timestep game loop (`Input` → fixed `Update` steps at `MS_PER_UPDATE` → `Draw`), so simulation rate stays decoupled from input polling and rendering.
- **Live typing feedback**.
- **A small custom "engine" (`Core`)**:
  - Window layout & alignment helpers built on top of raw `ncurses` windows
  - Cursor movement clamped to a window's bounds
  - A fixed-timestep `Clock`
  - A centralized `ERROR()` macro for consistent diagnostics and clean shutdown
  - Lightweight data structures (a `wchar_t` stack, a non-owning `String` view) purpose-built for the typing buffer
- **Sample bank** of Portuguese-language sentences of varying lengths used as typing prompts.

## Project Layout

```
.
├── Core/
│   ├── include/Core/
│   │   ├── clock.h
│   │   ├── constants.h
│   │   ├── error.h
│   │   ├── math.h
│   │   ├── scenes.h
│   │   ├── data_structures/  # Point, StackChar, String
│   │   └── ui/               # Window, Cursor, Color helpers
│   └── src/
├── App/
│   └── src/
│       ├── main.c
│       ├── scenes/           # Menu, Typing, Score
│       └── typing/           # Text entry + sample sentence bank
├── tools/
│   └── run.sh                # Build/run/debug convenience script
├── CMakeLists.txt
├── CMakePresets.json
├── flake.nix / flake.lock    # Optional reproducible dev shell (Nix)
└── .envrc                    # direnv integration for the flake
```

## Requirements

- CMake ≥ 3.15
- A C11-capable compiler (GCC or Clang)
- `ncursesw` (wide-character ncurses) development headers
- Optional, for `tools/run.sh debug`: `gdb`, and `tmux` + `gdbserver` if you want the split-pane debugging experience
- Optional: [Nix](https://nixos.org/) for a reproducible, zero-setup dev shell

## Building

### With Nix (optional, recommended)

```bash
nix develop
```

### Configure + build

The project ships CMake **workflow presets** that configure and build in one
step:

```bash
cmake --workflow --preset dev    # Debug build   -> build/debug/
cmake --workflow --preset ship   # Release build -> build/release/
```

Or drive configure/build separately:

```bash
cmake --preset debug                 # or: release
cmake --build --preset build-debug   # or: build-release
```

## Running

```bash
./build/debug/App/typetest       # Debug build
./build/release/App/typetest     # Release build
```

### Using the `tools/run.sh` helper

```bash
./tools/run.sh          # default: builds (dev workflow) and runs the debug binary
./tools/run.sh build    # just configure + build (dev workflow), don't run
./tools/run.sh debug    # build, then launch gdb (TUI);
                        # inside tmux, spins up gdbserver in a split pane automatically
```

## Controls

**Menu**
| Key | Action                 |
| :-: | :-                     |
| `P` | Start a typing session |
| `Q` | Quit                   |

**Typing**
| Key               | Action                                   |
| :-:               | :-                                       |
| *(any character)* | Type it — green if correct, red if wrong |
| Backspace         | Undo the last keystroke                  |
| `ESC`             | Return to the menu                       |

**Score**
| Key | Action                       |
| :-: | :-                           |
| `R` | Restart (new typing session) |
| `M` | Back to menu                 |
| `Q` | Quit                         |

## Code Style & Tooling

The repo is set up for consistent formatting and static analysis out of the box:

- `.clang-format` — Microsoft-based style
- `.editorconfig` — 4-space indentation, LF line endings, 80-column limit for C/C++ sources
- `.clangd` — points at `build/debug`'s compilation database for editor tooling (`compile_commands.json`)

## Known Issues / TODO

A few things are already flagged in the source:

- **`Window_SetAlign_Right`** (`Core/src/ui/window.c`) doesn't use the reference position, so right-aligned windows aren't positioned correctly.
- The layout factors in `Window_SetLayout_Center` / `Window_SetLayout_OnTop` are hard-coded rather than configurable per window.
- `ENTER` key handling in the typing scene is a stub.
