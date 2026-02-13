#define _CRT_SECURE_NO_WARNINGS

#include "ChessPuzzleSystem.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

ChessPuzzleSystem::ChessPuzzleSystem()
    : currentPuzzle(nullptr), userRating(1200), streakCount(0),
    lastSolvedDate(0), moveIndex(0), attempts(0), hintsUsed(0),
    currentDifficulty(PuzzleDifficulty::EASY), currentIndexInDifficulty(0) {
}

ChessPuzzleSystem::~ChessPuzzleSystem() {
    if (currentPuzzle) delete currentPuzzle;
}

void ChessPuzzleSystem::initializePuzzles() {
    easyPuzzles.clear();
    intermediatePuzzles.clear();
    hardPuzzles.clear();
    ultraHardPuzzles.clear();

    // ==================== EASY PUZZLES (5) ====================

    Puzzle e1;
    e1.id = 1;
    e1.fen = "6k1/5ppp/8/8/8/8/5PPP/R5K1 w - - 0 1";
    e1.solution = { "Ra8#" };
    e1.difficulty = PuzzleDifficulty::EASY;
    e1.theme = "Back Rank Mate";
    e1.description = "White to move";
    e1.objective = "Checkmate the King";
    e1.whiteToMove = true;
    easyPuzzles.push_back(e1);

    Puzzle e2;
    e2.id = 2;
    e2.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1";
    e2.solution = { "Nxe4" };
    e2.difficulty = PuzzleDifficulty::EASY;
    e2.theme = "Knight Fork";
    e2.description = "Black to move";
    e2.objective = "Win material with fork";
    e2.whiteToMove = false;
    easyPuzzles.push_back(e2);

    Puzzle e3;
    e3.id = 3;
    e3.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1";
    e3.solution = { "Qxf7#" };
    e3.difficulty = PuzzleDifficulty::EASY;
    e3.theme = "Queen Checkmate";
    e3.description = "White to move";
    e3.objective = "Checkmate the King";
    e3.whiteToMove = true;
    easyPuzzles.push_back(e3);

    Puzzle e4;
    e4.id = 4;
    e4.fen = "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1";
    e4.solution = { "Qg5" };
    e4.difficulty = PuzzleDifficulty::EASY;
    e4.theme = "Attack Queen";
    e4.description = "Black to move";
    e4.objective = "Win the Queen";
    e4.whiteToMove = false;
    easyPuzzles.push_back(e4);

    Puzzle e5;
    e5.id = 5;
    e5.fen = "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1";
    e5.solution = { "Bxf7+" };
    e5.difficulty = PuzzleDifficulty::EASY;
    e5.theme = "Pin and Check";
    e5.description = "White to move";
    e5.objective = "Win the pawn with check";
    e5.whiteToMove = true;
    easyPuzzles.push_back(e5);

    // ==================== INTERMEDIATE PUZZLES (5) ====================

    Puzzle i1;
    i1.id = 6;
    i1.fen = "r1bqk2r/ppp2ppp/2np1n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQK2R w KQkq - 0 1";
    i1.solution = { "Bxf7+", "Kxf7", "Ng5+" };
    i1.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i1.theme = "Discovered Attack";
    i1.description = "White to move";
    i1.objective = "Win the Queen";
    i1.whiteToMove = true;
    intermediatePuzzles.push_back(i1);

    Puzzle i2;
    i2.id = 7;
    i2.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/2N2N2/PPPP1PPP/R1BQK2R b KQkq - 0 1";
    i2.solution = { "Nxe4", "Nxe4", "d5" };
    i2.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i2.theme = "Double Attack";
    i2.description = "Black to move";
    i2.objective = "Win the Bishop";
    i2.whiteToMove = false;
    intermediatePuzzles.push_back(i2);

    Puzzle i3;
    i3.id = 8;
    i3.fen = "4r1k1/5ppp/8/8/8/8/5PPP/4R1K1 w - - 0 1";
    i3.solution = { "Re8+", "Rxe8" };
    i3.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i3.theme = "Skewer";
    i3.description = "White to move";
    i3.objective = "Win the Rook";
    i3.whiteToMove = true;
    intermediatePuzzles.push_back(i3);

    Puzzle i4;
    i4.id = 9;
    i4.fen = "8/4P3/8/8/8/4k3/8/4K3 w - - 0 1";
    i4.solution = { "e8=Q+" };
    i4.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i4.theme = "Promotion";
    i4.description = "White to move";
    i4.objective = "Promote the pawn with check";
    i4.whiteToMove = true;
    intermediatePuzzles.push_back(i4);

    Puzzle i5;
    i5.id = 10;
    i5.fen = "r3k2r/ppp2ppp/2n5/3q4/3P4/2PQ4/PP3PPP/R3K2R b KQkq - 0 1";
    i5.solution = { "Qxd4", "Qxd4", "Nxd4" };
    i5.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i5.theme = "Deflection";
    i5.description = "Black to move";
    i5.objective = "Win material";
    i5.whiteToMove = false;
    intermediatePuzzles.push_back(i5);

    // ==================== HARD PUZZLES (5) ====================

    Puzzle h1;
    h1.id = 11;
    h1.fen = "r2qkb1r/ppp2ppp/2n5/3pP3/3Pn1b1/2PB1N2/PP3PPP/RNBQK2R b KQkq - 0 1";
    h1.solution = { "Nxf2", "Kxf2", "Qh4+", "g3", "Qxg3#" };
    h1.difficulty = PuzzleDifficulty::HARD;
    h1.theme = "Sacrificial Attack";
    h1.description = "Black to move";
    h1.objective = "Checkmate the King";
    h1.whiteToMove = false;
    hardPuzzles.push_back(h1);

    Puzzle h2;
    h2.id = 12;
    h2.fen = "r1bqk2r/ppp2ppp/2n5/2bpP3/8/2PB1Q2/PP3PPP/RNB1K1NR w KQkq - 0 1";
    h2.solution = { "Qxf7+", "Kxf7", "Bc4+" };
    h2.difficulty = PuzzleDifficulty::HARD;
    h2.theme = "Queen Sacrifice";
    h2.description = "White to move";
    h2.objective = "Win material advantage";
    h2.whiteToMove = true;
    hardPuzzles.push_back(h2);

    Puzzle h3;
    h3.id = 13;
    h3.fen = "r1bq1rk1/ppp2ppp/2n5/3pPb2/1b1P4/2NB1N2/PPP2PPP/R1BQ1RK1 w - - 0 1";
    h3.solution = { "Bxh7+", "Kxh7", "Ng5+", "Kg8", "Qh5" };
    h3.difficulty = PuzzleDifficulty::HARD;
    h3.theme = "Greek Gift";
    h3.description = "White to move";
    h3.objective = "Win the Queen";
    h3.whiteToMove = true;
    hardPuzzles.push_back(h3);

    Puzzle h4;
    h4.id = 14;
    h4.fen = "6k1/5ppp/8/8/8/8/5PPP/R5K1 w - - 0 1";
    h4.solution = { "Ra8+", "Kh7", "Ra7" };
    h4.difficulty = PuzzleDifficulty::HARD;
    h4.theme = "Rook Endgame";
    h4.description = "White to move";
    h4.objective = "Win all pawns";
    h4.whiteToMove = true;
    hardPuzzles.push_back(h4);

    Puzzle h5;
    h5.id = 15;
    h5.fen = "8/8/4k3/4p3/4K3/8/8/8 w - - 0 1";
    h5.solution = { "Kd3", "Kd5", "Ke3" };
    h5.difficulty = PuzzleDifficulty::HARD;
    h5.theme = "Opposition";
    h5.description = "White to move";
    h5.objective = "Win the pawn";
    h5.whiteToMove = true;
    hardPuzzles.push_back(h5);

    // ==================== ULTRA HARD PUZZLES (5) ====================

    Puzzle u1;
    u1.id = 16;
    u1.fen = "r1bqk2r/ppp2ppp/2n2n2/2bpp3/2B1P3/2PP1N2/PP3PPP/RNBQ1RK1 b kq - 0 1";
    u1.solution = { "Nxe4", "dxe4", "Bxf2+", "Kxf2", "Qh4+", "g3", "Qxc4+" };
    u1.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u1.theme = "Complex Sacrifice";
    u1.description = "Black to move";
    u1.objective = "Win the Queen";
    u1.whiteToMove = false;
    ultraHardPuzzles.push_back(u1);

    Puzzle u2;
    u2.id = 17;
    u2.fen = "6rk/6pp/7P/8/8/8/8/5Q1K w - - 0 1";
    u2.solution = { "Qf7", "Rg7", "Qf8+", "Rg8", "Qxg8#" };
    u2.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u2.theme = "Smothered Mate";
    u2.description = "White to move";
    u2.objective = "Checkmate the King";
    u2.whiteToMove = true;
    ultraHardPuzzles.push_back(u2);

    Puzzle u3;
    u3.id = 18;
    u3.fen = "r2q1rk1/ppp2ppp/2n5/3pPb2/1b1P4/2NB1N2/PPP1QPPP/R1B2RK1 w - - 0 1";
    u3.solution = { "Bxh7+", "Kxh7", "Ng5+", "Kg6", "Qg4", "f5", "Qg3" };
    u3.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u3.theme = "Mating Attack";
    u3.description = "White to move";
    u3.objective = "Checkmate the King";
    u3.whiteToMove = true;
    ultraHardPuzzles.push_back(u3);

    Puzzle u4;
    u4.id = 19;
    u4.fen = "r1bq1rk1/ppp2ppp/2n5/3pPb2/1b1P4/2NB1N2/PPP2PPP/R1BQR1K1 w - - 0 1";
    u4.solution = { "Bxh7+", "Kxh7", "Ng5+", "Kg8", "Qh5", "Re8", "Qxf7+", "Kh8", "Qh5+", "Kg8", "Qh7#" };
    u4.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u4.theme = "Multiple Sacrifices";
    u4.description = "White to move";
    u4.objective = "Checkmate the King";
    u4.whiteToMove = true;
    ultraHardPuzzles.push_back(u4);

    Puzzle u5;
    u5.id = 20;
    u5.fen = "8/8/8/4k3/4p3/4K3/4P3/8 w - - 0 1";
    u5.solution = { "Kd3", "Kd5", "Ke3", "Ke5", "Kd3" };
    u5.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u5.theme = "King and Pawn Endgame";
    u5.description = "White to move";
    u5.objective = "Win the game";
    u5.whiteToMove = true;
    ultraHardPuzzles.push_back(u5);
}

