#define _CRT_SECURE_NO_WARNINGS
#include "ChessPuzzleSystem.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace sf;
using namespace std;



ChessPuzzleSystem::ChessPuzzleSystem()
    : currentPuzzle(nullptr), userRating(1200), streakCount(0),
    lastSolvedDate(0), moveIndex(0), attempts(0), hintsUsed(0),
    currentDifficulty(PuzzleDifficulty::EASY), currentIndexInDifficulty(0) {
}

ChessPuzzleSystem::~ChessPuzzleSystem() {
    if (currentPuzzle) delete currentPuzzle;
}

// ============================================================
//  initializePuzzles
// ============================================================

void ChessPuzzleSystem::initializePuzzles() {
    easyPuzzles.clear(); intermediatePuzzles.clear();
    hardPuzzles.clear(); ultraHardPuzzles.clear();

    // ── EASY ──────────────────────────────────────────────
    Puzzle e1; e1.id = 1; e1.fen = "6k1/5ppp/8/8/8/8/5PPP/R5K1 w - - 0 1";
    e1.solution = { "Ra8#" }; e1.difficulty = PuzzleDifficulty::EASY;
    e1.theme = "Back Rank Mate"; e1.description = "White to move";
    e1.objective = "Checkmate the King"; e1.whiteToMove = true; easyPuzzles.push_back(e1);

    Puzzle e2; e2.id = 2; e2.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1";
    e2.solution = { "Nxe4" }; e2.difficulty = PuzzleDifficulty::EASY;
    e2.theme = "Knight Fork"; e2.description = "Black to move";
    e2.objective = "Win material with fork"; e2.whiteToMove = false; easyPuzzles.push_back(e2);

    Puzzle e3; e3.id = 3; e3.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1";
    e3.solution = { "Qxf7#" }; e3.difficulty = PuzzleDifficulty::EASY;
    e3.theme = "Queen Checkmate"; e3.description = "White to move";
    e3.objective = "Checkmate the King"; e3.whiteToMove = true; easyPuzzles.push_back(e3);

    Puzzle e4; e4.id = 4; e4.fen = "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1";
    e4.solution = { "Qg5" }; e4.difficulty = PuzzleDifficulty::EASY;
    e4.theme = "Attack Queen"; e4.description = "Black to move";
    e4.objective = "Win the Queen"; e4.whiteToMove = false; easyPuzzles.push_back(e4);

    Puzzle e5; e5.id = 5; e5.fen = "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1";
    e5.solution = { "Bxf7+" }; e5.difficulty = PuzzleDifficulty::EASY;
    e5.theme = "Pin and Check"; e5.description = "White to move";
    e5.objective = "Win pawn with check"; e5.whiteToMove = true; easyPuzzles.push_back(e5);

    // ── INTERMEDIATE ──────────────────────────────────────
    Puzzle i1; i1.id = 6; i1.fen = "r1bqk2r/ppp2ppp/2np1n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQK2R w KQkq - 0 1";
    i1.solution = { "Bxf7+","Kxf7","Ng5+" }; i1.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i1.theme = "Discovered Attack"; i1.description = "White to move";
    i1.objective = "Win the Queen"; i1.whiteToMove = true; intermediatePuzzles.push_back(i1);

    Puzzle i2; i2.id = 7; i2.fen = "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/2N2N2/PPPP1PPP/R1BQK2R b KQkq - 0 1";
    i2.solution = { "Nxe4","Nxe4","d5" }; i2.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i2.theme = "Double Attack"; i2.description = "Black to move";
    i2.objective = "Win the Bishop"; i2.whiteToMove = false; intermediatePuzzles.push_back(i2);

    Puzzle i3; i3.id = 8; i3.fen = "4r1k1/5ppp/8/8/8/8/5PPP/4R1K1 w - - 0 1";
    i3.solution = { "Re8+","Rxe8" }; i3.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i3.theme = "Skewer"; i3.description = "White to move";
    i3.objective = "Win the Rook"; i3.whiteToMove = true; intermediatePuzzles.push_back(i3);

    Puzzle i4; i4.id = 9; i4.fen = "8/4P3/8/8/8/4k3/8/4K3 w - - 0 1";
    i4.solution = { "e8=Q+" }; i4.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i4.theme = "Promotion"; i4.description = "White to move";
    i4.objective = "Promote with check"; i4.whiteToMove = true; intermediatePuzzles.push_back(i4);

    Puzzle i5; i5.id = 10; i5.fen = "r3k2r/ppp2ppp/2n5/3q4/3P4/2PQ4/PP3PPP/R3K2R b KQkq - 0 1";
    i5.solution = { "Qxd4","Qxd4","Nxd4" }; i5.difficulty = PuzzleDifficulty::INTERMEDIATE;
    i5.theme = "Deflection"; i5.description = "Black to move";
    i5.objective = "Win material"; i5.whiteToMove = false; intermediatePuzzles.push_back(i5);

    // ── HARD ──────────────────────────────────────────────
    Puzzle h1; h1.id = 11; h1.fen = "r2qkb1r/ppp2ppp/2n5/3pP3/3Pn1b1/2PB1N2/PP3PPP/RNBQK2R b KQkq - 0 1";
    h1.solution = { "Nxf2","Kxf2","Qh4+","g3","Qxg3#" }; h1.difficulty = PuzzleDifficulty::HARD;
    h1.theme = "Sacrificial Attack"; h1.description = "Black to move";
    h1.objective = "Checkmate the King"; h1.whiteToMove = false; hardPuzzles.push_back(h1);

    Puzzle h2; h2.id = 12; h2.fen = "r1bqk2r/ppp2ppp/2n5/2bpP3/8/2PB1Q2/PP3PPP/RNB1K1NR w KQkq - 0 1";
    h2.solution = { "Qxf7+","Kxf7","Bc4+" }; h2.difficulty = PuzzleDifficulty::HARD;
    h2.theme = "Queen Sacrifice"; h2.description = "White to move";
    h2.objective = "Win material advantage"; h2.whiteToMove = true; hardPuzzles.push_back(h2);

    Puzzle h3; h3.id = 13; h3.fen = "r1bq1rk1/ppp2ppp/2n5/3pPb2/1b1P4/2NB1N2/PPP2PPP/R1BQ1RK1 w - - 0 1";
    h3.solution = { "Bxh7+","Kxh7","Ng5+","Kg8","Qh5" }; h3.difficulty = PuzzleDifficulty::HARD;
    h3.theme = "Greek Gift"; h3.description = "White to move";
    h3.objective = "Win the Queen"; h3.whiteToMove = true; hardPuzzles.push_back(h3);

    Puzzle h4; h4.id = 14; h4.fen = "6k1/5ppp/8/8/8/8/5PPP/R5K1 w - - 0 1";
    h4.solution = { "Ra8+","Kh7","Ra7" }; h4.difficulty = PuzzleDifficulty::HARD;
    h4.theme = "Rook Endgame"; h4.description = "White to move";
    h4.objective = "Win all pawns"; h4.whiteToMove = true; hardPuzzles.push_back(h4);

    Puzzle h5; h5.id = 15; h5.fen = "8/8/4k3/4p3/4K3/8/8/8 w - - 0 1";
    h5.solution = { "Kd3","Kd5","Ke3" }; h5.difficulty = PuzzleDifficulty::HARD;
    h5.theme = "Opposition"; h5.description = "White to move";
    h5.objective = "Win the pawn"; h5.whiteToMove = true; hardPuzzles.push_back(h5);

    // ── ULTRA HARD ────────────────────────────────────────
    Puzzle u1; u1.id = 16; u1.fen = "r1bqk2r/ppp2ppp/2n2n2/2bpp3/2B1P3/2PP1N2/PP3PPP/RNBQ1RK1 b kq - 0 1";
    u1.solution = { "Nxe4","dxe4","Bxf2+","Kxf2","Qh4+","g3","Qxc4+" }; u1.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u1.theme = "Complex Sacrifice"; u1.description = "Black to move";
    u1.objective = "Win the Queen"; u1.whiteToMove = false; ultraHardPuzzles.push_back(u1);

    Puzzle u2; u2.id = 17; u2.fen = "6rk/6pp/7P/8/8/8/8/5Q1K w - - 0 1";
    u2.solution = { "Qf7","Rg7","Qf8+","Rg8","Qxg8#" }; u2.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u2.theme = "Smothered Mate"; u2.description = "White to move";
    u2.objective = "Checkmate the King"; u2.whiteToMove = true; ultraHardPuzzles.push_back(u2);

    Puzzle u3; u3.id = 18; u3.fen = "r2q1rk1/ppp2ppp/2n5/3pPb2/1b1P4/2NB1N2/PPP1QPPP/R1B2RK1 w - - 0 1";
    u3.solution = { "Bxh7+","Kxh7","Ng5+","Kg6","Qg4","f5","Qg3" }; u3.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u3.theme = "Mating Attack"; u3.description = "White to move";
    u3.objective = "Checkmate the King"; u3.whiteToMove = true; ultraHardPuzzles.push_back(u3);

    Puzzle u4; u4.id = 19; u4.fen = "r1bq1rk1/ppp2ppp/2n5/3pPb2/1b1P4/2NB1N2/PPP2PPP/R1BQR1K1 w - - 0 1";
    u4.solution = { "Bxh7+","Kxh7","Ng5+","Kg8","Qh5","Re8","Qxf7+","Kh8","Qh5+","Kg8","Qh7#" };
    u4.difficulty = PuzzleDifficulty::ULTRA_HARD; u4.theme = "Multiple Sacrifices";
    u4.description = "White to move"; u4.objective = "Checkmate the King";
    u4.whiteToMove = true; ultraHardPuzzles.push_back(u4);

    Puzzle u5; u5.id = 20; u5.fen = "8/8/8/4k3/4p3/4K3/4P3/8 w - - 0 1";
    u5.solution = { "Kd3","Kd5","Ke3","Ke5","Kd3" }; u5.difficulty = PuzzleDifficulty::ULTRA_HARD;
    u5.theme = "King and Pawn Endgame"; u5.description = "White to move";
    u5.objective = "Win the game"; u5.whiteToMove = true; ultraHardPuzzles.push_back(u5);
}

