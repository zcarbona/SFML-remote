# 🚀 Endless Shooter — SFML 3 Game Demo

An exciting, lightweight **Endless Shooter** demo built from scratch using **C++20** and **SFML 3**.

The project is designed as a hands-on C++ game-development playground, focusing on object-oriented design, real-time input, entity management, projectile movement, collision detection, health systems, enemy behavior, level progression, and SFML rendering.

---

## 🎮 Gameplay & Mechanics

You control a player block on a customizable resolution canvas and survive against increasingly challenging enemies.

### 🧑‍🚀 Player

* Move freely around the screen using **W/A/S/D**.
* The player follows the mouse cursor for aiming.
* Player health is displayed through the HUD.
* Taking enough damage results in **Game Over**.

### 👾 Enemies

* Enemies are represented by floating square objects.
* Each enemy has its own health value, starting at **100**.
* Enemies move autonomously around the screen.
* Enemies bounce from the screen boundaries.
* Enemies can enter a **charging state** and attack the player.
* Enemy damage increases as the game level increases.

### 🔫 Shooting

* Left-click to fire a bullet toward the mouse cursor.
* Bullets travel in a normalized direction at a constant speed.
* Each successful hit deals **25 damage**.
* Bullets are removed after hitting an enemy.
* Bullets are also removed when they reach their target position.
* Bullet movement prevents overshooting the target and repeatedly oscillating around it.

### ❤️ Health & Combat

* Enemies start with **100 health**.
* Every bullet deals **25 damage**.
* An enemy is removed when its health reaches `0`.
* Charging enemies can damage the player.
* After damaging the player, the enemy retreats.

### 📈 Level System

The game continuously becomes more difficult.

* The game starts at **Level 1**.
* A new level is reached after a timed interval.
* The level-up interval becomes shorter as the level increases.
* A new enemy is spawned whenever the level increases.
* Enemy damage scales with the current level.
* There is no final level or victory condition — the goal is to survive and achieve a higher level.

### 💀 Game Over

When the player's health reaches zero:

* Gameplay stops.
* Shooting is disabled.
* The **GAME OVER** message appears in the HUD.
* Press **R** to restart the game from Level 1.

---

## 🕹️ Controls

|     Key / Action     | Function                |
| :------------------: | ----------------------- |
|        **`W`**       | Move Up                 |
|        **`A`**       | Move Left               |
|        **`S`**       | Move Down               |
|        **`D`**       | Move Right              |
|        **`R`**       | Restart after Game Over |
|        **`Q`**       | Quit / Close Game       |
| **Left Mouse Click** | Shoot toward the cursor |

> **Note:** The `R` key is used for restarting the game after Game Over. If quitting with `Q` is implemented through the current player/input code, it closes the game.

---

## 🛠️ Technical Stack & Architecture

This project uses modern C++ and a simple object-oriented architecture.

### Technology

* **Language:** C++20
* **Graphics:** SFML 3
* **Build System:** CMake
* **Compiler:** GCC, Clang, or MSVC with C++20 support
* **Platform:** Windows/Linux compatible with an appropriate SFML 3 setup

### Architecture

The game is separated into several classes, each responsible for a specific part of the game.

#### `Logic`

Responsible for the overall game:

* Main game loop
* SFML window management
* Event polling
* Game state
* Level progression
* Enemy spawning
* Bullet management
* Collision detection
* HUD rendering
* Game Over and restart logic

#### `Player`

Responsible for:

* Player movement
* Keyboard input
* Mouse aiming
* Screen boundaries
* Player health
* Taking damage
* Player rendering

#### `Bullet`

Responsible for:

* Projectile position
* Projectile target
* Direction calculation
* Constant projectile movement
* Bullet lifetime/removal
* Bullet rendering

The bullet direction is calculated using a normalized vector:

```text
direction = target - position
direction /= distance
```

This allows the projectile to travel toward the cursor at a constant speed.

#### `Object`

Responsible for enemies:

* Enemy position
* Autonomous movement
* Screen boundary bouncing
* Charging behavior
* Retreat behavior
* Health
* Damage calculation
* Enemy rendering

---

## ⚔️ Collision System

The game currently uses simple bounding-distance collision checks.

### Bullet vs Enemy

A bullet is considered to hit an enemy when its position is sufficiently close to the enemy's center.

When a collision occurs:

```text
Enemy Health -= 25
Bullet → Removed
```

If the enemy reaches zero health:

```text
Enemy → Removed
```

### Enemy vs Player

Only enemies in their **charging state** can damage the player.

When a charging enemy enters the player's collision range:

```text
Player Health -= Enemy Damage
Enemy → Retreat
```

---

## 📊 Level Progression

The level system dynamically increases the difficulty over time.

The level-up interval follows:

```cpp
std::max(5, 15 - level)
```

This means the game starts with longer intervals between levels and gradually becomes faster.

| Level | Approx. Level-Up Interval |
| :---: | :-----------------------: |
|   1   |         14 seconds        |
|   2   |         13 seconds        |
|   3   |         12 seconds        |
|  ...  |            ...            |
|  10+  |         5 seconds         |

As the level increases:

* New enemies appear.
* Enemy damage increases.
* The game becomes progressively harder.

---

## 📁 Project Structure

```text
EndlessShooter/
│
├── CMakeLists.txt
│
├── assets/
│   ├── texture/
│   │   └── Player/
│   │       └── Soldier_Walk.png
│   │
│   └── font/
│       └── GameFont.TTF
│
├── include/
│   ├── Player.hpp
│   ├── Entity.hpp
│   ├── bullet.hpp
│   ├── creatingOBJ.hpp
│   └── logic.hpp
│
├── src/
│   ├── Player.cpp
│   ├── Entity.cpp
│   ├── bullet.cpp
│   ├── creatingOBJ.cpp
│   ├── logic.cpp
│   └── main.cpp
│
└── build/
```

---

## 🚀 Building & Running

### Prerequisites

Make sure the following are installed:

1. **CMake 3.20+**
2. **C++20-compatible compiler**

   * GCC
   * Clang
   * MSVC
3. **SFML 3.x**
4. Required game assets

### Clone the Repository

```bash
git clone <repository-url>
cd EndlessShooter
```

### Configure the Project

```bash
mkdir build
cd build
cmake ..
```

### Build

```bash
cmake --build .
```

### Run

After compilation, run the generated executable from the appropriate build directory.

---

## 🖼️ Assets

The game currently loads its HUD font using a relative project path:

```text
assets/font/GameFont.TTF
```

This means the executable should be launched with the project root as its working directory, or the `assets` directory must be available relative to the executable's working directory.

If the game reports:

```text
Failed to load font: assets/font/GameFont.TTF
```

make sure the following file exists:

```text
assets/font/GameFont.TTF
```

---

## 🧠 Learning Goals

This project is primarily intended as a C++ and SFML learning project.

It demonstrates:

* C++20
* Object-oriented programming
* Classes and encapsulation
* `std::vector`
* Iterators
* `std::erase`
* Lambda functions
* `std::random_device`
* `std::mt19937`
* `std::uniform_real_distribution`
* `std::chrono`
* Vector mathematics
* Real-time input handling
* Game loops
* Collision detection
* Entity management
* State management
* SFML 3 graphics and events
* Runtime text rendering
* Basic game progression

---

## 👥 Collaborators

This project was built for learning and experimentation.

* **Mohamed Hanafy** — Lead Developer & Collaborator
  GitHub: [Hanafy10](https://github.com/Hanafy10)

---

## 📄 License

This project is open-source and free to modify, extend, or use for your own C++ and game-development learning.

Have fun and keep shooting! 🎮🔫
