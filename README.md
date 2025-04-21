# Match-3 Game using SFML

A simple Match-3 puzzle game made with C++ and SFML. Match at least 3 similar gems to earn points and level up. This project demonstrates object-oriented programming with inheritance, file handling for saving/loading, and interactive graphics.

## Features
- 8x8 gem grid with smooth tile movement
- Score tracking and level progression
- Save and load game state (keys: `K` to save, `L` to load)
- Cursor movement using `W`, `A`, `S`, `D`
- Mouse-based gem swapping
- Level up when score reaches 100+

## Controls
- **Mouse**: Click two adjacent tiles to swap
- **Keyboard**:
  - `W`, `A`, `S`, `D`: Move cursor
  - `K`: Save game
  - `L`: Load game
  - `Esc` or window close: Exit game

## Requirements
- SFML 2.5 or later
- C++ compiler

## Assets
Place the following files in the `assets/` directory:
- `background.png` – background image
- `gems.png` – spritesheet of gems
- `font.ttf` – font for UI text

## Build & Run
Compile using any SFML-supported compiler setup. Example with g++:
```bash
g++ main.cpp -o match3 -lsfml-graphics -lsfml-window -lsfml-system
./match3