// ============================================================
//  getNextPuzzle / startPuzzle / checkMove / completePuzzle
// ============================================================

Puzzle ChessPuzzleSystem::getNextPuzzle(PuzzleDifficulty difficulty) {
    vector<Puzzle>* ps = nullptr;
    switch (difficulty) {
    case PuzzleDifficulty::EASY:        ps = &easyPuzzles;        break;
    case PuzzleDifficulty::INTERMEDIATE:ps = &intermediatePuzzles; break;
    case PuzzleDifficulty::HARD:        ps = &hardPuzzles;         break;
    case PuzzleDifficulty::ULTRA_HARD:  ps = &ultraHardPuzzles;    break;
    }
    if (ps && !ps->empty()) {
        vector<Puzzle> unsolved;
        for (auto& p : *ps)
            if (solvedPuzzles.find(p.id) == solvedPuzzles.end())
                unsolved.push_back(p);
        if (unsolved.empty()) return (*ps)[rand() % ps->size()];
        return unsolved[rand() % unsolved.size()];
    }
    return easyPuzzles[0];
}

Puzzle ChessPuzzleSystem::getCurrentPuzzleByDifficulty(PuzzleDifficulty d) {
    return getNextPuzzle(d);
}

void ChessPuzzleSystem::startPuzzle(const Puzzle& puzzle) {
    if (currentPuzzle) delete currentPuzzle;
    currentPuzzle = new Puzzle(puzzle);
    puzzleStartTime = time(0);
    moveIndex = attempts = hintsUsed = 0;
}

PuzzleResult ChessPuzzleSystem::checkMove(const string& move) {
    PuzzleResult r; r.correct = false; r.isComplete = false;
    if (!currentPuzzle) { r.message = "No active puzzle"; return r; }

    bool ok = (normalizeSAN(move) == normalizeSAN(currentPuzzle->solution[moveIndex]));
    if (ok) {
        moveIndex++;
        if (moveIndex >= (int)currentPuzzle->solution.size())
            return completePuzzle(true);
        r.correct = true; r.message = "Correct! Continue..."; return r;
    }
    attempts++;
    if (attempts >= 3) return completePuzzle(false);
    r.message = "Incorrect. " + to_string(3 - attempts) + " attempts remaining";
    return r;
}

PuzzleResult ChessPuzzleSystem::completePuzzle(bool solved) {
    PuzzleResult r;
    time_t end = time(0);
    int elapsed = (int)difftime(end, puzzleStartTime);

    int diffRating = 800;
    switch (currentPuzzle->difficulty) {
    case PuzzleDifficulty::EASY:        diffRating = 800;  break;
    case PuzzleDifficulty::INTERMEDIATE:diffRating = 1200; break;
    case PuzzleDifficulty::HARD:        diffRating = 1600; break;
    case PuzzleDifficulty::ULTRA_HARD:  diffRating = 2000; break;
    }
    double exp = 1.0 / (1.0 + pow(10.0, (diffRating - userRating) / 400.0));
    int kFactor = 32;
    int ratingChange = (int)round(kFactor * (solved ? 1.0 : 0.0) - kFactor * exp);
    int bonus = 0;

    if (solved) {
        if (elapsed < 30) bonus += 20; else if (elapsed < 60) bonus += 10; else if (elapsed < 120) bonus += 5;
        if (attempts == 0) bonus += 15;
        if (hintsUsed == 0) bonus += 10;
        streakCount = isConsecutiveDay(lastSolvedDate) ? streakCount + 1 : 1;
        lastSolvedDate = time(0);
        solvedPuzzles.insert(currentPuzzle->id);
        r.message = "Puzzle solved!";
    }
    else {
        streakCount = 0;
        r.message = "Puzzle failed. Better luck next time!";
    }

    int oldRating = userRating;
    userRating = max(400, userRating + ratingChange + bonus);

    r.correct = solved; r.isComplete = true; r.timeTaken = elapsed;
    r.attempts = attempts; r.hintsUsed = hintsUsed;
    r.oldRating = oldRating; r.newRating = userRating;
    r.ratingChange = ratingChange + bonus; r.bonusPoints = bonus;
    r.streak = streakCount; r.solution = currentPuzzle->solution;

    delete currentPuzzle; currentPuzzle = nullptr;
    return r;
}