Puzzle ChessPuzzleSystem::getNextPuzzle(PuzzleDifficulty difficulty) {
    vector<Puzzle>* puzzleSet = nullptr;

    switch (difficulty) {
    case PuzzleDifficulty::EASY:
        puzzleSet = &easyPuzzles;
        break;
    case PuzzleDifficulty::INTERMEDIATE:
        puzzleSet = &intermediatePuzzles;
        break;
    case PuzzleDifficulty::HARD:
        puzzleSet = &hardPuzzles;
        break;
    case PuzzleDifficulty::ULTRA_HARD:
        puzzleSet = &ultraHardPuzzles;
        break;
    }

    if (puzzleSet && !puzzleSet->empty()) {
        vector<Puzzle> unsolved;
        for (const auto& p : *puzzleSet) {
            if (solvedPuzzles.find(p.id) == solvedPuzzles.end()) {
                unsolved.push_back(p);
            }
        }

        if (unsolved.empty()) {
            int idx = rand() % puzzleSet->size();
            return (*puzzleSet)[idx];
        }

        int idx = rand() % unsolved.size();
        return unsolved[idx];
    }

    return easyPuzzles[0];
}

Puzzle ChessPuzzleSystem::getCurrentPuzzleByDifficulty(PuzzleDifficulty difficulty) {
    return getNextPuzzle(difficulty);
}

