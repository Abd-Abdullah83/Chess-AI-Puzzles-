# ♟ Chess Master

![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue?style=flat-square&logo=cplusplus)
![SFML](https://img.shields.io/badge/Graphics-SFML%202.x-green?style=flat-square)
![AI](https://img.shields.io/badge/AI-Minimax%20%2B%20Alpha--Beta-blueviolet?style=flat-square)
![Puzzle](https://img.shields.io/badge/Feature-Puzzle%20System-orange?style=flat-square)
![Audio](https://img.shields.io/badge/Audio-SFML%20Sound-brightgreen?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-lightgrey?style=flat-square)

> A full-featured graphical chess game built in C++ with SFML — Human vs Human, Human vs AI with three difficulty levels, and a rated puzzle training mode with 4 tiers.

---

## Screenshots

| Main Menu | AI Difficulty Select |
|:---:|:---:|
| ![Menu](screenshots/menu.png) | ![Difficulty](screenshots/difficulty.png) |

| Blue Board Theme | Green Board Theme |
|:---:|:---:|
| ![Blue](screenshots/board_blue.png) | ![Green](screenshots/board_green.png) |

---

## Game Modes

| Mode | Description |
|------|-------------|
| **Human vs Human** | Local 2-player on the same machine. Full chess rules enforced. |
| **Human vs AI (White)** | Play as Black against the AI engine. Choose Easy / Medium / Hard. |
| **Human vs AI (Black)** | Play as White against the AI engine. Choose Easy / Medium / Hard. |
| **Puzzle Mode** | Rated puzzle training across 4 difficulty tiers with streaks and hints. |

---

## AI Engine

The AI uses **Minimax with Alpha-Beta pruning** and a **quiescence search** extension to avoid the horizon effect on captures. A hard 1.8-second time limit per turn keeps gameplay smooth.

### Difficulty levels

| Level | Behaviour |
|-------|-----------|
| Easy | Shallow search depth — makes occasional mistakes |
| Medium | Balanced depth — solid tactical play |
| Hard | Deep Minimax + quiescence — aggressive and positionally aware |

---

## Chess Rules Implemented

| Rule | Details |
|------|---------|
| Castling | Kingside & queenside for both colours; rook and king move flags tracked |
| En passant | Full double-pawn push detection; target square stored each turn |
| Pawn promotion | Interactive promotion menu appears on reaching the 8th rank |
| Check detection | Every move validated to ensure the moving side's king is not left in check |
| Checkmate & stalemate | Detected and announced at end of each turn |
| Undo / Redo | Full `GameState` stack — unlimited undo and redo during a game |
| Captured pieces | White and black captured pieces tracked separately |

---

## Puzzle System — `ChessPuzzleSystem`

A complete standalone puzzle engine with FEN-based positions, move validation, a rating system, and streak tracking.

### Puzzle difficulty tiers

| Tier | Description |
|------|-------------|
| `EASY` | Beginner tactics — one-move mates and simple forks |
| `INTERMEDIATE` | Two-move combinations, pins, and skewers |
| `HARD` | Multi-move sequences and piece sacrifices |
| `ULTRA_HARD` | Deep calculation, endgame studies, and complex middlegame tactics |

### Key `ChessPuzzleSystem` methods

```cpp
void initializePuzzles();                         // Load all puzzle banks
Puzzle getNextPuzzle(PuzzleDifficulty difficulty); // Advance to next puzzle in tier
void startPuzzle(const Puzzle& puzzle);           // Begin timing and reset state
PuzzleResult checkMove(const string& move);       // Validate move against solution
string getHint();                                 // Return next solution move hint
PuzzleResult completePuzzle(bool solved);         // Compute rating change & streak
```

### `PuzzleResult` fields

| Field | Description |
|-------|-------------|
| `correct` / `isComplete` | Per-move and full-puzzle result flags |
| `timeTaken` | Seconds from puzzle start to final move |
| `attempts` | Max 3 attempts per puzzle |
| `hintsUsed` | Hint count — reduces rating gain |
| `ratingChange` / `bonusPoints` | Elo-style rating delta + streak bonus |
| `streak` | Consecutive-day solving streak count |
| `oldRating` / `newRating` | Rating before and after the puzzle |

### `Puzzle` struct

```cpp
struct Puzzle {
    int id;
    string fen;               // Board position in FEN notation
    vector<string> solution;  // Sequence of correct SAN moves
    PuzzleDifficulty difficulty;
    string theme;             // e.g. "Fork", "Pin", "Mating Net"
    string description;
    string objective;
    bool whiteToMove;
};
```

---

## Board Themes

A slider in the top-right corner of the game window lets you switch board colour themes at runtime without restarting. Currently included:

- **Blue** — classic blue / light-blue alternating tiles
- **Green** — forest green / light-green alternating tiles

Coordinate labels (`a1`–`h8`) are displayed in red on each square.

---

## Sound Effects

All audio loads from the `audio/` folder. Missing files fall back gracefully with a console warning.

| Event | File |
|-------|------|
| Move | `move.wav` |
| Capture | `capture.wav` |
| Castling | `castling.wav` |
| Checkmate | `checkmate.wav` |
| Stalemate | `stalemate.wav` |
| Game start | `start.wav` |
| Puzzle complete | `completing.wav` |
| Background | `background.mp3` (looping `sf::Music`) |

---

## Project Structure

```
Chess-AI-Puzzles/
├── Chess.cpp                  # Main game — SFML rendering, input, AI, game loop
├── ChessPuzzleSystem.cpp      # Puzzle engine implementation
├── ChessPuzzleSystem.h        # Puzzle structs, enums, class declaration
├── pieces/                    # Piece sprite PNGs (white and black sets)
├── audio/                     # Sound effects + background music
├── Font/
│   └── arial.ttf              # UI font
├── .gitignore
└── LICENSE
```

---

## How to Build & Run

### Linux / macOS

```bash
# Clone
git clone https://github.com/Abd-Abdullah83/Chess-AI-Puzzles-.git
cd Chess-AI-Puzzles-

# Compile
g++ -std=c++17 Chess.cpp ChessPuzzleSystem.cpp -o chess \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Run (from project root so assets resolve correctly)
./chess
```

### Windows (MinGW)

```bash
g++ -std=c++17 Chess.cpp ChessPuzzleSystem.cpp -o chess.exe ^
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

chess.exe
```

> [!NOTE]
> SFML 2.x must be installed. Ubuntu: `sudo apt install libsfml-dev` · macOS: `brew install sfml` · Windows: download from [sfml-dev.org](https://www.sfml-dev.org/download.php) and add to your MinGW library path.

> [!WARNING]
> Always run the executable from the **project root directory**. The game expects `pieces/`, `audio/`, and `Font/arial.ttf` relative to the working directory. Running from a different location will cause asset load failures.

---

## Controls

| Action | Input |
|--------|-------|
| Select & move a piece | Left click — click the source square, then the destination |
| Undo move | **UNDO** button (top-left of game window) |
| Redo move | **REDO** button (top-left of game window) |
| Switch board colour theme | Slider in top-right corner |
| Pawn promotion | Interactive menu auto-appears on reaching the 8th rank |
| Navigate menus | Left click on mode / difficulty buttons |

---

## Dependencies

| Library | Purpose |
|---------|---------|
| SFML Graphics | Window, board rendering, sprites, UI |
| SFML Audio | Sound effects and background music |
| SFML Window | Event handling, keyboard/mouse input |
| C++ STL | `stack` for undo/redo, `vector`, `set`, `algorithm` |

---

## Author

**Abd-Abdullah83**  
Built as a C++ / SFML project — FAST National University of Computer & Emerging Sciences (NUCES)

---

<p align="center"><sub>C++ · SFML · Minimax Alpha-Beta · Puzzle Rating System · FAST NUCES</sub></p>