// ============================================================
//  getHint / helpers
// ============================================================

string ChessPuzzleSystem::getHint() {
    if (!currentPuzzle) return "No active puzzle";
    hintsUsed++;
    string cur = currentPuzzle->solution[moveIndex];
    switch (hintsUsed) {
    case 1: return "Theme: " + currentPuzzle->theme;
    case 2: return "Move your " + getPieceFromMove(cur);
    case 3: return "The move ends on " + getDestinationSquare(cur);
    default:return "Move: " + cur;
    }
}

string ChessPuzzleSystem::normalizeSAN(const string& move) {
    string s = move;
    s.erase(remove(s.begin(), s.end(), '+'), s.end());
    s.erase(remove(s.begin(), s.end(), '#'), s.end());
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    return s;
}

string ChessPuzzleSystem::getPieceFromMove(const string& move) {
    if (move.empty()) return "Pawn";
    switch (move[0]) {
    case 'K':return "King"; case 'Q':return "Queen";
    case 'R':return "Rook"; case 'B':return "Bishop";
    case 'N':return "Knight"; default:return "Pawn";
    }
}

string ChessPuzzleSystem::getDestinationSquare(const string& move) {
    for (size_t i = 0; i + 1 < move.length(); i++)
        if (move[i] >= 'a' && move[i] <= 'h' && move[i + 1] >= '1' && move[i + 1] <= '8')
            return move.substr(i, 2);
    return "";
}

bool ChessPuzzleSystem::isConsecutiveDay(time_t lastDate) {
    if (!lastDate) return false;
    double diff = difftime(time(0), lastDate);
    return (int)(diff / 86400) == 1;
}

int ChessPuzzleSystem::getTotalPuzzles() const {
    return (int)(easyPuzzles.size() + intermediatePuzzles.size() +
        hardPuzzles.size() + ultraHardPuzzles.size());
}

int ChessPuzzleSystem::getElapsedTime() const {
    if (!currentPuzzle) return 0;
    return (int)difftime(time(0), puzzleStartTime);
}

int ChessPuzzleSystem::getPuzzleCount(PuzzleDifficulty d) const {
    switch (d) {
    case PuzzleDifficulty::EASY:        return (int)easyPuzzles.size();
    case PuzzleDifficulty::INTERMEDIATE:return (int)intermediatePuzzles.size();
    case PuzzleDifficulty::HARD:        return (int)hardPuzzles.size();
    case PuzzleDifficulty::ULTRA_HARD:  return (int)ultraHardPuzzles.size();
    default:return 0;
    }
}

int ChessPuzzleSystem::getSolvedCountByDifficulty(PuzzleDifficulty d) const {
    const vector<Puzzle>* ps = nullptr;
    switch (d) {
    case PuzzleDifficulty::EASY:        ps = &easyPuzzles;        break;
    case PuzzleDifficulty::INTERMEDIATE:ps = &intermediatePuzzles; break;
    case PuzzleDifficulty::HARD:        ps = &hardPuzzles;         break;
    case PuzzleDifficulty::ULTRA_HARD:  ps = &ultraHardPuzzles;    break;
    }
    if (!ps) return 0;
    int cnt = 0;
    for (auto& p : *ps)
        if (solvedPuzzles.find(p.id) != solvedPuzzles.end()) cnt++;
    return cnt;
}

void ChessPuzzleSystem::resetProgress() {
    userRating = 1200; solvedPuzzles.clear(); streakCount = 0; lastSolvedDate = 0;
    if (currentPuzzle) { delete currentPuzzle; currentPuzzle = nullptr; }
}

void ChessPuzzleSystem::saveProgress() {
    ofstream f("puzzle_progress.dat");
    if (!f) return;
    f << userRating << "\n" << streakCount << "\n" << lastSolvedDate << "\n"
        << solvedPuzzles.size() << "\n";
    for (int id : solvedPuzzles) f << id << " ";
}

void ChessPuzzleSystem::loadProgress() {
    ifstream f("puzzle_progress.dat");
    if (!f) return;
    f >> userRating >> streakCount >> lastSolvedDate;
    int cnt; f >> cnt;
    solvedPuzzles.clear();
    for (int i = 0; i < cnt; i++) { int id; f >> id; solvedPuzzles.insert(id); }
}

// ============================================================
//  UI STRUCT IMPLEMENTATIONS
// ============================================================

// ── PuzzleButton ─────────────────────────────────────────────
void PuzzleButton::setup(const string& txt, Font& font, unsigned cs,
    Color bg, Color hover, Color outline, float thick) {
    normalColor = bg; hoverColor = hover;
    shape.setFillColor(bg);
    shape.setOutlineThickness(thick);
    shape.setOutlineColor(outline);
    label.setFont(font);
    label.setString(txt);
    label.setCharacterSize(cs);
    label.setFillColor(Color::White);
}
static void centreLabel(sf::Text& lbl, const sf::RectangleShape& shp) {
    FloatRect lb = lbl.getLocalBounds();
    FloatRect sb = shp.getGlobalBounds();
    lbl.setPosition(sb.left + (sb.width - lb.width) / 2.f - lb.left,
        sb.top + (sb.height - lb.height) / 2.f - lb.top - 4.f);
}
void PuzzleButton::setPos(float x, float y) {
    shape.setPosition(x, y);
    centreLabel(label, shape);
}
void PuzzleButton::resize(float w, float h) {
    shape.setSize({ w,h });
    centreLabel(label, shape);
}
void PuzzleButton::update(Vector2i mp) {
    bool h = shape.getGlobalBounds().contains((float)mp.x, (float)mp.y);
    shape.setFillColor(h ? hoverColor : normalColor);
    shape.setOutlineColor(h ? Color(255, 215, 0) : shape.getOutlineColor());
}
void PuzzleButton::draw(RenderWindow& win) { win.draw(shape); win.draw(label); }
bool PuzzleButton::contains(Vector2f p) const { return shape.getGlobalBounds().contains(p); }

// ── PuzzleNotification ────────────────────────────────────────
void PuzzleNotification::show(const string& msg, Color c, Font& font, float dur) {
    duration = dur; active = true; clock.restart();
    text.setFont(font); text.setString(msg);
    text.setCharacterSize(19); text.setFillColor(Color::White);
    text.setStyle(Text::Bold);
    box.setFillColor(Color(c.r / 4, c.g / 4, c.b / 4, 230));
    box.setOutlineThickness(2.f); box.setOutlineColor(c);
}
void PuzzleNotification::draw(RenderWindow& win) {
    if (!active) return;
    float t = clock.getElapsedTime().asSeconds();
    float a = (t > duration - 0.5f) ? max(0.f, (duration - t) / 0.5f) : 1.f;
    auto fade = [&](sf::Shape& s) { Color c = s.getFillColor(); c.a = (Uint8)(c.a * a); s.setFillColor(c); };
    auto fadeT = [&]() { Color c = text.getFillColor(); c.a = (Uint8)(255 * a); text.setFillColor(c); };
    fade(box); fadeT();
    win.draw(box); win.draw(text);
    if (t >= duration) active = false;
}