void ChessPuzzleSystem::startPuzzle(const Puzzle& puzzle) {
    if (currentPuzzle) delete currentPuzzle;

    currentPuzzle = new Puzzle(puzzle);
    puzzleStartTime = time(0);
    moveIndex = 0;
    attempts = 0;
    hintsUsed = 0;
}

PuzzleResult ChessPuzzleSystem::checkMove(const string& move) {
    PuzzleResult result;
    result.correct = false;
    result.isComplete = false;

    if (!currentPuzzle) {
        result.message = "No active puzzle";
        return result;
    }

    string expectedMove = currentPuzzle->solution[moveIndex];
    bool isCorrect = (normalizeSAN(move) == normalizeSAN(expectedMove));

    if (isCorrect) {
        moveIndex++;

        if (moveIndex >= (int)currentPuzzle->solution.size()) {
            return completePuzzle(true);
        }

        result.correct = true;
        result.message = "Correct! Continue...";
        result.isComplete = false;
        return result;
    }
    else {
        attempts++;

        if (attempts >= 3) {
            return completePuzzle(false);
        }

        result.correct = false;
        result.message = "Incorrect. " + to_string(3 - attempts) + " attempts remaining";
        result.isComplete = false;
        return result;
    }
}

PuzzleResult ChessPuzzleSystem::completePuzzle(bool solved) {
    PuzzleResult result;

    time_t endTime = time(0);
    int timeTaken = (int)difftime(endTime, puzzleStartTime);

    int puzzleDifficultyRating = 800;
    switch (currentPuzzle->difficulty) {
    case PuzzleDifficulty::EASY: puzzleDifficultyRating = 800; break;
    case PuzzleDifficulty::INTERMEDIATE: puzzleDifficultyRating = 1200; break;
    case PuzzleDifficulty::HARD: puzzleDifficultyRating = 1600; break;
    case PuzzleDifficulty::ULTRA_HARD: puzzleDifficultyRating = 2000; break;
    }

    int ratingChange = 0;
    int bonusPoints = 0;

    if (solved) {
        double expectedScore = 1.0 / (1.0 + pow(10.0, (puzzleDifficultyRating - userRating) / 400.0));
        int kFactor = 32;
        ratingChange = (int)round(kFactor * (1.0 - expectedScore));

        if (timeTaken < 30) bonusPoints += 20;
        else if (timeTaken < 60) bonusPoints += 10;
        else if (timeTaken < 120) bonusPoints += 5;

        if (attempts == 0) bonusPoints += 15;
        if (hintsUsed == 0) bonusPoints += 10;

        time_t today = time(0);
        if (!isConsecutiveDay(lastSolvedDate)) {
            streakCount = 1;
        }
        else {
            streakCount++;
        }
        lastSolvedDate = today;

        solvedPuzzles.insert(currentPuzzle->id);
        result.message = "Puzzle solved!";
    }
    else {
        double expectedScore = 1.0 / (1.0 + pow(10.0, (puzzleDifficultyRating - userRating) / 400.0));
        int kFactor = 32;
        ratingChange = (int)round(kFactor * (0.0 - expectedScore));

        streakCount = 0;
        result.message = "Puzzle failed. Better luck next time!";
    }

    int oldRating = userRating;
    userRating = max(400, userRating + ratingChange + bonusPoints);

    result.correct = solved;
    result.isComplete = true;
    result.timeTaken = timeTaken;
    result.attempts = attempts;
    result.hintsUsed = hintsUsed;
    result.oldRating = oldRating;
    result.newRating = userRating;
    result.ratingChange = ratingChange + bonusPoints;
    result.bonusPoints = bonusPoints;
    result.streak = streakCount;
    result.solution = currentPuzzle->solution;

    delete currentPuzzle;
    currentPuzzle = nullptr;

    return result;
}

