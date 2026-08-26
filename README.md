# 🚀 Endless Shooter — SFML 3 Game Demo

An exciting, lightweight, and fun **Endless Shooter** demo built from scratch using **C++20** and the **SFML 3 (Simple and Fast Multimedia Library)** API. 

This project serves as an interactive playground to demonstrate basic game mechanics including physics-based movement, particle/bullet trajectory tracking, real-time bounding box collisions, dynamic entity spawning, and text rendering.

---

## 🎮 Gameplay & Mechanics

In this game, you control a player block on a custom resolution canvas:
* **The Player:** A responsive block (blue) that moves freely across the screen.
* **The Targets:** Floating square blocks that drift around, bouncing realistically off the walls. Each block exhibits a random initial trajectory and dynamically changes its course every 5 seconds.
* **Dynamic Health & Text Rendering:** Every enemy block displays its current health (starting at `100`).
* **Interactive Combat:** Fire high-velocity projectiles (white bullets) directly towards your mouse cursor position. Bullet hits deplete the targets' health by `25` per shot. Upon reaching `0` health, targets are vaporized!

---

## 🕹️ Controls

Take control with these straightforward key bindings:

| Key / Action | Function |
| :---: | :--- |
| **`W`** | Move Up |
| **`A`** | Move Left |
| **`S`** | Move Down |
| **`D`** | Move Right |
| **`R`** | Emergency Reset (teleports player back to center) |
| **`Q`** | Quit / Close Game |
| **`Left Mouse Click`** | Shoot projectile towards cursor position |

---

## 🛠️ Technical Stack & Architecture

This demo leverages modern C++ practices and clean Object-Oriented Design (OOD):

* **Language:** C++20
* **Graphics Library:** [SFML 3](https://www.sfml-dev.org/) (specifically `Graphics` and `Audio` modules)
* **Build System:** CMake (Minimum version 3.20)
* **Design Patterns:** Component-like architecture with clean state separation:
  * **`Logic`**: Manages the main game loop, windows polling, user events, and master collision detection.
  * **`Player`**: Handles player-specific input, movement bounds, and drawing.
  * **`Bullet`**: Calculates linear vectors towards target mouse coordinates using trigonometry ($\Delta X, \Delta Y$ vector normalizing).
  * **`Object`**: Manages autonomous floating obstacles, boundary bouncing, and health state.

---

## 🚀 Building & Running

### Prerequisites

Ensure you have the following installed on your system:
1. **CMake** (v3.20 or higher)
2. **C++20 Compatible Compiler** (MSVC, GCC, or Clang)
3. **SFML 3.x** development libraries installed and configured in your system path.

### Build Steps

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd SFML
   ```

2. **Generate build files and compile:**
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. **Asset Setup Note:**
   The game loads its UI/HUD font from `C:\GameFont.TTF` at runtime (as specified in `src/logic.cpp`). A copy of the font `GameFont.TTF` is conveniently bundled in the project's assets folder at:
   ```text
   assets/font/GameFont.TTF
   ```
   *Make sure to copy/link this file or update the logic source path to run the executable correctly outside your system root.*

---

## 👥 Collaborators

This project was built just for fun and learning!

* **Mohamed Hanafy** ([Hanafy10](https://github.com/Hanafy10)) — Lead Developer & Collaborator

---

## 📄 License

This project is open-source and free to modify, extend, or use in your own game development learning journey. Have fun! 🎮
