#pragma once
#ifndef CHESSPUZZLESYSTEM_H
#define CHESSPUZZLESYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <set>
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;


enum class PuzzleDifficulty {
    EASY = 0,
    INTERMEDIATE = 1,
    HARD = 2,
    ULTRA_HARD = 3
};

struct Puzzle {
    int            id;
    string         fen;
    vector<string> solution;
    PuzzleDifficulty difficulty;
    string         theme;
    string         description;
    string         objective;
    bool           whiteToMove;
};

struct PuzzleResult {
    bool           correct;
    bool           isComplete;
    string         message;
    int            timeTaken;
    int            attempts;
    int            hintsUsed;
    int            oldRating;
    int            newRating;
    int            ratingChange;
    int            bonusPoints;
    int            streak;
    vector<string> solution;
};



class ChessPuzzleSystem {
private:
    vector<Puzzle> easyPuzzles;
    vector<Puzzle> intermediatePuzzles;
    vector<Puzzle> hardPuzzles;
    vector<Puzzle> ultraHardPuzzles;

    Puzzle* currentPuzzle;
    int       userRating;
    set<int>  solvedPuzzles;
    int       streakCount;
    time_t    lastSolvedDate;
    time_t    puzzleStartTime;
    int       moveIndex;
    int       attempts;
    int       hintsUsed;
    PuzzleDifficulty currentDifficulty;
    int       currentIndexInDifficulty;

public:
    ChessPuzzleSystem();
    ~ChessPuzzleSystem();

    void         initializePuzzles();
    Puzzle       getNextPuzzle(PuzzleDifficulty difficulty);
    Puzzle       getCurrentPuzzleByDifficulty(PuzzleDifficulty difficulty);
    void         startPuzzle(const Puzzle& puzzle);
    PuzzleResult checkMove(const string& move);
    PuzzleResult completePuzzle(bool solved);
    string       getHint();

    int  getRating()           const { return userRating; }
    int  getSolvedCount()      const { return (int)solvedPuzzles.size(); }
    int  getStreak()           const { return streakCount; }
    int  getTotalPuzzles()     const;
    int  getElapsedTime()      const;
    int  getRemainingAttempts()const { return 3 - attempts; }
    bool hasPuzzle()           const { return currentPuzzle != nullptr; }
    Puzzle* getCurrentPuzzle() { return currentPuzzle; }
    int  getPuzzleCount(PuzzleDifficulty difficulty)         const;
    int  getSolvedCountByDifficulty(PuzzleDifficulty diff)   const;

    string normalizeSAN(const string& move);
    string getPieceFromMove(const string& move);
    string getDestinationSquare(const string& move);
    bool   isConsecutiveDay(time_t lastDate);

    void resetProgress();
    void saveProgress();
    void loadProgress();
};



struct PuzzleButton {
    sf::RectangleShape shape;
    sf::Text           label;
    sf::Color          normalColor;
    sf::Color          hoverColor;

    void setup(const string& txt, sf::Font& font,
        unsigned int charSize,
        sf::Color bg, sf::Color hover,
        sf::Color outline, float thick = 2.f);
    void setPos(float x, float y);
    void resize(float w, float h);
    void update(sf::Vector2i mouse);
    void draw(sf::RenderWindow& win);
    bool contains(sf::Vector2f pos) const;
};

struct PuzzleNotification {
    sf::RectangleShape box;
    sf::Text           text;
    sf::Clock          clock;
    float              duration = 2.5f;
    bool               active = false;

    void show(const string& msg, sf::Color c, sf::Font& font, float dur = 2.5f);
    void draw(sf::RenderWindow& win);
    bool isActive() const { return active; }
};

struct PuzzleStars {
    vector<sf::CircleShape> stars;
    int count = 0;
    void setup(int n, float x, float y, float radius, float gap);
    void setRating(int r);   // 0 – 3
    void draw(sf::RenderWindow& win);
};

struct PuzzleMoveLog {
    vector<string>   moves;
    vector<sf::Color>colors;
    sf::Font* font = nullptr;
    sf::Vector2f     pos;
    float            lineH = 21.f;
    int              maxLines = 7;

    void init(sf::Font& f, sf::Vector2f p, float lineHeight = 21.f);
    void add(const string& san, bool correct);
    void clear();
    void draw(sf::RenderWindow& win);
};



void runPuzzleMode(sf::RenderWindow& window, ChessPuzzleSystem& ps);

#endif 