string ChessPuzzleSystem::getHint() {
    if (!currentPuzzle) {
        return "No active puzzle";
    }

    hintsUsed++;
    string currentMove = currentPuzzle->solution[moveIndex];

    string hint;
    switch (hintsUsed) {
    case 1:
        hint = "Theme: " + currentPuzzle->theme;
        break;
    case 2:
        hint = "Move your " + getPieceFromMove(currentMove);
        break;
    case 3:
        hint = "The move ends on " + getDestinationSquare(currentMove);
        break;
    default:
        hint = "Move: " + currentMove;
        break;
    }

    return hint;
}

string ChessPuzzleSystem::normalizeSAN(const string& move) {
    string result = move;
    result.erase(remove(result.begin(), result.end(), '+'), result.end());
    result.erase(remove(result.begin(), result.end(), '#'), result.end());
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    return result;
}

string ChessPuzzleSystem::getPieceFromMove(const string& move) {
    if (move.empty()) return "Pawn";

    char firstChar = move[0];
    switch (firstChar) {
    case 'K': return "King";
    case 'Q': return "Queen";
    case 'R': return "Rook";
    case 'B': return "Bishop";
    case 'N': return "Knight";
    default: return "Pawn";
    }
}

string ChessPuzzleSystem::getDestinationSquare(const string& move) {
    for (size_t i = 0; i < move.length() - 1; i++) {
        if (move[i] >= 'a' && move[i] <= 'h' &&
            move[i + 1] >= '1' && move[i + 1] <= '8') {
            return move.substr(i, 2);
        }
    }
    return "";
}