// ── PuzzleStars ───────────────────────────────────────────────
void PuzzleStars::setup(int n, float x, float y, float radius, float gap) {
    count = n; stars.resize(n);
    for (int i = 0; i < n; i++) {
        stars[i].setRadius(radius);
        stars[i].setPointCount(5);
        stars[i].setOrigin(radius, radius);
        stars[i].setPosition(x + i * (radius * 2 + gap), y);
        stars[i].setFillColor(Color(50, 50, 50));
        stars[i].setOutlineThickness(1.5f);
        stars[i].setOutlineColor(Color(180, 150, 0));
    }
}
void PuzzleStars::setRating(int r) {
    for (int i = 0; i < count; i++)
        stars[i].setFillColor(i < r ? Color(255, 215, 0) : Color(50, 50, 50));
}
void PuzzleStars::draw(RenderWindow& win) {
    for (auto& s : stars) win.draw(s);
}

// ── PuzzleMoveLog ─────────────────────────────────────────────
void PuzzleMoveLog::init(Font& f, Vector2f p, float lh) {
    font = &f; pos = p; lineH = lh;
}
void PuzzleMoveLog::add(const string& san, bool correct) {
    if ((int)moves.size() >= maxLines) { moves.erase(moves.begin()); colors.erase(colors.begin()); }
    moves.push_back(san);
    colors.push_back(correct ? Color(50, 200, 80) : Color(220, 60, 60));
}
void PuzzleMoveLog::clear() { moves.clear(); colors.clear(); }
void PuzzleMoveLog::draw(RenderWindow& win) {
    for (int i = 0; i < (int)moves.size(); i++) {
        Text t(moves[i], *font, 15);
        t.setFillColor(colors[i]);
        t.setPosition(pos.x, pos.y + i * lineH);
        win.draw(t);
    }
}

// ============================================================
//  runPuzzleMode
//  Relies on Chess.cpp externs: boardLogic, boardArr, whiteTurn,
//  tileW, tileH, offX, offY, isInsideBoard, isValidMove,
//  makeMove, updateboard, loadBoardFromFEN,
//  drawBoard, drawPieces, moveToSAN
// ============================================================

// Forward-declare the Chess.cpp globals we need
extern char  boardLogic[8][8];
extern int   boardArr[8][8];
extern bool  whiteTurn;
extern float tileW, tileH, offX, offY;

bool        isInsideBoard(int r, int c);
bool        isValidMove(int sx, int sy, int dx, int dy);
void        makeMove(int sx, int sy, int dx, int dy);
void        updateboard();
void        loadBoardFromFEN(const string& fen);
void        drawBoard(RenderWindow& win, RectangleShape& box,
    int dr, int dc, int hr, int hc, Color lt, Color dk);
void        drawPieces(RenderWindow& win, Texture W[], Texture B[], int sr, int sc);
string      moveToSAN(int fr, int fc, int tr, int tc);

// ── Colour palette ────────────────────────────────────────────
// ============================================================
//  DROP-IN REPLACEMENT FOR runPuzzleMode
//  Fixes:
//   1. Piece size  – dynamic scale so pieces fit inside tiles
//   2. Duplicate buttons – positioned once, drawn once
//   3. solutionClock scope – declared BEFORE the while-loop
//   4. Right-panel layout – clean, no overlapping text
// ============================================================

// ── Forward-declares (already present in your file – keep them) ──
extern char  boardLogic[8][8];
extern int   boardArr[8][8];
extern bool  whiteTurn;
extern float tileW, tileH, offX, offY;

bool   isInsideBoard(int r, int c);
bool   isValidMove(int sx, int sy, int dx, int dy);
void   makeMove(int sx, int sy, int dx, int dy);
void   updateboard();
void   loadBoardFromFEN(const string& fen);
void   drawBoard(RenderWindow& win, RectangleShape& box,
    int dr, int dc, int hr, int hc, Color lt, Color dk);
string moveToSAN(int fr, int fc, int tr, int tc);

namespace PC {
    const Color BG(12, 14, 20);
    const Color PANEL(22, 26, 36, 245);
    const Color PANEL_HDR(30, 34, 50);
    const Color GOLD(255, 215, 0);
    const Color GOLD_DIM(180, 150, 0);
    const Color GREEN(50, 200, 80);
    const Color RED(220, 60, 60);
    const Color ORANGE(255, 140, 0);
    const Color CYAN(80, 220, 230);
    const Color MAGENTA(200, 80, 200);
    const Color WHITE(240, 240, 240);
    const Color GREY(130, 130, 130);
    const Color LAST_MOVE(100, 180, 255, 80);
    const Color SEL(255, 215, 0, 100);
    const Color DOT(80, 200, 100, 140);
    const Color RING_OUT(80, 200, 100, 160);
}

// ── Small helpers ─────────────────────────────────────────────
static string pzDiffName(PuzzleDifficulty d) {
    switch (d) {
    case PuzzleDifficulty::EASY:        return "Easy";
    case PuzzleDifficulty::INTERMEDIATE:return "Intermediate";
    case PuzzleDifficulty::HARD:        return "Hard";
    case PuzzleDifficulty::ULTRA_HARD:  return "Ultra Hard";
    } return "";
}
static Color pzDiffColor(PuzzleDifficulty d) {
    switch (d) {
    case PuzzleDifficulty::EASY:        return Color(30, 100, 50, 220);
    case PuzzleDifficulty::INTERMEDIATE:return Color(130, 90, 10, 220);
    case PuzzleDifficulty::HARD:        return Color(130, 50, 10, 220);
    case PuzzleDifficulty::ULTRA_HARD:  return Color(110, 20, 20, 220);
    } return Color(60, 60, 60, 220);
}
static Color pzDiffHover(PuzzleDifficulty d) {
    switch (d) {
    case PuzzleDifficulty::EASY:        return Color(50, 160, 80, 240);
    case PuzzleDifficulty::INTERMEDIATE:return Color(180, 130, 20, 240);
    case PuzzleDifficulty::HARD:        return Color(180, 80, 20, 240);
    case PuzzleDifficulty::ULTRA_HARD:  return Color(180, 40, 40, 240);
    } return Color(80, 80, 80, 240);
}
static Color pzDiffOutline(PuzzleDifficulty d) {
    switch (d) {
    case PuzzleDifficulty::EASY:        return Color(50, 200, 80);
    case PuzzleDifficulty::INTERMEDIATE:return Color(255, 200, 0);
    case PuzzleDifficulty::HARD:        return Color(255, 140, 0);
    case PuzzleDifficulty::ULTRA_HARD:  return Color(220, 60, 60);
    } return Color(130, 130, 130);
}
static string pzFmtTime(int sec) {
    char buf[16]; sprintf(buf, "%02d:%02d", sec / 60, sec % 60); return buf;
}
static Text pzMkText(const string& s, Font& f, unsigned cs,
    Color c, float x, float y, bool bold = false) {
    Text t(s, f, cs); t.setFillColor(c); t.setPosition(x, y);
    if (bold) t.setStyle(Text::Bold); return t;
}
static void pzSep(RenderWindow& win, float x, float y, float w,
    Color c = Color(50, 50, 70)) {
    RectangleShape s({ w,1.f }); s.setPosition(x, y); s.setFillColor(c); win.draw(s);
}
static void pzSquare(RenderWindow& win, int row, int col, Color c) {
    RectangleShape s({ tileW,tileH });
    s.setPosition(offX + col * tileW, offY + row * tileH);
    s.setFillColor(c); win.draw(s);
}
static vector<string> pzWrap(const string& txt, Font& font,
    unsigned cs, float maxW) {
    vector<string> lines;
    istringstream ss(txt); string word, line;
    while (ss >> word) {
        string test = line.empty() ? word : line + " " + word;
        if (!line.empty() &&
            Text(test, font, cs).getLocalBounds().width > maxW) {
            lines.push_back(line); line = word;
        }
        else line = test;
    }
    if (!line.empty()) lines.push_back(line);
    return lines;
}
static int pzCalcStars(const PuzzleResult& r) {
    if (!r.correct) return 0;
    if (r.attempts == 0 && r.hintsUsed == 0 && r.timeTaken < 60) return 3;
    if (r.attempts <= 1 && r.hintsUsed == 0) return 2;
    return 1;
}

