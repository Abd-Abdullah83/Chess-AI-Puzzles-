#ifndef CHESSPUZZLESYSTEM_H
#define CHESSPUZZLESYSTEM_H

#include <string>
#include <vector>
#include <set>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;

// Forward declaration to avoid conflicts
enum class PuzzleDifficulty {
    EASY = 0,
    INTERMEDIATE = 1,
    HARD = 2,
    ULTRA_HARD = 3
};

struct Puzzle {
    int id;
    string fen;
    vector<string> solution;
    PuzzleDifficulty difficulty;
    string theme;
    string description;
    string objective;
    bool whiteToMove;
};

struct PuzzleResult {
    bool correct;
    bool isComplete;
    string message;
    int timeTaken;
    int attempts;
    int hintsUsed;
    int oldRating;
    int newRating;
    int ratingChange;
    int bonusPoints;
    int streak;
    vector<string> solution;
};

class ChessPuzzleSystem {
private:
    vector<Puzzle> easyPuzzles;
    vector<Puzzle> intermediatePuzzles;
    vector<Puzzle> hardPuzzles;
    vector<Puzzle> ultraHardPuzzles;

    Puzzle* currentPuzzle;
    int userRating;
    set<int> solvedPuzzles;
    int streakCount;
    time_t lastSolvedDate;

    time_t puzzleStartTime;
    int moveIndex;
    int attempts;
    int hintsUsed;

    PuzzleDifficulty currentDifficulty;
    int currentIndexInDifficulty;

public:
    ChessPuzzleSystem();
    ~ChessPuzzleSystem();

    void initializePuzzles();
    Puzzle getNextPuzzle(PuzzleDifficulty difficulty);
    Puzzle getCurrentPuzzleByDifficulty(PuzzleDifficulty difficulty);
    void startPuzzle(const Puzzle& puzzle);
    PuzzleResult checkMove(const string& move);
    string getHint();

    int getRating() const { return userRating; }
    int getSolvedCount() const { return (int)solvedPuzzles.size(); }
    int getStreak() const { return streakCount; }
    int getTotalPuzzles() const;
    int getElapsedTime() const;
    int getRemainingAttempts() const { return 3 - attempts; }

    string normalizeSAN(const string& move);
    string getPieceFromMove(const string& move);
    string getDestinationSquare(const string& move);
    bool isConsecutiveDay(time_t lastDate);
    PuzzleResult completePuzzle(bool solved);

    void resetProgress();
    void saveProgress();
    void loadProgress();

    bool hasPuzzle() const { return currentPuzzle != nullptr; }
    Puzzle* getCurrentPuzzle() { return currentPuzzle; }

    int getPuzzleCount(PuzzleDifficulty difficulty) const;
    int getSolvedCountByDifficulty(PuzzleDifficulty difficulty) const;
};

#endif