<div align="center">

# ♟️ CHESS MASTER

### *A full-featured chess game built in C++ with SFML*

<br>

![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![SFML](https://img.shields.io/badge/SFML-2.x-8CC445?style=for-the-badge&logo=sfml&logoColor=white)
![AI](https://img.shields.io/badge/AI-Minimax_Alpha--Beta-9B59B6?style=for-the-badge)
![Puzzles](https://img.shields.io/badge/Puzzles-4_Difficulty_Tiers-E67E22?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-lightgrey?style=for-the-badge)

<br>

> *Human vs Human · Human vs AI · Rated Puzzle Training · Undo/Redo · Themes · Sound*

<br>

---

</div>

## 🎬 Demo Videos

| Mode | Link |
|:----:|:----:|
| 🎮 Full Gameplay | [▶ Watch](ChessV%20(1).mp4) |
| 🤖 Human vs AI | [▶ Watch](Human%20VS%20Ai%20.mp4) |
| 👥 Human vs Human | [▶ Watch](HumanvsHuman.mp4) |
| 🧩 Puzzle Mode | [▶ Watch](ChessPuzzle.mp4) |

---

## 🕹️ Game Modes

<div align="center">

| | Mode | Description |
|:---:|:---|:---|
| 🟢 | **Human vs Human** | Local 2-player · full rules enforced |
| 🔵 | **Human vs AI (White)** | You play Black · choose your difficulty |
| 🔴 | **Human vs AI (Black)** | You play White · choose your difficulty |
| 🟣 | **Puzzle Mode** | Rated training · 4 tiers · streaks · hints |

</div>

---

## 🤖 AI Engine

The AI uses **Minimax with Alpha-Beta pruning** and a **quiescence search** extension to avoid horizon-effect blunders. A hard **1.8-second** time limit per move keeps gameplay fluid.

<div align="center">

| 🟢 Easy | 🟡 Medium | 🔴 Hard |
|:---:|:---:|:---:|
| Shallow depth | Balanced search | Deep + quiescence |
| Makes mistakes | Solid tactics | Aggressive & accurate |

</div>

---

## ♟️ Chess Rules Implemented

| Rule | Detail |
|:---|:---|
| ♜ **Castling** | Kingside & queenside — king and rook move flags tracked |
| ♟ **En Passant** | Double-pawn push detection; target square cleared each turn |
| ♛ **Pawn Promotion** | Interactive menu auto-appears on reaching the 8th rank |
| 👑 **Check Detection** | Every move validated — king cannot be left in check |
| ✅ **Checkmate & Stalemate** | Detected and announced with dedicated sound |
| ↩️ **Undo / Redo** | Full `GameState` stack — unlimited during a game |
| 📋 **Captured Pieces** | Tracked separately for White and Black |

---

## 🧩 Puzzle System

A complete standalone engine with **FEN-based positions**, SAN move validation, Elo-style rating, streak tracking, and hint support.

### Difficulty Tiers

<div align="center">

| Tier | Level | Description |
|:---:|:---:|:---|
| 🟢 | **Easy** | One-move mates, simple forks |
| 🔵 | **Intermediate** | Two-move combos, pins, skewers |
| 🟠 | **Hard** | Multi-move sequences, sacrifices |
| 🔴 | **Ultra Hard** | Deep calculation, endgame studies |

</div>

### Rating & Progress Tracking

```
✔  timeTaken      — seconds from start to solution
✔  attempts       — max 3 per puzzle
✔  hintsUsed      — reduces rating gain
✔  ratingChange   — Elo-style delta
✔  bonusPoints    — streak reward
✔  streak         — consecutive-day solving streak
✔  oldRating / newRating
```

### Core API

```cpp
void         initializePuzzles();
Puzzle       getNextPuzzle(PuzzleDifficulty difficulty);
void         startPuzzle(const Puzzle& puzzle);
PuzzleResult checkMove(const string& move);
string       getHint();
PuzzleResult completePuzzle(bool solved);
```

---

## 🎨 Board Themes

Switch themes at runtime using the **slider** in the top-right corner — no restart needed.

<div align="center">

| 🔵 Blue Theme | 🟢 Green Theme |
|:---:|:---:|
| Classic blue / light-blue tiles | Forest green / light-green tiles |

</div>

> Coordinate labels (`a1`–`h8`) displayed in red on every square.

---

## 🔊 Sound Effects

> All effects load from `audio/` — missing files fall back gracefully.

`move` · `capture` · `castling` · `checkmate` · `stalemate` · `game start` · `puzzle complete` · `background music (loop)`

---

## 📁 Project Structure

```
Chess-AI-Puzzles/
│
├── 📄 Chess.cpp                 ← Main game: rendering, input, AI, loop
├── 📄 ChessPuzzleSystem.cpp     ← Puzzle engine implementation
├── 📄 ChessPuzzleSystem.h       ← Structs, enums, class declaration
│
├── 🎬 ChessPuzzle.mp4           ← Puzzle mode demo
├── 🎬 ChessV (1).mp4            ← Full gameplay demo
├── 🎬 Human VS Ai .mp4          ← Human vs AI demo
├── 🎬 HumanvsHuman.mp4          ← Human vs Human demo
│
├── 📂 pieces/                   ← Piece sprite PNGs
├── 📂 audio/                    ← Sound effects + music
├── 📂 Font/
│   └── arial.ttf
├── 📂 Output/                    ← Screenshots of Game
│
├── .gitignore
└── LICENSE
```

---

## 🚀 How to Build & Run

### 🐧 Linux / 🍎 macOS

```bash
# Clone the repository
git clone https://github.com/Abd-Abdullah83/Chess-AI-Puzzles-.git
cd Chess-AI-Puzzles-

# Compile
g++ -std=c++17 Chess.cpp ChessPuzzleSystem.cpp -o chess \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Run from project root (assets resolve relative to working directory)
./chess
```

### 🪟 Windows (MinGW)

```bash
g++ -std=c++17 Chess.cpp ChessPuzzleSystem.cpp -o chess.exe ^
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

chess.exe
```

> [!NOTE]
> **SFML 2.x** must be installed.
> - Ubuntu → `sudo apt install libsfml-dev`
> - macOS → `brew install sfml`
> - Windows → [sfml-dev.org](https://www.sfml-dev.org/download.php)

> [!WARNING]
> Always run from the **project root directory** — the game resolves `pieces/`, `audio/`, and `Font/` relative to the working directory.

---

## 🎮 Controls

<div align="center">

| Action | Input |
|:---|:---|
| Select & move a piece | Left click → source square → destination |
| Undo move | **UNDO** button (top-left) |
| Redo move | **REDO** button (top-left) |
| Switch board theme | Slider (top-right) |
| Pawn promotion | Interactive menu on 8th rank |
| Navigate menus | Left click on buttons |

</div>

---

## 🛠️ Dependencies

| Library | Purpose |
|:---|:---|
| `SFML Graphics` | Window, board, sprites, UI rendering |
| `SFML Audio` | Sound effects and looping background music |
| `SFML Window` | Events, keyboard & mouse input |
| `C++ STL` | `stack` (undo/redo), `vector`, `set`, `algorithm` |

---

<div align="center">

---

### Built by **Abd-Abdullah83**
*FAST National University of Computer & Emerging Sciences (NUCES)*

<br>

*Every position has a best move.*

</div>