// ============================================================
void runPuzzleMode(RenderWindow& window, ChessPuzzleSystem& ps)
{
    // ── Textures ──────────────────────────────────────────────
    Texture texW[6], texB[6];
    const char* wf[] = { "pieces/white-pawn.png","pieces/white-rook.png",
                      "pieces/white-knight.png","pieces/white-bishop.png",
                      "pieces/white-queen.png","pieces/white-king.png" };
    const char* bf[] = { "pieces/black-pawn.png","pieces/black-rook.png",
                      "pieces/black-knight.png","pieces/black-bishop.png",
                      "pieces/black-queen.png","pieces/black-king.png" };
    for (int i = 0; i < 6; i++) { texW[i].loadFromFile(wf[i]); texB[i].loadFromFile(bf[i]); }

    // ── Fonts ─────────────────────────────────────────────────
    Font fontUI, fontTitle;
    if (!fontUI.loadFromFile("Font/roboto.ttf")) fontUI.loadFromFile("Font/arial.ttf");
    if (!fontTitle.loadFromFile("Font/bebasneue.ttf")) fontTitle = fontUI;

    // ── Sounds ────────────────────────────────────────────────
    SoundBuffer sbMove, sbWrong, sbSolve, sbFail, sbHint;
    sbMove.loadFromFile("audio/move.wav");
    sbWrong.loadFromFile("audio/capture.wav");
    sbSolve.loadFromFile("audio/Completing.wav");
    sbFail.loadFromFile("audio/stalemate.wav");
    sbHint.loadFromFile("audio/castling.wav");
    Sound sndMove(sbMove), sndWrong(sbWrong), sndSolve(sbSolve),
        sndFail(sbFail), sndHint(sbHint);

    // ── State ─────────────────────────────────────────────────
    PuzzleDifficulty selDiff = PuzzleDifficulty::EASY;
    Puzzle cur = ps.getNextPuzzle(selDiff);
    ps.startPuzzle(cur);
    loadBoardFromFEN(cur.fen);
    whiteTurn = cur.whiteToMove;

    bool pzDone = false, pzWon = false, showSol = false;
    int  solStep = 0;
    Clock solutionClock;          // FIX 1 – OUTSIDE the while loop

    bool  dragging = false;
    int   dragR = -1, dragC = -1, hoverR = -1, hoverC = -1;
    Sprite dragSpr;
    float  dragOX = 0, dragOY = 0;
    int   lastFR = -1, lastFC = -1, lastTR = -1, lastTC = -1;

    bool  aiWait = false;
    Clock aiClock;
    const float AI_DELAY = 1.0f;

    PuzzleNotification notif;
    PuzzleMoveLog      moveLog;
    PuzzleStars        stars;
    PuzzleButton       btnBack, btnHint, btnNext, btnReset, btnSolve, btnDiff[4];
    bool uiReady = false;
    RectangleShape boardBox;

    // ── FIX 2 – Local piece-drawing lambda: dynamic tile-fitted scale ──
    auto drawPiecesLocal = [&](int skipR, int skipC) {
        for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) {
            if (r == skipR && c == skipC) continue;
            int id = boardArr[r][c];
            if (id == 0) continue;
            Texture& tex = (id > 0) ? texW[id - 1] : texB[-id - 1];
            Vector2u ts = tex.getSize();
            if (ts.x == 0) continue;
            // Scale piece so it fills 88% of the tile
            float scale = (tileW * 0.88f) / (float)ts.x;
            float ox = (tileW - ts.x * scale) / 2.f;
            float oy = (tileH - ts.y * scale) / 2.f;
            Sprite s(tex);
            s.setScale(scale, scale);
            s.setPosition(offX + c * tileW + ox, offY + r * tileH + oy);
            window.draw(s);
        }
        };

    // ── Load-puzzle helper ────────────────────────────────────
    auto loadPuzzle = [&](PuzzleDifficulty d) {
        selDiff = d; cur = ps.getNextPuzzle(d);
        ps.startPuzzle(cur);
        loadBoardFromFEN(cur.fen);
        whiteTurn = cur.whiteToMove;
        pzDone = pzWon = showSol = false; solStep = 0;
        solutionClock.restart();
        dragging = aiWait = false;
        lastFR = lastFC = lastTR = lastTC = -1;
        moveLog.clear(); stars.setRating(0);
        };

    // ─────────────────────────────────────────────────────────
    //  MAIN LOOP
    // ─────────────────────────────────────────────────────────
    while (window.isOpen()) {

        // ── 1. Layout ─────────────────────────────────────────
        const float W = (float)window.getSize().x;
        const float H = (float)window.getSize().y;

        // Board: leave room for side panels and top/bottom labels
        float board = min(H * 0.86f, W * 0.56f);
        board = max(board, 400.f);
        tileW = tileH = board / 8.f;
        offX = (W - board) / 2.f;
        offY = (H - board) / 2.f;

        const float LP = 12.f;
        const float PANW = min(offX - 16.f, 275.f);
        const float LX = max(4.f, offX - PANW - 8.f);
        const float RX = offX + board + 8.f;
        const float RW = min(PANW, W - RX - 4.f);

        // Build UI widgets once
        if (!uiReady) {
            uiReady = true;
            moveLog.init(fontUI, { LX + LP,0 });
            stars.setup(3, 0, 0, 11.f, 5.f);

            btnBack.setup("< BACK", fontTitle, 20, Color(100, 30, 30, 220), Color(180, 50, 50, 240), Color(200, 60, 60));
            btnHint.setup("Get Hint", fontUI, 17, Color(40, 50, 140, 220), Color(70, 80, 200, 240), PC::CYAN);
            btnNext.setup("Next Puzzle", fontUI, 15, Color(25, 100, 45, 220), Color(45, 160, 75, 240), PC::GREEN);
            btnReset.setup("Reset", fontUI, 15, Color(70, 55, 15, 220), Color(130, 100, 25, 240), PC::GOLD_DIM);
            btnSolve.setup("Solution", fontUI, 15, Color(65, 25, 65, 220), Color(120, 45, 120, 240), PC::MAGENTA);

            const char* dn[4] = { "Easy","Inter","Hard","Ultra" };
            for (int i = 0; i < 4; i++) {
                auto d = static_cast<PuzzleDifficulty>(i);
                btnDiff[i].setup(dn[i], fontUI, 15,
                    pzDiffColor(d), pzDiffHover(d), pzDiffOutline(d));
            }
        }

        // ── 2. Position ALL buttons (for hit-testing this frame) ──
        btnBack.resize(110, 34); btnBack.setPos(LX, 12.f);

        const float LY = 54.f, LH = H - LY - 14.f;

        // Right panel: buttons positioned relative to where they'll be drawn
        const float diffBW = (RW - LP * 3.f) / 2.f;
        const float diffBH = 34.f;
        const float abW = (RW - LP * 3.f) / 2.f;

        // Approximate y positions matching the render section below
        // (right panel content starts at y=90)
        float rpBtnY = 90.f
            + 16.f   // "Objective" label
            + 40.f   // objective text (up to 2 lines)
            + 4.f    // gap
            + 7.f    // separator
            + 20.f   // Theme text
            + 20.f   // Description text
            + 28.f   // Turn pill
            + 14.f   // separator + gap
            + 16.f;  // "Difficulty" label height

        for (int i = 0; i < 4; i++) {
            btnDiff[i].resize(diffBW, diffBH);
            btnDiff[i].setPos(RX + LP + (i % 2) * (diffBW + LP), rpBtnY + (i / 2) * 42.f);
        }
        float actY = rpBtnY + 90.f + 8.f; // after diff grid + separator
        btnHint.resize(abW, 36); btnHint.setPos(RX + LP, actY);
        btnNext.resize(abW, 36); btnNext.setPos(RX + LP * 2 + abW, actY);
        btnReset.resize(abW, 32); btnReset.setPos(RX + LP, actY + 44.f);
        btnSolve.resize(abW, 32); btnSolve.setPos(RX + LP * 2 + abW, actY + 44.f);

        // Notification box
        notif.box.setSize({ board - 20.f,44.f });
        notif.box.setPosition(offX + 10.f, offY + board - 50.f);
        notif.text.setPosition(offX + 20.f, offY + board - 44.f);

        // ── 3. AI wait ────────────────────────────────────────
        if (aiWait && aiClock.getElapsedTime().asSeconds() >= AI_DELAY) {
            whiteTurn = !whiteTurn; aiWait = false;
        }

        // ── 4. Solution step (FIX: solutionClock is outside loop) ──
        if (showSol && !cur.solution.empty() &&
            solutionClock.getElapsedTime().asSeconds() > 1.4f) {
            solutionClock.restart();
            if (solStep < (int)cur.solution.size()) solStep++;
        }

        // ── 5. Events ─────────────────────────────────────────
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) { ps.saveProgress(); window.close(); return; }

            if (ev.type == Event::MouseButtonPressed &&
                ev.mouseButton.button == Mouse::Left) {

                Vector2f pos((float)ev.mouseButton.x, (float)ev.mouseButton.y);

                if (btnBack.contains(pos)) { ps.saveProgress(); return; }

                for (int i = 0; i < 4; i++)
                    if (btnDiff[i].contains(pos)) {
                        loadPuzzle(static_cast<PuzzleDifficulty>(i)); break;
                    }

                if (btnHint.contains(pos) && !pzDone && !aiWait) {
                    sndHint.play();
                    notif.show("Hint: " + ps.getHint(), PC::CYAN, fontUI, 4.f);
                }
                if (btnNext.contains(pos)) { sndMove.play(); loadPuzzle(selDiff); }

                if (btnReset.contains(pos) && !pzDone && !aiWait) {
                    loadBoardFromFEN(cur.fen);
                    whiteTurn = cur.whiteToMove;
                    lastFR = lastFC = lastTR = lastTC = -1;
                    moveLog.clear(); aiWait = false;
                    notif.show("Board Reset", PC::ORANGE, fontUI, 1.8f);
                }
                if (btnSolve.contains(pos)) {
                    showSol = true; solStep = 0; solutionClock.restart();
                    if (!pzDone) { ps.completePuzzle(false); pzDone = true; pzWon = false; }
                    notif.show("Showing solution...", PC::MAGENTA, fontUI, 4.f);
                }

                // Drag start
                if (!pzDone && !aiWait && !showSol) {
                    int c = (int)((pos.x - offX) / tileW);
                    int r = (int)((pos.y - offY) / tileH);
                    if (isInsideBoard(r, c) && boardArr[r][c] != 0) {
                        char p = boardLogic[r][c];
                        bool mine = (cur.whiteToMove && isupper(p)) ||
                            (!cur.whiteToMove && islower(p));
                        bool turn = (whiteTurn && isupper(p)) ||
                            (!whiteTurn && islower(p));
                        if (mine && turn) {
                            dragging = true; dragR = r; dragC = c;
                            int id = boardArr[r][c];
                            Texture& tex = (id > 0) ? texW[id - 1] : texB[-id - 1];
                            Vector2u ts = tex.getSize();
                            float sc = (tileW * 0.88f) / (float)ts.x;
                            dragSpr.setTexture(tex);
                            dragSpr.setScale(sc, sc);
                            dragOX = pos.x - (c * tileW + offX);
                            dragOY = pos.y - (r * tileH + offY);
                        }
                    }
                }
            }

            if (ev.type == Event::MouseButtonReleased &&
                ev.mouseButton.button == Mouse::Left && dragging) {
                dragging = false;
                int c = (int)(((float)ev.mouseButton.x - offX) / tileW);
                int r = (int)(((float)ev.mouseButton.y - offY) / tileH);

                if (isInsideBoard(r, c) && isValidMove(dragR, dragC, r, c)) {
                    string san = moveToSAN(dragR, dragC, r, c);
                    makeMove(dragR, dragC, r, c); updateboard();
                    lastFR = dragR; lastFC = dragC; lastTR = r; lastTC = c;

                    PuzzleResult res = ps.checkMove(san);

                    if (res.isComplete) {
                        pzDone = true; pzWon = res.correct;
                        if (res.correct) {
                            sndSolve.play();
                            stars.setRating(pzCalcStars(res));
                            int s3 = pzCalcStars(res);
                            std::string ss =
                                (s3 == 3 ? std::string("*** ") : "") +
                                (s3 == 2 ? std::string("** ") : "") +
                                (s3 == 1 ? std::string("* ") : "");
                            notif.show("SOLVED! " + ss + "+" + to_string(res.ratingChange) + " pts",
                                PC::GREEN, fontUI, 5.f);
                        }
                        else {
                            sndFail.play(); stars.setRating(0);
                            notif.show("Puzzle Failed  " + to_string(res.ratingChange) + " pts",
                                PC::RED, fontUI, 4.f);
                        }
                        moveLog.add(san + (res.correct ? " OK" : " X"), res.correct);
                    }
                    else if (res.correct) {
                        sndMove.play();
                        moveLog.add(san + " OK", true);
                        notif.show("Correct! Keep going...", PC::GREEN, fontUI, 1.5f);
                        whiteTurn = !whiteTurn; aiWait = true; aiClock.restart();
                    }
                    else {
                        sndWrong.play();
                        moveLog.add(san + " X", false);
                        int rem = ps.getRemainingAttempts();
                        notif.show("Wrong move - " + to_string(rem) + " attempts left",
                            PC::RED, fontUI, 2.2f);
                        loadBoardFromFEN(cur.fen);
                        whiteTurn = cur.whiteToMove;
                        lastFR = lastFC = lastTR = lastTC = -1;
                    }
                }
                dragR = dragC = -1;
            }

            if (ev.type == Event::MouseMoved) {
                hoverC = (int)((ev.mouseMove.x - offX) / tileW);
                hoverR = (int)((ev.mouseMove.y - offY) / tileH);
            }
        }

        // ── 6. Hover colours ──────────────────────────────────
        Vector2i mp2 = Mouse::getPosition(window);
        btnBack.update(mp2); btnHint.update(mp2); btnNext.update(mp2);
        btnReset.update(mp2); btnSolve.update(mp2);
        for (int i = 0; i < 4; i++) {
            btnDiff[i].update(mp2);
            if (static_cast<PuzzleDifficulty>(i) == selDiff)
                btnDiff[i].shape.setOutlineColor(PC::GOLD);
        }

        // ─────────────────────────────────────────────────────
        //  RENDER
        // ─────────────────────────────────────────────────────
        window.clear(PC::BG);

        // Board tiles
        drawBoard(window, boardBox,
            dragging ? dragR : -1, dragging ? dragC : -1,
            hoverR, hoverC,
            Color(235, 213, 178), Color(175, 135, 95));

        // Last-move highlight
        if (lastFR >= 0) {
            pzSquare(window, lastFR, lastFC, PC::LAST_MOVE);
            pzSquare(window, lastTR, lastTC, PC::LAST_MOVE);
        }
        // Selected square
        if (dragging && dragR >= 0) pzSquare(window, dragR, dragC, PC::SEL);

        // Valid-move indicators
        if (dragging) {
            for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) {
                if (!isValidMove(dragR, dragC, r, c)) continue;
                float cx = offX + c * tileW + tileW / 2.f;
                float cy = offY + r * tileH + tileH / 2.f;
                if (boardLogic[r][c] != ' ') {
                    CircleShape ring(tileW / 2.f - 3.f);
                    ring.setOrigin(ring.getRadius(), ring.getRadius());
                    ring.setPosition(cx, cy);
                    ring.setFillColor(Color::Transparent);
                    ring.setOutlineThickness(5.f);
                    ring.setOutlineColor(PC::RING_OUT);
                    window.draw(ring);
                }
                else {
                    CircleShape dot(tileW / 6.f);
                    dot.setOrigin(dot.getRadius(), dot.getRadius());
                    dot.setPosition(cx, cy);
                    dot.setFillColor(PC::DOT);
                    window.draw(dot);
                }
            }
        }

        // FIX 2 – pieces drawn with correct tile-fitted scale
        drawPiecesLocal(dragging ? dragR : -1, dragging ? dragC : -1);

        // Board-edge labels
        for (int r = 0; r < 8; r++) {
            Text t(string(1, char('8' - r)), fontUI, 12);
            t.setFillColor(PC::GREY);
            t.setPosition(offX - 15.f, offY + r * tileH + tileH / 2.f - 8.f);
            window.draw(t);
        }
        for (int c = 0; c < 8; c++) {
            Text t(string(1, char('a' + c)), fontUI, 12);
            t.setFillColor(PC::GREY);
            t.setPosition(offX + c * tileW + tileW / 2.f - 5.f, offY + board + 4.f);
            window.draw(t);
        }

        // Dragged piece follows cursor
        if (dragging) {
            dragSpr.setPosition((float)mp2.x - dragOX, (float)mp2.y - dragOY);
            window.draw(dragSpr);
        }

        // Solution step text (above board)
        if (showSol && solStep > 0 && solStep <= (int)cur.solution.size()) {
            Text st("Step " + to_string(solStep) + ": " + cur.solution[solStep - 1],
                fontTitle, 24);
            st.setFillColor(PC::MAGENTA); st.setStyle(Text::Bold);
            st.setPosition(offX + board / 2.f - st.getLocalBounds().width / 2.f,
                offY - 38.f);
            window.draw(st);
        }

        // Banner above board
        if (!showSol) {
            string banner; Color bc;
            if (pzDone) {
                banner = pzWon ? "Solved!" : "Failed";
                bc = pzWon ? PC::GREEN : PC::RED;
            }
            else if (aiWait) { banner = "..."; bc = PC::GREY; }
            else {
                banner = (cur.whiteToMove ? "White" : "Black");
                banner += " to Move";
                bc = cur.whiteToMove ? PC::WHITE : PC::CYAN;
            }
            Text bt(banner, fontTitle, 24);
            bt.setFillColor(bc); bt.setStyle(Text::Bold);
            bt.setPosition(offX + board / 2.f - bt.getLocalBounds().width / 2.f,
                offY - 38.f);
            window.draw(bt);
        }

        // ── LEFT PANEL ────────────────────────────────────────
        {
            RectangleShape pb({ PANW,LH }); pb.setPosition(LX, LY);
            pb.setFillColor(PC::PANEL);
            pb.setOutlineThickness(1.5f); pb.setOutlineColor(PC::GOLD_DIM);
            window.draw(pb);

            RectangleShape ph({ PANW,36.f }); ph.setPosition(LX, LY);
            ph.setFillColor(PC::PANEL_HDR); window.draw(ph);
            window.draw(pzMkText("PUZZLE STATUS", fontTitle, 20, PC::GOLD, LX + LP, LY + 8.f));

            float y = LY + 46.f;

            // Rating
            window.draw(pzMkText("Rating", fontUI, 12, PC::GREY, LX + LP, y));
            Text rv(to_string(ps.getRating()), fontTitle, 28);
            rv.setFillColor(PC::CYAN);
            rv.setPosition(LX + PANW - LP - rv.getLocalBounds().width, y - 4.f);
            window.draw(rv); y += 34.f;

            // Streak
            window.draw(pzMkText("Streak", fontUI, 12, PC::GREY, LX + LP, y));
            int stk = ps.getStreak();
            Color sc2 = stk >= 7 ? PC::GOLD : (stk >= 3 ? PC::ORANGE : PC::GREEN);
            Text sv(to_string(stk) + (stk == 1 ? " day" : " days"), fontTitle, 20);
            sv.setFillColor(sc2);
            sv.setPosition(LX + PANW - LP - sv.getLocalBounds().width, y + 2.f);
            window.draw(sv); y += 30.f;

            pzSep(window, LX + LP, y, PANW - LP * 2); y += 8.f;

            // Timer
            window.draw(pzMkText("Time", fontUI, 12, PC::GREY, LX + LP, y));
            string ts = pzFmtTime(pzDone ? 0 : ps.getElapsedTime());
            Text tv(ts, fontTitle, 28);
            tv.setFillColor(pzDone ? PC::GREY : PC::GOLD);
            tv.setPosition(LX + PANW - LP - tv.getLocalBounds().width, y - 4.f);
            window.draw(tv); y += 34.f;

            // Attempts pips
            window.draw(pzMkText("Attempts", fontUI, 12, PC::GREY, LX + LP, y));
            int rem = ps.getRemainingAttempts();
            Color ac = rem <= 1 ? PC::RED : (rem == 2 ? PC::ORANGE : PC::GREEN);
            for (int i = 0; i < 3; i++) {
                RectangleShape pip({ 14.f,14.f });
                pip.setPosition(LX + PANW - LP - 3 * 18.f + i * 18.f, y + 3.f);
                pip.setFillColor(i < rem ? ac : Color(55, 55, 55));
                pip.setOutlineThickness(1.f); pip.setOutlineColor(ac);
                window.draw(pip);
            } y += 30.f;

            pzSep(window, LX + LP, y, PANW - LP * 2); y += 8.f;

            // Progress bar
            int slvD = ps.getSolvedCountByDifficulty(selDiff);
            int totD = ps.getPuzzleCount(selDiff);
            string pstr = pzDiffName(selDiff) + " " + to_string(slvD) + "/" + to_string(totD);
            window.draw(pzMkText(pstr, fontUI, 12, PC::GREY, LX + LP, y)); y += 15.f;
            float bw = PANW - LP * 2;
            RectangleShape btr({ bw,7.f }); btr.setPosition(LX + LP, y);
            btr.setFillColor(Color(45, 45, 45));
            btr.setOutlineThickness(1.f); btr.setOutlineColor(PC::GREY);
            window.draw(btr);
            float frac = totD ? (float)slvD / totD : 0.f;
            RectangleShape bfl({ bw * frac,7.f }); bfl.setPosition(LX + LP, y);
            bfl.setFillColor(pzDiffOutline(selDiff)); window.draw(bfl); y += 20.f;

            pzSep(window, LX + LP, y, PANW - LP * 2); y += 8.f;

            // Stars
            window.draw(pzMkText("Last Result", fontUI, 12, PC::GREY, LX + LP, y));
            for (int i = 0; i < 3; i++)
                stars.stars[i].setPosition(LX + LP + 12.f + i * 29.f, y + 13.f);
            stars.draw(window); y += 42.f;

            pzSep(window, LX + LP, y, PANW - LP * 2); y += 5.f;
            window.draw(pzMkText("Move Log", fontUI, 12, PC::GREY, LX + LP, y)); y += 18.f;
            moveLog.pos = { LX + LP,y };
            moveLog.draw(window);

            // Solved / failed badge at panel bottom
            if (pzDone) {
                RectangleShape badge({ PANW - LP * 2,32.f });
                badge.setPosition(LX + LP, LY + LH - 40.f);
                badge.setFillColor(pzWon ? Color(25, 70, 35, 230) : Color(70, 25, 25, 230));
                badge.setOutlineThickness(2.f);
                badge.setOutlineColor(pzWon ? PC::GREEN : PC::RED);
                window.draw(badge);
                Text bt2(pzWon ? "PUZZLE SOLVED" : "PUZZLE FAILED", fontTitle, 18);
                bt2.setFillColor(pzWon ? PC::GREEN : PC::RED);
                bt2.setPosition(LX + LP + (PANW - LP * 2 - bt2.getLocalBounds().width) / 2.f,
                    LY + LH - 36.f);
                window.draw(bt2);
            }
        }
        btnBack.draw(window);

        // ── RIGHT PANEL ───────────────────────────────────────
        // FIX 3 – every button is positioned ONCE and drawn ONCE
        {
            float rpH = H - 46.f - 14.f;
            RectangleShape pb({ RW,rpH }); pb.setPosition(RX, 46.f);
            pb.setFillColor(PC::PANEL);
            pb.setOutlineThickness(1.5f);
            pb.setOutlineColor(Color(70, 70, 110, 200)); window.draw(pb);

            RectangleShape ph({ RW,36.f }); ph.setPosition(RX, 46.f);
            ph.setFillColor(Color(38, 32, 58)); window.draw(ph);
            Text ht("CHESS PUZZLE", fontTitle, 20); ht.setFillColor(PC::GOLD);
            ht.setPosition(RX + RW / 2.f - ht.getLocalBounds().width / 2.f, 52.f);
            window.draw(ht);

            float y = 90.f;

            // Objective
            window.draw(pzMkText("Objective", fontUI, 12, PC::GREY, RX + LP, y)); y += 16.f;
            for (auto& ln : pzWrap(cur.objective, fontUI, 16, RW - LP * 2)) {
                window.draw(pzMkText(ln, fontUI, 16, Color(255, 100, 100), RX + LP, y, true));
                y += 20.f;
            }
            y += 4.f;

            pzSep(window, RX + LP, y, RW - LP * 2); y += 7.f;

            // Theme
            window.draw(pzMkText("Theme: " + cur.theme, fontUI, 14, PC::ORANGE, RX + LP, y));
            y += 20.f;

            // Description / who to move
            window.draw(pzMkText(cur.description, fontUI, 13, PC::WHITE, RX + LP, y));
            y += 20.f;

            // Turn pill
            bool wm = cur.whiteToMove;
            RectangleShape pill({ RW - LP * 2,20.f }); pill.setPosition(RX + LP, y);
            pill.setFillColor(wm ? Color(220, 220, 190, 55) : Color(25, 25, 45, 55));
            pill.setOutlineThickness(1.f);
            pill.setOutlineColor(wm ? PC::WHITE : PC::GREY);
            window.draw(pill);
            Text tt(string(wm ? "White" : "Black") + " to move", fontUI, 13);
            tt.setFillColor(wm ? PC::WHITE : PC::GREY);
            tt.setPosition(RX + LP + 5.f, y + 3.f); window.draw(tt); y += 28.f;

            pzSep(window, RX + LP, y, RW - LP * 2); y += 7.f;

            // Difficulty label
            window.draw(pzMkText("Difficulty", fontUI, 12, PC::GREY, RX + LP, y));
            y += 16.f;

            // Difficulty buttons – position AND draw HERE (only once)
            const float dBW = (RW - LP * 3.f) / 2.f;
            const float dBH = 34.f;
            for (int i = 0; i < 4; i++) {
                btnDiff[i].resize(dBW, dBH);
                btnDiff[i].setPos(RX + LP + (i % 2) * (dBW + LP), y + (i / 2) * 42.f);
                btnDiff[i].draw(window);
            }
            y += 90.f;  // 2 rows × 42px + small gap

            pzSep(window, RX + LP, y, RW - LP * 2); y += 8.f;

            // Action buttons – position AND draw HERE (only once)
            const float aW = (RW - LP * 3.f) / 2.f;
            btnHint.resize(aW, 36); btnHint.setPos(RX + LP, y);
            btnNext.resize(aW, 36); btnNext.setPos(RX + LP * 2 + aW, y);
            y += 44.f;
            btnReset.resize(aW, 32); btnReset.setPos(RX + LP, y);
            btnSolve.resize(aW, 32); btnSolve.setPos(RX + LP * 2 + aW, y);

            btnHint.draw(window); btnNext.draw(window);
            btnReset.draw(window); btnSolve.draw(window);

            // Solved count (bottom of right panel)
            float bot = 46.f + rpH - 34.f;
            pzSep(window, RX + LP, bot, RW - LP * 2);
            string sc = "Solved: " + to_string(ps.getSolvedCount()) +
                " / " + to_string(ps.getTotalPuzzles());
            Text sct(sc, fontUI, 13); sct.setFillColor(PC::GREY);
            sct.setPosition(RX + RW / 2.f - sct.getLocalBounds().width / 2.f, bot + 5.f);
            window.draw(sct);
        }

        // Notification overlay (topmost)
        notif.draw(window);
        window.display();
    }
}