bool ChessPuzzleSystem::isConsecutiveDay(time_t lastDate) {
    if (lastDate == 0) return false;

    time_t now = time(0);
    double diffSeconds = difftime(now, lastDate);
    int diffDays = (int)(diffSeconds / (60 * 60 * 24));

    return diffDays == 1;
}

void ChessPuzzleSystem::resetProgress() {
    userRating = 1200;
    solvedPuzzles.clear();
    streakCount = 0;
    lastSolvedDate = 0;
    if (currentPuzzle) {
        delete currentPuzzle;
        currentPuzzle = nullptr;
    }
}

void ChessPuzzleSystem::saveProgress() {
    ofstream file("puzzle_progress.dat");
    if (file.is_open()) {
        file << userRating << endl;
        file << streakCount << endl;
        file << lastSolvedDate << endl;
        file << solvedPuzzles.size() << endl;
        for (int id : solvedPuzzles) {
            file << id << " ";
        }
        file.close();
    }
}

void ChessPuzzleSystem::loadProgress() {
    ifstream file("puzzle_progress.dat");
    if (file.is_open()) {
        file >> userRating;
        file >> streakCount;
        file >> lastSolvedDate;

        int count;
        file >> count;
        solvedPuzzles.clear();
        for (int i = 0; i < count; i++) {
            int id;
            file >> id;
            solvedPuzzles.insert(id);
        }
        file.close();
    }
}

int ChessPuzzleSystem::getTotalPuzzles() const {
    return (int)(easyPuzzles.size() + intermediatePuzzles.size() +
        hardPuzzles.size() + ultraHardPuzzles.size());
}

int ChessPuzzleSystem::getElapsedTime() const {
    if (!currentPuzzle) return 0;
    time_t now = time(0);
    return (int)difftime(now, puzzleStartTime);
}

int ChessPuzzleSystem::getPuzzleCount(PuzzleDifficulty difficulty) const {
    switch (difficulty) {
    case PuzzleDifficulty::EASY: return (int)easyPuzzles.size();
    case PuzzleDifficulty::INTERMEDIATE: return (int)intermediatePuzzles.size();
    case PuzzleDifficulty::HARD: return (int)hardPuzzles.size();
    case PuzzleDifficulty::ULTRA_HARD: return (int)ultraHardPuzzles.size();
    default: return 0;
    }
}

int ChessPuzzleSystem::getSolvedCountByDifficulty(PuzzleDifficulty difficulty) const {
    const vector<Puzzle>* puzzleSet = nullptr;
    switch (difficulty) {
    case PuzzleDifficulty::EASY: puzzleSet = &easyPuzzles; break;
    case PuzzleDifficulty::INTERMEDIATE: puzzleSet = &intermediatePuzzles; break;
    case PuzzleDifficulty::HARD: puzzleSet = &hardPuzzles; break;
    case PuzzleDifficulty::ULTRA_HARD: puzzleSet = &ultraHardPuzzles; break;
    }

    if (!puzzleSet) return 0;

    int count = 0;
    for (const auto& p : *puzzleSet) {
        if (solvedPuzzles.find(p.id) != solvedPuzzles.end()) {
            count++;
        }
    }
    return count;
}