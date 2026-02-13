#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <cctype>
#include <cstring>
#include <stack>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include "ChessPuzzleSystem.h"

using namespace sf;
using namespace std;



SoundBuffer capture;
SoundBuffer castling;
SoundBuffer stalemate;
SoundBuffer checkmate;
SoundBuffer moveBuffer;
SoundBuffer startBuffer;
SoundBuffer completingBuffer;

Sound captureSound;
Music backgroundSound;
Sound castlingSound;
Sound stalemateSound;
Sound checkmateSound;
Sound moveSound;
Sound startSound;
Sound completingSound;

bool gameOver = false;
bool whiteKingMoved = false;
bool blackKingMoved = false;
bool whiteRookLeftMoved = false;
bool whiteRookRightMoved = false;
bool blackRookLeftMoved = false;
bool blackRookRightMoved = false;

// ==============
//  CHESS LOGIC
// ==============

const int SIZE = 8;
char boardLogic[SIZE][SIZE];
bool whiteTurn = true;

// =========================
//  EN PASSANT VARIABLES
// =========================
int enPassantCol = -1;
int enPassantRow = -1;

Color lightColor = Color::White;
Color darkColor = Color(100, 160, 100);


char whiteCaptured[16];   // stores captured white pieces
char blackCaptured[16];   // stores captured black pieces
int whiteCapCount = 0;    // number of captured white pieces
int blackCapCount = 0;    // number of captured white pieces

// =============Globel variables for AI===========
char aiBoard[8][8];
sf::Clock aiThinkClock;
const float AI_TIME_LIMIT = 1.8f; 

bool AIenabled = false;   // whether AI is enabled
bool AIisWhite = false;   // if AI is white (true) or black (false)

bool isValidMove(int sx, int sy, int dx, int dy);
bool isValidPawnMove(int sx, int sy, int dx, int dy, char piece);
bool isvalidRookmove(int sx, int sy, int dx, int dy, char p);
bool isvalidKinghtmove(int sx, int sy, int dx, int dy, char p);
bool isValidBishopMove(int sx, int sy, int dx, int dy, char p);
bool isValidQueenMove(int sx, int sy, int dx, int dy, char p);
bool isvalidKingmove(int sx, int sy, int dx, int dy, char p);
bool kingexpose(int sx, int sy, int dx, int dy);
void updateboard();
void findKing(char kingChar, int& kx, int& ky);
bool isSquareAttacked(int dx, int dy, bool byWhite);
bool isInCheck(bool whiteTurn);
bool hasAnyLegalMove(bool turn);
bool isCheckmate(bool whiteTurn);
bool isStalemate(bool whiteTurn);
bool kingExists(bool white);


struct Move
{
    int sx, sy, dx, dy;
    Move() : sx(-1), sy(-1), dx(-1), dy(-1) {}
    Move(int a, int b, int c, int d) : sx(a), sy(b), dx(c), dy(d) {}
};
struct GameState {
    char board[8][8];
    bool whiteTurn;

    // Castling
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookLeftMoved;
    bool whiteRookRightMoved;
    bool blackRookLeftMoved;
    bool blackRookRightMoved;

    // En Passant
    int enPassantRow;
    int enPassantCol;

    // Captured pieces
    char whiteCaptured[16];
    char blackCaptured[16];
    int whiteCapCount;
    int blackCapCount;
};
std::stack<GameState> undoStack;
std::stack<GameState> redoStack;
GameState captureGameState() {
    GameState s;
    memcpy(s.board, boardLogic, sizeof(boardLogic));

    s.whiteTurn = whiteTurn;
    s.whiteKingMoved = whiteKingMoved;
    s.blackKingMoved = blackKingMoved;
    s.whiteRookLeftMoved = whiteRookLeftMoved;
    s.whiteRookRightMoved = whiteRookRightMoved;
    s.blackRookLeftMoved = blackRookLeftMoved;
    s.blackRookRightMoved = blackRookRightMoved;

    s.enPassantRow = enPassantRow;
    s.enPassantCol = enPassantCol;

    memcpy(s.whiteCaptured, whiteCaptured, sizeof(whiteCaptured));
    memcpy(s.blackCaptured, blackCaptured, sizeof(blackCaptured));
    s.whiteCapCount = whiteCapCount;
    s.blackCapCount = blackCapCount;

    return s;
}
void restoreGameState(const GameState& s) {
    memcpy(boardLogic, s.board, sizeof(boardLogic));
    whiteTurn = s.whiteTurn;
    whiteKingMoved = s.whiteKingMoved;
    blackKingMoved = s.blackKingMoved;
    whiteRookLeftMoved = s.whiteRookLeftMoved;
    whiteRookRightMoved = s.whiteRookRightMoved;
    blackRookLeftMoved = s.blackRookLeftMoved;
    blackRookRightMoved = s.blackRookRightMoved;

    enPassantRow = s.enPassantRow;
    enPassantCol = s.enPassantCol;

    memcpy(whiteCaptured, s.whiteCaptured, sizeof(whiteCaptured));
    memcpy(blackCaptured, s.blackCaptured, sizeof(blackCaptured));
    whiteCapCount = s.whiteCapCount;
    blackCapCount = s.blackCapCount;

    updateboard();
}
void recordStateBeforeMove() {
    undoStack.push(captureGameState());
    while (!redoStack.empty()) redoStack.pop();
}
void undoMove(bool aiThinking) {
    if (aiThinking) return;
    if (undoStack.empty()) return;

    redoStack.push(captureGameState());
    GameState prev = undoStack.top();
    undoStack.pop();
    restoreGameState(prev);
}
void redoMove(bool aiThinking) {
    if (aiThinking) return;
    if (redoStack.empty()) return;

    undoStack.push(captureGameState());
    GameState next = redoStack.top();
    redoStack.pop();
    restoreGameState(next);
}


bool isInsideBoard(int r, int c)
{
    if (r >= 0 && r < 8 && c >= 0 && c < 8) // tackle the Pieces must not go outside board
        return true;

    return false;
}
void initializeBoardLogic()            //Set Character Board
{

    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            boardLogic[row][col] = ' ';
        }
    }
    for (int i = 0; i < SIZE; i++)
    {
        boardLogic[6][i] = 'P'; // White-pawn at 2nd Row from Bottom
        boardLogic[1][i] = 'p'; // Black-pawn at 2nd Row from Top
    }
    // for White Pieces (Upper-Case)
    boardLogic[7][4] = 'K';
    boardLogic[7][5] = 'B';
    boardLogic[7][6] = 'N';
    boardLogic[7][7] = 'R';
    boardLogic[7][0] = 'R';
    boardLogic[7][1] = 'N';
    boardLogic[7][2] = 'B';
    boardLogic[7][3] = 'Q';
    //Black Pieces (Lower-case)
    boardLogic[0][0] = 'r';
    boardLogic[0][1] = 'n';
    boardLogic[0][2] = 'b';
    boardLogic[0][3] = 'q';
    boardLogic[0][4] = 'k';
    boardLogic[0][5] = 'b';
    boardLogic[0][6] = 'n';
    boardLogic[0][7] = 'r';
}
bool isValidMove(int sx, int sy, int dx, int dy)
{


    if (!isInsideBoard(dx, dy))                                          //Destination Box Must be inside Board 8*8
    {
        return false;
    }

    char piece = boardLogic[sx][sy];                                     //Check if the source square has a piece or not(empty)
    if (piece == ' ')
    {
        return false;
    }


    if (whiteTurn && islower(piece))                                      //Check whether Valid Turn or Not
    {
        return false;
    }
    if (!whiteTurn && isupper(piece))
    {
        return false;
    }
    switch (tolower(piece))                                               //Check if the move is valid according to the piece type
    {
    case 'p':
        return isValidPawnMove(sx, sy, dx, dy, piece);
    case 'r':
        return isvalidRookmove(sx, sy, dx, dy, piece);
    case 'n':
        return isvalidKinghtmove(sx, sy, dx, dy, piece);
    case 'b':
        return isValidBishopMove(sx, sy, dx, dy, piece);
    case 'q':
        return isValidQueenMove(sx, sy, dx, dy, piece);
    case 'k':
        return isvalidKingmove(sx, sy, dx, dy, piece);
    }
    return false;
}
bool isValidPawnMove(int sx, int sy, int dx, int dy, char piece)
{
    int dir = (isupper(piece)) ? -1 : +1;   // White (-1), Black (+1)

    // 1 — Forward 1 step
    if (sy == dy && dx == sx + dir && boardLogic[dx][dy] == ' ')
        return true;

    // 2 — Forward 2 steps (first move only)
    if (sy == dy && dx == sx + 2 * dir &&
        boardLogic[sx + dir][sy] == ' ' &&
        boardLogic[dx][dy] == ' ' &&
        ((isupper(piece) && sx == 6) || (islower(piece) && sx == 1)))
        return true;

    // 3 — Normal Capture
    if (abs(dy - sy) == 1 && dx == sx + dir &&
        boardLogic[dx][dy] != ' ' &&
        isupper(piece) != isupper(boardLogic[dx][dy]))
        return true;

    // 4 — EN PASSANT
    if (abs(dy - sy) == 1 && dx == sx + dir &&
        boardLogic[dx][dy] == ' ' &&                // destination empty
        dx == enPassantRow && dy == enPassantCol)   // matches EP target
        return true;

    return false;
}
bool isvalidRookmove(int sx, int sy, int dx, int dy, char piece)
{
    if (dx != sx && dy != sy) // for same row because weather the row or column should be same
        return false;
    int stepX = 0, stepY = 0;
    if (dx > sx) // if moving down dx > sx (0 >> 7) stepX should be increase
    {
        stepX = +1;
    }
    else if (sx > dx) // if moving up sx > dx (7 >> 0) stepX should be decrease
    {
        stepX = -1;
    }
    else if (dy > sy) // if moving right then dy > sy (0 >> 7) stepY should be increase
    {
        stepY = +1;
    }
    else if (sy > dy) // if moving left then sy > dy (7 >> 0) stepY should be Decrease
    {
        stepY = -1;
    }
    int x = sx + stepX, y = sy + stepY;
    while (y != dy || x != dx) // check one coordinate before the destination
    {
        if (boardLogic[x][y] != ' ') // checking for whether the path is block or not 
        {
            return false;
        }
        x += stepX, y += stepY; // increment or decrement for next checking
    }
    if (boardLogic[dx][dy] == ' ') // if not space then return true for making move
    {
        return true;
    }
    if (isupper(piece) != isupper(boardLogic[dx][dy]))
    {
        return true;
    }
    return false;
}
bool isValidBishopMove(int sx, int sy, int dx, int dy, char piece)
{
    if (abs(dx - sx) != abs(dy - sy)) // change in row == change in column for bishop (abs function make - to +)
    {
        return false;
    }

    int stepX = (dx > sx) ? 1 : -1; //down Right x+=1, down left x+=1, Upward (Right or left) x-=1
    int stepY = (dy > sy) ? 1 : -1; //down Right y+=1, down left y-=1, Upward Right y+=1, Upward left y-=1

    int x = sx + stepX;
    int y = sy + stepY;


    while (x != dx && y != dy) // Checking for the move (not blocked, is-space )
    {
        if (boardLogic[x][y] != ' ')
        {
            return false;
        }
        x += stepX;
        y += stepY;
    }


    if (boardLogic[dx][dy] == ' ' || (isupper(piece) != isupper(boardLogic[dx][dy]))) //  if !same piece or not space then return true for making move
    {
        return true;
    }

    return false;
}
bool isvalidKinghtmove(int sx, int sy, int dx, int dy, char piece)
{
    int x = abs(dx - sx), y = abs(dy - sy);
    if (!((x == 2 && y == 1) || (x == 1 && y == 2)))                            //knight can move L shape within the Board
    {                                                                           // so row =2 and column =1 or vise versa
        return false;
    }
    if (boardLogic[dx][dy] == ' ' || (isupper(piece) != isupper(boardLogic[dx][dy])))
    {
        return true;
    }
    return false;

}
bool isValidQueenMove(int sx, int sy, int dx, int dy, char piece)
{
    //Queen can move Digonally or in a straight line
    if (abs(dx - sx) == abs(dy - sy))                                        // so I have a function of Rook and Bishop
    {                                                                        // for Rook atleast same row or column
        return isValidBishopMove(sx, sy, dx, dy, piece);                     // for bishop change in row == change in column
    }                                                                        // if destination is not as same as rook or bishop then not valid move

    else if (sx == dx || sy == dy)
    {
        return isvalidRookmove(sx, sy, dx, dy, piece);
    }
    else
    {
        return false;
    }
}
bool isvalidKingmove(int sx, int sy, int dx, int dy, char piece)
{
    // Can't capture own piece
    if (boardLogic[dx][dy] != ' ' && isupper(piece) == isupper(boardLogic[dx][dy]))
        return false;

    // Normal king move (one square)
    if (abs(dx - sx) <= 1 && abs(dy - sy) <= 1)
    {
        // IMPORTANT: Temporarily remove king from board
        // Otherwise the king blocks attacks from behind itself
        boardLogic[sx][sy] = ' ';

        // Check if destination is attacked by opponent
        bool attacked = isSquareAttacked(dx, dy, !whiteTurn);

        // Restore king
        boardLogic[sx][sy] = piece;

        return !attacked;  // Valid only if NOT attacked
    }

    // =======================
    //      CASTLING RULES
    // =======================
    if (sx == dx && abs(dy - sy) == 2)
    {
        bool white = isupper(piece);

        if (white)
        {
            if (whiteKingMoved) return false;

            // Kingside castling
            if (sy == 4 && dy == 6 &&
                !whiteRookRightMoved &&
                boardLogic[7][5] == ' ' &&
                boardLogic[7][6] == ' ' &&
                !isSquareAttacked(7, 4, false) &&
                !isSquareAttacked(7, 5, false) &&
                !isSquareAttacked(7, 6, false))
                return true;

            // Queenside castling
            if (sy == 4 && dy == 2 &&
                !whiteRookLeftMoved &&
                boardLogic[7][1] == ' ' &&
                boardLogic[7][2] == ' ' &&
                boardLogic[7][3] == ' ' &&
                !isSquareAttacked(7, 4, false) &&
                !isSquareAttacked(7, 3, false) &&
                !isSquareAttacked(7, 2, false))
                return true;
        }
        else  // Black castling
        {
            if (blackKingMoved) return false;

            // Kingside
            if (sy == 4 && dy == 6 &&
                !blackRookRightMoved &&
                boardLogic[0][5] == ' ' &&
                boardLogic[0][6] == ' ' &&
                !isSquareAttacked(0, 4, true) &&
                !isSquareAttacked(0, 5, true) &&
                !isSquareAttacked(0, 6, true))
                return true;

            // Queenside
            if (sy == 4 && dy == 2 &&
                !blackRookLeftMoved &&
                boardLogic[0][1] == ' ' &&
                boardLogic[0][2] == ' ' &&
                boardLogic[0][3] == ' ' &&
                !isSquareAttacked(0, 4, true) &&
                !isSquareAttacked(0, 3, true) &&
                !isSquareAttacked(0, 2, true))
                return true;
        }
    }

    return false;
}

void findKing(char king, int& kx, int& ky)    //This Function Search for King Position on the Board
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (boardLogic[r][c] == king)
            {
                kx = r;
                ky = c;
                return;
            }
        }
    }
}
bool kingexpose(int sx, int sy, int dx, int dy)
{
    char backupSrc = boardLogic[sx][sy];
    char backupDst = boardLogic[dx][dy];

    boardLogic[dx][dy] = backupSrc;
    boardLogic[sx][sy] = ' ';

    char kingChar = whiteTurn ? 'K' : 'k';

    int kr = -1, kc = -1;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (boardLogic[r][c] == kingChar)
            {
                kr = r;
                kc = c;
            }


    bool inCheck = isSquareAttacked(kr, kc, !whiteTurn);


    boardLogic[sx][sy] = backupSrc;
    boardLogic[dx][dy] = backupDst;

    return inCheck;
}
bool isSquareAttacked(int dx, int dy, bool byWhite)
{

    bool old = whiteTurn;
    whiteTurn = byWhite;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            char p = boardLogic[r][c];
            if (p == ' ')
                continue;

            if (byWhite && isupper(p) == false)
                continue;
            if (!byWhite && isupper(p) == true)
                continue;

            if (isValidMove(r, c, dx, dy))
            {
                whiteTurn = old;
                return true;
            }
        }
    }
    whiteTurn = old;
    return false;
}
bool isInCheck(bool whiteChecked)
{
    int kingX = -1, kingY = -1;

    if (whiteChecked)                // WhiteChecked Provides the detail which King is actually Attacked
        findKing('K', kingX, kingY); //Find the Location of White King if it is checked
    else
        findKing('k', kingX, kingY); //Find the Location of Black King if it is checked

    if (kingX == -1)
        return false; // safety

    return isSquareAttacked(kingX, kingY, !whiteChecked); // Check the King Square is Under Attack or Not
}
bool hasAnyLegalMove(bool turn)
{
    bool old = whiteTurn;
    whiteTurn = turn;

    for (int sx = 0; sx < 8; sx++)
    {
        for (int sy = 0; sy < 8; sy++)
        {
            char p = boardLogic[sx][sy];
            if (p == ' ') continue;

            if (turn && islower(p))
                continue;
            if (!turn && isupper(p))
                continue;

            for (int dx = 0; dx < 8; dx++)
            {
                for (int dy = 0; dy < 8; dy++)
                {
                    if (isValidMove(sx, sy, dx, dy))
                    {

                        char backup = boardLogic[dx][dy]; //Temporary move to Destination for Check still in check or Not
                        char self = boardLogic[sx][sy];

                        boardLogic[dx][dy] = self;
                        boardLogic[sx][sy] = ' ';

                        bool stillCheck = isInCheck(turn);

                        // undo
                        boardLogic[sx][sy] = self;
                        boardLogic[dx][dy] = backup;

                        if (!stillCheck)
                        {
                            whiteTurn = old;
                            return true;
                        }
                    }
                }
            }
        }
    }

    whiteTurn = old;
    return false;
}
bool isCheckmate(bool whiteChecked)
{
    if (isInCheck(whiteChecked) && !hasAnyLegalMove(whiteChecked))
        return true;
    return false;
}
bool isStalemate(bool whiteChecked)
{
    // Stalemate>> NOT in check, but dont have any legal moves.
    if (!isInCheck(whiteChecked) && !hasAnyLegalMove(whiteChecked))
        return true;
    return false;
}
bool kingExists(bool white)
{
    char kc;
    if (white)
    {
        kc = 'K';
    }
    else
    {
        kc = 'k';
    }
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (boardLogic[r][c] == kc)
            {
                return true;
            }
        }
    }
    return false;
}
void makeMove(int sx, int sy, int dx, int dy)
{
    char piece = boardLogic[sx][sy];

    // =========================
    // Track king & rook movement
    // =========================
    if (piece == 'K') whiteKingMoved = true;
    if (piece == 'k') blackKingMoved = true;

    if (piece == 'R') {
        if (sx == 7 && sy == 0) whiteRookLeftMoved = true;
        if (sx == 7 && sy == 7) whiteRookRightMoved = true;
    }
    if (piece == 'r') {
        if (sx == 0 && sy == 0) blackRookLeftMoved = true;
        if (sx == 0 && sy == 7) blackRookRightMoved = true;
    }

    // =========================
    // EN PASSANT CAPTURE
    // =========================
    if (tolower(piece) == 'p' && dx == enPassantRow && dy == enPassantCol)
    {
        if (isupper(piece)) { // White pawn captures black
            if (boardLogic[dx + 1][dy] != ' ') {
                blackCaptured[blackCapCount++] = boardLogic[dx + 1][dy];
                boardLogic[dx + 1][dy] = ' ';
                castlingSound.play();
            }
        }
        else { // Black pawn captures white
            if (boardLogic[dx - 1][dy] != ' ') {
                whiteCaptured[whiteCapCount++] = boardLogic[dx - 1][dy];
                boardLogic[dx - 1][dy] = ' ';
                castlingSound.play();
            }
        }
    }

    // =========================
    // NORMAL CAPTURE
    // =========================
    if (boardLogic[dx][dy] != ' ') {
        if (isupper(boardLogic[dx][dy])) whiteCaptured[whiteCapCount++] = boardLogic[dx][dy];
        else blackCaptured[blackCapCount++] = boardLogic[dx][dy];

        captureSound.play();
    }

    // =========================
    // Move piece
    // =========================
    boardLogic[dx][dy] = piece;
    boardLogic[sx][sy] = ' ';

    // =========================
    // DOUBLE PAWN MOVE (Enable En Passant)
    // =========================
    enPassantRow = -1;
    enPassantCol = -1;
    if (piece == 'P' && sx == 6 && dx == 4) { enPassantRow = 5; enPassantCol = sy; }
    else if (piece == 'p' && sx == 1 && dx == 3) { enPassantRow = 2; enPassantCol = sy; }

    // =========================
    // CASTLING EXECUTION
    // =========================
    if (piece == 'K') {
        if (sy == 4 && dy == 6) { // King side
            boardLogic[7][5] = 'R';
            boardLogic[7][7] = ' ';
            castlingSound.play();
        }
        if (sy == 4 && dy == 2) { // Queen side
            boardLogic[7][3] = 'R';
            boardLogic[7][0] = ' ';
            castlingSound.play();
        }
    }

    if (piece == 'k') {
        if (sy == 4 && dy == 6) { // King side
            boardLogic[0][5] = 'r';
            boardLogic[0][7] = ' ';
            castlingSound.play();
        }
        if (sy == 4 && dy == 2) { // Queen side
            boardLogic[0][3] = 'r';
            boardLogic[0][0] = ' ';
            castlingSound.play();
        }
    }

    // =========================
    // Done
    // =========================
    updateboard(); // Refresh display after move
}

// ===========================
//     SFML BOARD HANDLING
// ===========================

int boardArr[8][8];

int convert(char Piece) //+for Upper(white Piece) -for Lower(black Piece)
{
    //when we are going to perform move it is easy to campare integers rather than characters so....
    switch (Piece)
    {
        //1 for white Pawn & so on
    case 'P': return 1;
    case 'R': return 2;
    case 'N': return 3;
    case 'B': return 4;
    case 'Q': return 5;
    case 'K': return 6;
        //-1 for Black Pawn & so on
    case 'p': return -1;
    case 'r': return -2;
    case 'n': return -3;
    case 'b': return -4;
    case 'q': return -5;
    case 'k': return -6;
    }
    return 0; //for spaces it return 0
}

char backConvert(int id)
{
    //when board update after move it assign the charater to the pieces (texture requires character not digit as our image name is Character)
    switch (id)
    {
    case 1: return 'P';
    case 2: return 'R';
    case 3: return 'N';
    case 4: return 'B';
    case 5: return 'Q';
    case 6: return 'K';

    case -1: return 'p';
    case -2: return 'r';
    case -3: return 'n';
    case -4: return 'b';
    case -5: return 'q';
    case -6: return 'k';
    }
    return ' '; //for 0 it returns spaces
}

void updateboard()// It actually convert the board into Digital-One
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            boardArr[r][c] = convert(boardLogic[r][c]);
        }
    }
}

// ==========
// DRAWING
// ==========

float tileW, tileH, offX, offY;
void drawBoard(RenderWindow& win, RectangleShape& box, int dragR, int dragC, int hoverR, int hoverC, Color lightTile, Color darkTile)
{
    // Stop menu music if it's still playing
    if (backgroundSound.getStatus() == sf::Music::Playing)
        backgroundSound.stop();

    Color validMoveColor = Color(230, 230, 250, 190);   // Lavender Mist
    Color invalidMoveColor = Color(255, 120, 140, 180); // Warm Rose

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            box.setSize(Vector2f(tileW, tileH));

            // Highlight the square under mouse while dragging
            if (r == hoverR && c == hoverC && dragR != -1 && dragC != -1)
            {
                if (isValidMove(dragR, dragC, r, c))
                    box.setFillColor(validMoveColor);
                else
                    box.setFillColor(invalidMoveColor);
            }
            else if ((r + c) % 2 == 0)
            {
                box.setFillColor(lightTile);
            }
            else
            {
                box.setFillColor(darkTile);
            }

            box.setPosition(c * tileW + offX, r * tileH + offY);
            win.draw(box);
        }
    }
}
void drawPieces(RenderWindow& win,Texture W[], Texture B[],int skipR, int skipC)
{
    Sprite s;
    s.setScale(0.75f, 0.75f);

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            // Skip drawing the dragged piece
            if (r == skipR && c == skipC)
                continue;

            int id = boardArr[r][c];
            if (id == 0)
                continue;

            if (id > 0)
                s.setTexture(W[id - 1]);
            else
                s.setTexture(B[-id - 1]);

            s.setPosition(c * tileW + offX,
                r * tileH + offY);

            win.draw(s);
        }
    }
}
void drawCaptured(RenderWindow& win, Texture W[], Texture B[])
{
    Sprite s;
    s.setScale(0.5f, 0.5f);
    float xWhite = offX + 8 * tileW + 10; // Right of board
    float xBlack = offX - tileW;           // Left of board
    float yStart = offY;

    // Draw light background for captured pieces
    RectangleShape bgWhite(Vector2f(tileW, tileH * 8));
    bgWhite.setFillColor(Color(200, 200, 200)); // Light grey
    bgWhite.setPosition(xWhite, yStart);
    win.draw(bgWhite);

    RectangleShape bgBlack(Vector2f(tileW, tileH * 8));
    bgBlack.setFillColor(Color(200, 200, 200)); // Light grey
    bgBlack.setPosition(xBlack, yStart);
    win.draw(bgBlack);

    // White captured pieces (shown on right)
    for (int i = 0; i < whiteCapCount; i++)
    {
        char c = whiteCaptured[i];
        int idx = convert(c) - 1;
        if (idx >= 0 && idx < 6)
        {
            s.setTexture(W[idx]);
            s.setPosition(xWhite, yStart + i * (tileH / 2));
            win.draw(s);
        }
    }

    // Black captured pieces (shown on left)
    for (int i = 0; i < blackCapCount; i++)
    {
        char c = blackCaptured[i];
        int idx = -convert(c) - 1;
        if (idx >= 0 && idx < 6)
        {
            s.setTexture(B[idx]);
            s.setPosition(xBlack, yStart + i * (tileH / 2));
            win.draw(s);
        }
    }
}
char showPromotionMenu(RenderWindow& window, bool white, Texture W[], Texture B[])
{
    RectangleShape menuBG(Vector2f(tileW * 4, tileH));
    menuBG.setFillColor(Color(200, 200, 200));
    menuBG.setPosition((window.getSize().x - tileW * 4) / 2, (window.getSize().y - tileH) / 2);

    Sprite sprites[4];
    int startX = menuBG.getPosition().x;
    for (int i = 0; i < 4; i++)
    {
        sprites[i].setScale(0.75f, 0.75f);
        sprites[i].setPosition(startX + i * tileW, menuBG.getPosition().y);
        if (white)
        {
            if (i == 0) sprites[i].setTexture(W[1]); // Rook
            if (i == 1) sprites[i].setTexture(W[2]); // Knight
            if (i == 2) sprites[i].setTexture(W[3]); // Bishop
            if (i == 3) sprites[i].setTexture(W[4]); // Queen
        }
        else
        {
            if (i == 0) sprites[i].setTexture(B[1]);
            if (i == 1) sprites[i].setTexture(B[2]);
            if (i == 2) sprites[i].setTexture(B[3]);
            if (i == 3) sprites[i].setTexture(B[4]);
        }
    }

    while (true)
    {
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
            }
            if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left)
            {
                int mx = ev.mouseButton.x;
                int idx = (mx - startX) / tileW;
                if (idx >= 0 && idx < 4)
                {
                    if (white) return (idx == 0 ? 'R' : idx == 1 ? 'N' : idx == 2 ? 'B' : 'Q');
                    else return (idx == 0 ? 'r' : idx == 1 ? 'n' : idx == 2 ? 'b' : 'q');
                }
            }
        }

        window.clear();
        RectangleShape box(Vector2f(tileW, tileH));
        drawBoard(window, box, -1, -1, -1, -1, lightColor, darkColor);



        drawPieces(window, W, B, -1, -1);
        window.draw(menuBG);
        for (int i = 0; i < 4; i++) window.draw(sprites[i]);
        window.display();
    }
}


// =======================
// DRAG & DROP VARIABLES
// =======================

bool dragging = false;
Sprite dragSprite;
int dragR, dragC, highlightRow, highlightCol;
float dragOffsetX, dragOffsetY;



// ============================
// 
// CHESS AI 
// 
// ===========================

enum AILevel {
    EASY,
    MEDIUM,
    HARD
};

AILevel aiDifficulty = MEDIUM;

// ===================== PIECE-SQUARE TABLES =====================

const int PAWN_TABLE[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    { 50, 50, 50, 50, 50, 50, 50, 50 },
    { 10, 10, 20, 30, 30, 20, 10, 10 },
    {  5,  5, 10, 25, 25, 10,  5,  5 },
    {  0,  0,  0, 20, 20,  0,  0,  0 },
    {  5, -5,-10,  0,  0,-10, -5,  5 },
    {  5, 10, 10,-20,-20, 10, 10,  5 },
    {  0,  0,  0,  0,  0,  0,  0,  0 }
};
const int KNIGHT_TABLE[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50 },
    {-40,-20,  0,  0,  0,  0,-20,-40 },
    {-30,  0, 10, 15, 15, 10,  0,-30 },
    {-30,  5, 15, 20, 20, 15,  5,-30 },
    {-30,  0, 15, 20, 20, 15,  0,-30 },
    {-30,  5, 10, 15, 15, 10,  5,-30 },
    {-40,-20,  0,  5,  5,  0,-20,-40 },
    {-50,-40,-30,-30,-30,-30,-40,-50 }
};
const int BISHOP_TABLE[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20 },
    {-10,  0,  0,  0,  0,  0,  0,-10 },
    {-10,  0,  5, 10, 10,  5,  0,-10 },
    {-10,  5,  5, 10, 10,  5,  5,-10 },
    {-10,  0, 10, 10, 10, 10,  0,-10 },
    {-10, 10, 10, 10, 10, 10, 10,-10 },
    {-10,  5,  0,  0,  0,  0,  5,-10 },
    {-20,-10,-10,-10,-10,-10,-10,-20 }
};
const int ROOK_TABLE[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    {  5, 10, 10, 10, 10, 10, 10,  5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    {  0,  0,  0,  5,  5,  0,  0,  0 }
};
const int QUEEN_TABLE[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20 },
    {-10,  0,  0,  0,  0,  0,  0,-10 },
    {-10,  0,  5,  5,  5,  5,  0,-10 },
    { -5,  0,  5,  5,  5,  5,  0, -5 },
    {  0,  0,  5,  5,  5,  5,  0, -5 },
    {-10,  5,  5,  5,  5,  5,  0,-10 },
    {-10,  0,  5,  0,  0,  0,  0,-10 },
    {-20,-10,-10, -5, -5,-10,-10,-20 }
};
const int KING_MIDDLE_TABLE[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30 },
    {-30,-40,-40,-50,-50,-40,-40,-30 },
    {-30,-40,-40,-50,-50,-40,-40,-30 },
    {-30,-40,-40,-50,-50,-40,-40,-30 },
    {-20,-30,-30,-40,-40,-30,-30,-20 },
    {-10,-20,-20,-20,-20,-20,-20,-10 },
    { 20, 20,  0,  0,  0,  0, 20, 20 },
    { 20, 30, 10,  0,  0, 10, 30, 20 }
};
const int KING_END_TABLE[8][8] = {
    {-50,-40,-30,-20,-20,-30,-40,-50 },
    {-30,-20,-10,  0,  0,-10,-20,-30 },
    {-30,-10, 20, 30, 30, 20,-10,-30 },
    {-30,-10, 30, 40, 40, 30,-10,-30 },
    {-30,-10, 30, 40, 40, 30,-10,-30 },
    {-30,-10, 20, 30, 30, 20,-10,-30 },
    {-30,-30,  0,  0,  0,  0,-30,-30 },
    {-50,-30,-30,-30,-30,-30,-30,-50 }
};
// ===================== UTILITY FUNCTIONS =====================

void copyBoard(char src[8][8], char dst[8][8]) {
    memcpy(dst, src, sizeof(char) * 64);
}   
int pieceValue(char p) {
    switch (tolower(p)) {
    case 'p': return 100;
    case 'n': return 320;
    case 'b': return 330;
    case 'r': return 500;
    case 'q': return 900;
    case 'k': return 20000;
    default: return 0;
    }
}
bool isEndgamePhase() {
    int queens = 0, rooks = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char p = tolower(boardLogic[r][c]);
            if (p == 'q') queens++;
            else if (p == 'r') rooks++;
        }
    }
    return queens == 0 || (queens <= 1 && rooks == 0);
}
bool isOpenFile(int col) {
    for (int r = 0; r < 8; r++) {
        if (tolower(boardLogic[r][col]) == 'p') return false;
    }
    return true;
}

// ===================== CRITICAL: ATTACK/DEFENSE DETECTION =====================

// Count attackers with FULL exchange simulation
int countAttackers(int r, int c, bool byWhite) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char p = boardLogic[i][j];
            if (p == ' ') continue;
            if ((isupper(p) != 0) != byWhite) continue;

            // Temporarily move attacker and check if move is valid
            char backup = boardLogic[r][c];
            char attacker = boardLogic[i][j];

            if (isValidMove(i, j, r, c)) {
                // Simulate capture to check if it's a valid attack
                boardLogic[r][c] = attacker;
                boardLogic[i][j] = ' ';

                // Check if this puts own king in check
                bool selfCheck = isInCheck(byWhite);

                // Restore
                boardLogic[i][j] = attacker;
                boardLogic[r][c] = backup;

                if (!selfCheck) count++;
            }
        }
    }
    return count;
}

// CRITICAL: Full Static Exchange Evaluation
int fullStaticExchange(int sx, int sy, int dx, int dy) {
    char attacker = boardLogic[sx][sy];
    char victim = boardLogic[dx][dy];

    if (victim == ' ') return 0;
    if (attacker == ' ') return 0;

    bool isWhite = (isupper(attacker) != 0);

    // Material gained from capture
    int gain = pieceValue(victim);

    // Simulate the capture
    char backup[8][8];
    copyBoard(boardLogic, backup);
    boardLogic[dx][dy] = attacker;
    boardLogic[sx][sy] = ' ';

    // How many can recapture?
    int enemyAttackers = countAttackers(dx, dy, !isWhite);
    int ourDefenders = countAttackers(dx, dy, isWhite);

    // Restore board
    copyBoard(backup, boardLogic);

    // If enemy can recapture
    if (enemyAttackers > 0) {
        gain -= pieceValue(attacker);

        // If we have defenders, it might continue
        if (ourDefenders > 0 && gain < 0) {
            // Don't take if we lose material
            return gain;
        }
    }

    return gain;
}

// CRITICAL: Is this move truly safe?
bool isMoveTrulySafe(int sx, int sy, int dx, int dy) {
    char mover = boardLogic[sx][sy];
    char victim = boardLogic[dx][dy];

    if (mover == ' ') return false;

    bool isWhite = (isupper(mover) != 0);
    int moverValue = pieceValue(mover);
    int victimValue = victim == ' ' ? 0 : pieceValue(victim);

    // Simulate move
    boardLogic[dx][dy] = mover;
    boardLogic[sx][sy] = ' ';

    // Check if we put ourselves in check
    bool inCheck = isInCheck(isWhite);

    // Count attacks on destination
    int attackers = 0;
    int defenders = 0;

    if (!inCheck) {
        attackers = countAttackers(dx, dy, !isWhite);
        defenders = countAttackers(dx, dy, isWhite);
    }

    // Undo move
    boardLogic[sx][sy] = mover;
    boardLogic[dx][dy] = victim;

    // If move puts us in check, it's illegal (should be caught earlier)
    if (inCheck) return false;

    // CRITICAL CHECKS:

    // 1. If it's a capture
    if (victim != ' ') {
        // Good: Taking equal or better piece
        if (victimValue >= moverValue) {
            // But make sure we won't be recaptured for more loss
            if (attackers > 0) {
                int see = fullStaticExchange(sx, sy, dx, dy);
                if (see < 0) return false; // Bad trade
            }
            return true;
        }

        // Bad: Taking worse piece (Queen takes Pawn)
        if (attackers > 0) {
            return false; // Will be recaptured, bad trade
        }
    }

    // 2. If it's a quiet move (no capture)
    else {
        // Make sure piece won't hang
        if (attackers > defenders) {
            return false; // Piece hangs!
        }

        // Even if defended, don't move valuable piece to attacked square
        if (attackers > 0 && moverValue > 300) {
            // Only move if very well defended
            if (defenders < attackers) return false;
        }
    }

    return true;
}

// ===================== THREAT EVALUATION =====================

int evaluateThreats(bool ourColor) {
    int threatScore = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char p = boardLogic[r][c];
            if (p == ' ') continue;
            if ((isupper(p) != 0) != ourColor) continue;

            int attackers = countAttackers(r, c, !ourColor);
            int defenders = countAttackers(r, c, ourColor);

            if (attackers > defenders) {
                // Hanging piece - MASSIVE penalty
                threatScore -= pieceValue(p) * 2;
            }
            else if (attackers > 0) {
                // Attacked but defended - minor penalty
                threatScore -= pieceValue(p) / 6;
            }
        }
    }

    return threatScore;
}

// Detect fork opportunities
int detectForks(bool ourColor) {
    int forkBonus = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char p = boardLogic[r][c];
            if (p == ' ' || (isupper(p) != 0) != ourColor) continue;

            std::vector<int> targetValues;

            for (int dr = 0; dr < 8; dr++) {
                for (int dc = 0; dc < 8; dc++) {
                    char target = boardLogic[dr][dc];
                    if (target == ' ' || (isupper(target) != 0) == ourColor) continue;

                    if (isValidMove(r, c, dr, dc)) {
                        targetValues.push_back(pieceValue(target));
                    }
                }
            }

            if (targetValues.size() >= 2) {
                int sum = 0;
                for (int val : targetValues) sum += val;
                forkBonus += sum / 4;
            }
        }
    }

    return forkBonus;
}

// ===================== EVALUATION =====================

int evaluateBoard(bool aiIsWhite) {
    int score = 0;
    bool endgame = isEndgamePhase();

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char p = boardLogic[r][c];
            if (p == ' ') continue;

            bool isWhite = (isupper(p) != 0);
            int val = pieceValue(p);
            int positional = 0;
            char type = tolower(p);

            int tableRow = isWhite ? r : (7 - r);

            switch (type) {
            case 'p': positional = PAWN_TABLE[tableRow][c]; break;
            case 'n': positional = KNIGHT_TABLE[tableRow][c]; break;
            case 'b': positional = BISHOP_TABLE[tableRow][c]; break;
            case 'r':
                positional = ROOK_TABLE[tableRow][c];
                if (isOpenFile(c)) positional += 50;
                break;
            case 'q': positional = QUEEN_TABLE[tableRow][c]; break;
            case 'k': positional = endgame ? KING_END_TABLE[tableRow][c] : KING_MIDDLE_TABLE[tableRow][c]; break;
            }

            int total = val + positional;
            score += isWhite ? total : -total;
        }
    }

    // CRITICAL: Heavy weight on threats
    score += evaluateThreats(true) * 3;
    score -= evaluateThreats(false) * 3;

    score += detectForks(true);
    score -= detectForks(false);

    return aiIsWhite ? score : -score;
}

// ===================== MOVE GENERATION =====================

void simulateMove(Move m, char board[8][8]) {
    char p = board[m.sx][m.sy];
    board[m.dx][m.dy] = p;
    board[m.sx][m.sy] = ' ';

    if ((p == 'P' && m.dx == 0) || (p == 'p' && m.dx == 7)) {
        board[m.dx][m.dy] = (isupper(p) != 0) ? 'Q' : 'q';
    }
}

std::vector<Move> generateAllMoves(bool whiteTurn) {
    std::vector<Move> moves;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char p = boardLogic[r][c];
            if (p == ' ' || (isupper(p) != 0) != whiteTurn) continue;

            for (int dr = 0; dr < 8; dr++) {
                for (int dc = 0; dc < 8; dc++) {
                    if (!isValidMove(r, c, dr, dc)) continue;

                    char captured = boardLogic[dr][dc];
                    char moved = boardLogic[r][c];

                    boardLogic[dr][dc] = moved;
                    boardLogic[r][c] = ' ';

                    bool kingInCheck = isInCheck(whiteTurn);

                    boardLogic[r][c] = moved;
                    boardLogic[dr][dc] = captured;

                    if (!kingInCheck) {
                        moves.push_back({ r, c, dr, dc });
                    }
                }
            }
        }
    }

    return moves;
}

// ===================== MOVE ORDERING =====================

int scoreMoveForOrdering(const Move& m, bool isWhite) {
    int score = 0;
    char attacker = boardLogic[m.sx][m.sy];
    char victim = boardLogic[m.dx][m.dy];

    // CRITICAL: Reject unsafe moves
    if (!isMoveTrulySafe(m.sx, m.sy, m.dx, m.dy)) {
        score -= 500000; // NEVER pick unsafe moves
    }

    // Captures
    if (victim != ' ') {
        int see = fullStaticExchange(m.sx, m.sy, m.dx, m.dy);
        if (see > 0) {
            score += 50000 + see * 10; // Great capture
        }
        else if (see == 0) {
            score += 10000; // Equal trade
        }
        else {
            score -= 10000; // Bad trade
        }
    }

    // Promotions
    if (tolower(attacker) == 'p' && (m.dx == 0 || m.dx == 7)) {
        score += 90000;
    }

    // Checks
    char backup[8][8];
    copyBoard(boardLogic, backup);
    simulateMove(m, boardLogic);
    if (isInCheck(!isWhite)) {
        score += 1000;
    }
    copyBoard(backup, boardLogic);

    // Center control
    if (m.dx >= 3 && m.dx <= 4 && m.dy >= 3 && m.dy <= 4) {
        score += 50;
    }

    return score;
}

// ===================== QUIESCENCE =====================

int quiescence(int alpha, int beta, bool maximizing, bool aiIsWhite, int depth = 0) {
    if (depth > 3) return evaluateBoard(aiIsWhite);

    int standPat = evaluateBoard(aiIsWhite);

    if (maximizing) {
        if (standPat >= beta) return beta;
        if (standPat > alpha) alpha = standPat;
    }
    else {
        if (standPat <= alpha) return alpha;
        if (standPat < beta) beta = standPat;
    }

    std::vector<Move> allMoves = generateAllMoves(maximizing ? aiIsWhite : !aiIsWhite);
    std::vector<Move> captures;

    for (auto& m : allMoves) {
        if (boardLogic[m.dx][m.dy] != ' ') {
            if (fullStaticExchange(m.sx, m.sy, m.dx, m.dy) >= 0) {
                captures.push_back(m);
            }
        }
    }

    std::sort(captures.begin(), captures.end(), [&](const Move& a, const Move& b) {
        return scoreMoveForOrdering(a, maximizing ? aiIsWhite : !aiIsWhite) >
            scoreMoveForOrdering(b, maximizing ? aiIsWhite : !aiIsWhite);
        });

    for (auto& m : captures) {
        char backup[8][8];
        copyBoard(boardLogic, backup);
        simulateMove(m, boardLogic);

        int score = quiescence(alpha, beta, !maximizing, aiIsWhite, depth + 1);

        copyBoard(backup, boardLogic);

        if (maximizing) {
            if (score >= beta) return beta;
            if (score > alpha) alpha = score;
        }
        else {
            if (score <= alpha) return alpha;
            if (score < beta) beta = score;
        }
    }

    return maximizing ? alpha : beta;
}

// ===================== MINIMAX =====================

const int MATE_SCORE = 30000;

int minimax(int depth, bool maximizing, int alpha, int beta, bool aiIsWhite) {
    bool currentTurn = maximizing ? aiIsWhite : !aiIsWhite;

    if (isCheckmate(currentTurn)) {
        return maximizing ? (-MATE_SCORE + depth) : (MATE_SCORE - depth);
    }

    if (isStalemate(currentTurn)) return 0;
    if (depth == 0) return quiescence(alpha, beta, maximizing, aiIsWhite);

    std::vector<Move> moves = generateAllMoves(currentTurn);
    if (moves.empty()) return 0;

    std::sort(moves.begin(), moves.end(), [&](const Move& a, const Move& b) {
        return scoreMoveForOrdering(a, currentTurn) > scoreMoveForOrdering(b, currentTurn);
        });

    if (maximizing) {
        int maxEval = -999999;
        for (auto& m : moves) {
            char backup[8][8];
            copyBoard(boardLogic, backup);
            simulateMove(m, boardLogic);

            int eval = minimax(depth - 1, false, alpha, beta, aiIsWhite);

            copyBoard(backup, boardLogic);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    }
    else {
        int minEval = 999999;
        for (auto& m : moves) {
            char backup[8][8];
            copyBoard(boardLogic, backup);
            simulateMove(m, boardLogic);

            int eval = minimax(depth - 1, true, alpha, beta, aiIsWhite);

            copyBoard(backup, boardLogic);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

// ===================== FIND BEST MOVE =====================

Move findBestAIMove(AILevel level, bool aiIsWhite) {
    std::vector<Move> moves = generateAllMoves(aiIsWhite);
    if (moves.empty()) return { -1, -1, -1, -1 };

    int maxDepth = 3;

    switch (level) {
    case EASY:   maxDepth = 2; break;
    case MEDIUM: maxDepth = 3; break;
    case HARD:   maxDepth = 4; break;
    }

    Move bestMove = moves[0];
    int bestScore = -999999;

    // Filter out unsafe moves immediately
    std::vector<Move> safeMoves;
    for (auto& m : moves) {
        if (isMoveTrulySafe(m.sx, m.sy, m.dx, m.dy)) {
            safeMoves.push_back(m);
        }
    }

    // If all moves are "unsafe", use all moves (stalemate prevention)
    if (safeMoves.empty()) safeMoves = moves;

    std::sort(safeMoves.begin(), safeMoves.end(), [&](const Move& a, const Move& b) {
        return scoreMoveForOrdering(a, aiIsWhite) > scoreMoveForOrdering(b, aiIsWhite);
        });

    for (auto& m : safeMoves) {
        char backup[8][8];
        copyBoard(boardLogic, backup);
        simulateMove(m, boardLogic);

        int score = minimax(maxDepth - 1, false, -999999, 999999, aiIsWhite);

        copyBoard(backup, boardLogic);

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }

        if (score >= MATE_SCORE - 20) break;
    }

    return bestMove;
}

// ===================== APPLY AI MOVE =====================

void applyAIMove(bool aiIsWhite) {
    Move aiMove = findBestAIMove(aiDifficulty, aiIsWhite);

    if (aiMove.sx == -1) {
        cout << "AI has no moves\n";
        return;
    }

    recordStateBeforeMove();
    makeMove(aiMove.sx, aiMove.sy, aiMove.dx, aiMove.dy);

    char movedPiece = boardLogic[aiMove.dx][aiMove.dy];
    if ((movedPiece == 'P' && aiMove.dx == 0) || (movedPiece == 'p' && aiMove.dx == 7)) {
        boardLogic[aiMove.dx][aiMove.dy] = (isupper(movedPiece) != 0) ? 'Q' : 'q';
    }

    updateboard();
}

// =======================
// MAIN MENU
// =======================
int showMainMenu(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile("Font/bebasneue.ttf")) {
        cout << "Font load failed!" << endl;
        return 0;
    }
    Texture bgTexture;
    if (!bgTexture.loadFromFile("pieces/bg.png")) {
        cout << "Background image not found!" << endl;
    }
    Sprite bgSprite(bgTexture);
    bgSprite.setScale(
        (float)window.getSize().x / bgTexture.getSize().x,
        (float)window.getSize().y / bgTexture.getSize().y
    );

    // Title
    Text title("CHESS MASTER", font, 90);
    title.setFillColor(Color(255, 215, 0));
    title.setStyle(Text::Bold);
    FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        window.getSize().x / 2 - titleBounds.width / 2,
        80
    );

    // Subtitle
    Text subtitle("Select Game Mode", font, 35);
    subtitle.setFillColor(Color::White);
    FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition(
        window.getSize().x / 2 - subtitleBounds.width / 2,
        190
    );

    // Menu buttons
    const int BUTTON_COUNT = 4;
    RectangleShape buttons[BUTTON_COUNT];
    Text buttonTexts[BUTTON_COUNT];
    string labels[] = {
        "Human vs Human",
        "Human vs AI (White)",
        "Human vs AI (Black)",
        "Puzzle Mode"
    };

    Color buttonColors[] = {
        Color(50, 150, 50),
        Color(50, 100, 200),
        Color(200, 50, 50),
        Color(150, 100, 200)
    };

    float buttonStartY = 280;
    float buttonSpacing = 100;
    float buttonWidth = 450;
    float buttonHeight = 75;

    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].setSize(Vector2f(buttonWidth, buttonHeight));
        buttons[i].setPosition(
            window.getSize().x / 2 - buttonWidth / 2,
            buttonStartY + i * buttonSpacing
        );
        buttons[i].setFillColor(Color(buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 200));
        buttons[i].setOutlineColor(Color::White);
        buttons[i].setOutlineThickness(3);

        buttonTexts[i].setString(labels[i]);
        buttonTexts[i].setFont(font);
        buttonTexts[i].setCharacterSize(32);
        buttonTexts[i].setFillColor(Color::White);
        buttonTexts[i].setStyle(Text::Bold);

        FloatRect bounds = buttonTexts[i].getLocalBounds();
        buttonTexts[i].setPosition(
            buttons[i].getPosition().x + (buttonWidth - bounds.width) / 2,
            buttons[i].getPosition().y + (buttonHeight - bounds.height) / 2 - 8
        );
    }

    // Main menu loop
    while (window.isOpen()) {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) {
                window.close();
                return -1;  // FIXED: Return -1 to indicate window was closed
            }

            if (ev.type == Event::MouseButtonPressed &&
                ev.mouseButton.button == Mouse::Left) {
                Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);

                for (int i = 0; i < BUTTON_COUNT; i++) {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) {
                        return i + 1;
                    }
                }
            }
        }

        // Hover effects
        Vector2i mousePos = Mouse::getPosition(window);
        for (int i = 0; i < BUTTON_COUNT; i++) {
            if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                buttons[i].setFillColor(Color(
                    buttonColors[i].r + 50,
                    buttonColors[i].g + 50,
                    buttonColors[i].b + 50,
                    230
                ));
                buttons[i].setOutlineColor(Color::Yellow);
            }
            else {
                buttons[i].setFillColor(Color(
                    buttonColors[i].r,
                    buttonColors[i].g,
                    buttonColors[i].b,
                    200
                ));
                buttons[i].setOutlineColor(Color::White);
            }
        }

        // Render
        window.clear(Color::Black);
        window.draw(bgSprite);
        window.draw(title);
        window.draw(subtitle);

        for (int i = 0; i < BUTTON_COUNT; i++) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }

        window.display();
    }

    return -1;  // FIXED: Return -1 if loop exits
}


// =======================
// AI DIFFICULTY MENU
// =======================
int showAIDifficultyMenu(RenderWindow& window)
{
    backgroundSound.play();

    Font font;
    if (!font.loadFromFile("Font/bebasneue.ttf")) {
        cout << "Font load failed!" << endl;
        return 0;
    }

    Texture bgTexture;
    if (!bgTexture.loadFromFile("pieces/bg1.png")) {
        cout << "Background image not found!" << endl;
    }
    Sprite bgSprite(bgTexture);
    bgSprite.setScale(
        (float)window.getSize().x / bgTexture.getSize().x,
        (float)window.getSize().y / bgTexture.getSize().y
    );

    Text title("SELECT AI DIFFICULTY", font, 90);
    title.setFillColor(Color(255, 215, 0));
    title.setStyle(Text::Bold);
    FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        window.getSize().x / 2 - titleBounds.width / 2,
        80
    );

    Text subtitle("Choose Your Challenge Level", font, 35);
    subtitle.setFillColor(Color::White);
    FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition(
        window.getSize().x / 2 - subtitleBounds.width / 2,
        190
    );

    RectangleShape backButton(Vector2f(120.f, 50.f));
    backButton.setPosition(30.f, 30.f);
    backButton.setFillColor(Color(100, 100, 100, 200));
    backButton.setOutlineColor(Color::White);
    backButton.setOutlineThickness(3);

    Text backText("< BACK", font, 28);
    backText.setFillColor(Color::White);
    backText.setStyle(Text::Bold);
    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        backButton.getPosition().x + (120.f - backBounds.width) / 2,
        backButton.getPosition().y + (50.f - backBounds.height) / 2 - 5
    );

    const int BUTTON_COUNT = 3;
    RectangleShape buttons[BUTTON_COUNT];
    Text buttonTexts[BUTTON_COUNT];
    string labels[] = {
        "EASY",
        "MEDIUM",
        "HARD"
    };

    Color buttonColors[] = {
        Color(50, 150, 50),
        Color(200, 150, 50),
        Color(200, 50, 50)
    };

    float buttonStartY = 280;
    float buttonSpacing = 100;
    float buttonWidth = 450;
    float buttonHeight = 75;

    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].setSize(Vector2f(buttonWidth, buttonHeight));
        buttons[i].setPosition(
            window.getSize().x / 2 - buttonWidth / 2,
            buttonStartY + i * buttonSpacing
        );
        buttons[i].setFillColor(Color(buttonColors[i].r, buttonColors[i].g, buttonColors[i].b, 200));
        buttons[i].setOutlineColor(Color::White);
        buttons[i].setOutlineThickness(3);

        buttonTexts[i].setString(labels[i]);
        buttonTexts[i].setFont(font);
        buttonTexts[i].setCharacterSize(32);
        buttonTexts[i].setFillColor(Color::White);
        buttonTexts[i].setStyle(Text::Bold);

        FloatRect bounds = buttonTexts[i].getLocalBounds();
        buttonTexts[i].setPosition(
            buttons[i].getPosition().x + (buttonWidth - bounds.width) / 2,
            buttons[i].getPosition().y + (buttonHeight - bounds.height) / 2 - 8
        );
    }

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile("audio/move.wav"))
        cout << "Click sound not found!\n";
    Sound clickSound;
    clickSound.setBuffer(clickBuffer);

    while (window.isOpen()) {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) {
                window.close();
                backgroundSound.stop();
                return -1;  // FIXED: Return -1 to indicate window was closed
            }

            if (ev.type == Event::MouseButtonPressed &&
                ev.mouseButton.button == Mouse::Left) {
                Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);

                if (backButton.getGlobalBounds().contains(mousePos)) {
                    clickSound.play();
                    backgroundSound.stop();
                    return 0;
                }

                for (int i = 0; i < BUTTON_COUNT; i++) {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        backgroundSound.stop();
                        return i + 1;
                    }
                }
            }
        }

        Vector2i mousePos = Mouse::getPosition(window);

        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            backButton.setFillColor(Color(150, 150, 150, 230));
            backButton.setOutlineColor(Color::Yellow);
        }
        else {
            backButton.setFillColor(Color(100, 100, 100, 200));
            backButton.setOutlineColor(Color::White);
        }

        for (int i = 0; i < BUTTON_COUNT; i++) {
            if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                buttons[i].setFillColor(Color(
                    min(buttonColors[i].r + 50, 255),
                    min(buttonColors[i].g + 50, 255),
                    min(buttonColors[i].b + 50, 255),
                    230
                ));
                buttons[i].setOutlineColor(Color::Yellow);
            }
            else {
                buttons[i].setFillColor(Color(
                    buttonColors[i].r,
                    buttonColors[i].g,
                    buttonColors[i].b,
                    200
                ));
                buttons[i].setOutlineColor(Color::White);
            }
        }

        window.clear(Color::Black);
        window.draw(bgSprite);
        window.draw(title);
        window.draw(subtitle);
        window.draw(backButton);
        window.draw(backText);

        for (int i = 0; i < BUTTON_COUNT; i++) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }

        window.display();
    }

    backgroundSound.stop();
    return -1;  // FIXED: Return -1 if loop exits
}
int showEndGameMenu(RenderWindow& window, std::string message) {
    backgroundSound.play();
    Font font;
    if (!font.loadFromFile("Font/bebasneue.ttf")) {
        std::cout << "Font not found!\n";
        return -1;
    }

    // --- Setup message text ---
    Text msgText;
    msgText.setFont(font);
    msgText.setString(message);
    msgText.setCharacterSize(50);
    msgText.setFillColor(Color::White);
    msgText.setPosition(window.getSize().x / 2.f - 200, window.getSize().y / 2.f - 150);

    // --- Buttons ---
    RectangleShape btnMain(Vector2f(250, 60));
    RectangleShape btnExit(Vector2f(250, 60));

    btnMain.setFillColor(Color(100, 100, 200));
    btnExit.setFillColor(Color(200, 100, 100));

    btnMain.setPosition(window.getSize().x / 2.f - 125, window.getSize().y / 2.f);
    btnExit.setPosition(window.getSize().x / 2.f - 125, window.getSize().y / 2.f + 100);

    Text txtMain, txtExit;
    txtMain.setFont(font);
    txtExit.setFont(font);
    txtMain.setString("Back to Menu");
    txtExit.setString("Exit Game");
    txtMain.setCharacterSize(30);
    txtExit.setCharacterSize(30);
    txtMain.setFillColor(Color::White);
    txtExit.setFillColor(Color::White);
    txtMain.setPosition(btnMain.getPosition().x + 20, btnMain.getPosition().y + 10);
    txtExit.setPosition(btnExit.getPosition().x + 50, btnExit.getPosition().y + 10);

    // --- Menu Loop ---
    while (true) {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) return 1;

            if (ev.type == Event::MouseButtonPressed &&
                ev.mouseButton.button == Mouse::Left) {
                Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);
                if (btnMain.getGlobalBounds().contains(mousePos))
                    return 0; // Back to main menu
                if (btnExit.getGlobalBounds().contains(mousePos))
                    return 1; // Exit
            }
        }

        window.clear(Color::Black);
        window.draw(msgText);
        window.draw(btnMain);
        window.draw(btnExit);
        window.draw(txtMain);
        window.draw(txtExit);
        backgroundSound.stop();
        window.display();
    }
}
int showBoardThemeMenu(RenderWindow& window) {
    backgroundSound.play();

    Font font;
    if (!font.loadFromFile("Font/bebasneue.ttf")) {
        cout << "Font load failed!" << endl;
        return 0;
    }

    Texture menuBGTex;
    if (!menuBGTex.loadFromFile("pieces/cat.png")) {
        cout << "Failed to load menu background!" << endl;
    }
    Sprite menuBG(menuBGTex);
    menuBG.setScale(
        (float)window.getSize().x / menuBGTex.getSize().x,
        (float)window.getSize().y / menuBGTex.getSize().y
    );

    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 120));

    Text title("SELECT BOARD THEME", font, 90);
    title.setFillColor(Color(255, 215, 0));
    title.setStyle(Text::Bold);
    FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        window.getSize().x / 2 - titleBounds.width / 2,
        60
    );

    Text subtitle("Choose Your Battle Arena", font, 35);
    subtitle.setFillColor(Color::White);
    FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition(
        window.getSize().x / 2 - subtitleBounds.width / 2,
        170
    );

    RectangleShape backButton(Vector2f(120.f, 50.f));
    backButton.setPosition(30.f, 30.f);
    backButton.setFillColor(Color(100, 100, 100, 200));
    backButton.setOutlineColor(Color::White);
    backButton.setOutlineThickness(3);

    Text backText("< BACK", font, 28);
    backText.setFillColor(Color::White);
    backText.setStyle(Text::Bold);
    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        backButton.getPosition().x + (120.f - backBounds.width) / 2,
        backButton.getPosition().y + (50.f - backBounds.height) / 2 - 5
    );

    struct BoardTheme {
        Color lightTile;
        Color darkTile;
        string name;
    };

    BoardTheme themes[4] = {
        { Color(240, 217, 181), Color(181, 136, 99), "Classic Oak" },
        { Color(200, 255, 200), Color(50, 150, 50),  "Emerald Forest" },
        { Color(180, 210, 255), Color(50, 90, 200),  "Ocean Depths" },
        { Color(255, 200, 200), Color(200, 50, 50),  "Crimson Fire" }
    };

    float previewSize = 180.f;
    float totalWidth = 4 * previewSize + 3 * 40.f;
    float startX = (window.getSize().x - totalWidth) / 2.f;
    float startY = 280.f;

    RectangleShape boardPreviews[4];
    Text themeNames[4];

    for (int i = 0; i < 4; i++) {
        boardPreviews[i].setSize(Vector2f(previewSize, previewSize));
        boardPreviews[i].setPosition(startX + i * (previewSize + 40.f), startY);
        boardPreviews[i].setFillColor(Color::Transparent);
        boardPreviews[i].setOutlineThickness(4.f);
        boardPreviews[i].setOutlineColor(Color::White);

        themeNames[i].setString(themes[i].name);
        themeNames[i].setFont(font);
        themeNames[i].setCharacterSize(26);
        themeNames[i].setFillColor(Color::White);
        themeNames[i].setStyle(Text::Bold);

        FloatRect nameBounds = themeNames[i].getLocalBounds();
        themeNames[i].setPosition(
            boardPreviews[i].getPosition().x + (previewSize - nameBounds.width) / 2,
            boardPreviews[i].getPosition().y + previewSize + 15.f
        );
    }

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile("audio/move.wav"))
        cout << "Click sound not found!\n";
    Sound clickSound;
    clickSound.setBuffer(clickBuffer);

    int selectedTheme = -1;

    while (window.isOpen() && selectedTheme == -1) {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) {
                window.close();
                backgroundSound.stop();
                return -2;  // FIXED: Return -2 to indicate window was closed
            }

            if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);

                if (backButton.getGlobalBounds().contains(mousePos)) {
                    clickSound.play();
                    backgroundSound.stop();
                    return -1;  // Return -1 for back button
                }

                for (int i = 0; i < 4; i++) {
                    if (boardPreviews[i].getGlobalBounds().contains(mousePos)) {
                        clickSound.play();
                        selectedTheme = i;
                        break;
                    }
                }
            }
        }

        Vector2i mousePos = Mouse::getPosition(window);

        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            backButton.setFillColor(Color(150, 150, 150, 230));
            backButton.setOutlineColor(Color::Yellow);
        }
        else {
            backButton.setFillColor(Color(100, 100, 100, 200));
            backButton.setOutlineColor(Color::White);
        }

        for (int i = 0; i < 4; i++) {
            if (boardPreviews[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                boardPreviews[i].setOutlineColor(Color::Yellow);
                boardPreviews[i].setOutlineThickness(5.f);
                themeNames[i].setFillColor(Color::Yellow);
                themeNames[i].setScale(1.05f, 1.05f);
            }
            else {
                boardPreviews[i].setOutlineColor(Color::White);
                boardPreviews[i].setOutlineThickness(4.f);
                themeNames[i].setFillColor(Color::White);
                themeNames[i].setScale(1.0f, 1.0f);
            }
        }

        window.clear();
        window.draw(menuBG);
        window.draw(overlay);
        window.draw(title);
        window.draw(subtitle);
        window.draw(backButton);
        window.draw(backText);

        for (int i = 0; i < 4; i++) {
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                    RectangleShape tile(Vector2f(previewSize / 8.f, previewSize / 8.f));
                    tile.setPosition(
                        boardPreviews[i].getPosition().x + c * previewSize / 8.f,
                        boardPreviews[i].getPosition().y + r * previewSize / 8.f
                    );

                    if ((r + c) % 2 == 0)
                        tile.setFillColor(themes[i].lightTile);
                    else
                        tile.setFillColor(themes[i].darkTile);

                    window.draw(tile);
                }
            }

            window.draw(boardPreviews[i]);
            window.draw(themeNames[i]);
        }

        window.display();
    }

    backgroundSound.stop();
    return selectedTheme;
}
std::string getBoxName(int row, int col) {
    char file = 'a' + col;         // column a-h
    char rank = '8' - row;         // row 0(top) -> 8, row 7(bottom) -> 1
    return std::string() + file + rank;
}
void drawBoardLabels(sf::RenderWindow& window, sf::Font& font, float tileSize, float offX, float offY) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            sf::Text text;
            text.setFont(font);
            text.setString(getBoxName(row, col));
            text.setCharacterSize(static_cast<unsigned int>(tileSize / 5)); // adjust for readability
            text.setFillColor(sf::Color::Red);  // choose color
            text.setPosition(offX + col * tileSize + 3, offY + row * tileSize + 3); // small padding
            window.draw(text);
        }
    }
}



void loadBoardFromFEN(const string& fen) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            boardLogic[i][j] = ' ';
            boardArr[i][j] = 0;
        }
    }

    int row = 0, col = 0;

    for (size_t idx = 0; idx < fen.length(); idx++) {
        char c = fen[idx];
        if (c == ' ') break;

        if (c == '/') {
            row++;
            col = 0;
        }
        else if (isdigit(c)) {
            col += (c - '0');
        }
        else {
            boardLogic[row][col] = c;

            switch (tolower(c)) {
            case 'p': boardArr[row][col] = isupper(c) ? 1 : -1; break;
            case 'r': boardArr[row][col] = isupper(c) ? 2 : -2; break;
            case 'n': boardArr[row][col] = isupper(c) ? 3 : -3; break;
            case 'b': boardArr[row][col] = isupper(c) ? 4 : -4; break;
            case 'q': boardArr[row][col] = isupper(c) ? 5 : -5; break;
            case 'k': boardArr[row][col] = isupper(c) ? 6 : -6; break;
            }
            col++;
        }
    }

    updateboard();
}
string moveToSAN(int fromR, int fromC, int toR, int toC) {
    char piece = boardLogic[fromR][fromC];
    string san = "";

    if (toupper(piece) != 'P') {
        san += toupper(piece);
    }

    bool isCapture = (boardLogic[toR][toC] != ' ');
    if (isCapture) {
        if (toupper(piece) == 'P') {
            san += char('a' + fromC);
        }
        san += 'x';
    }

    san += char('a' + toC);
    san += char('8' - toR);

    return san;
}
void runPuzzleMode(RenderWindow& window, ChessPuzzleSystem& puzzleSystem) {
    Texture texW[6], texB[6];
    texW[0].loadFromFile("pieces/white-pawn.png");   texB[0].loadFromFile("pieces/black-pawn.png");
    texW[1].loadFromFile("pieces/white-rook.png");   texB[1].loadFromFile("pieces/black-rook.png");
    texW[2].loadFromFile("pieces/white-knight.png"); texB[2].loadFromFile("pieces/black-knight.png");
    texW[3].loadFromFile("pieces/white-bishop.png"); texB[3].loadFromFile("pieces/black-bishop.png");
    texW[4].loadFromFile("pieces/white-queen.png");  texB[4].loadFromFile("pieces/black-queen.png");
    texW[5].loadFromFile("pieces/white-king.png");   texB[5].loadFromFile("pieces/black-king.png");

    Font font, titleFont;
    if (!font.loadFromFile("Font/roboto.ttf")) {
        cout << "Font not found!" << endl;
        return;
    }
    if (!titleFont.loadFromFile("Font/bebasneue.ttf")) {
        titleFont = font;
    }

    PuzzleDifficulty selectedDifficulty = PuzzleDifficulty::EASY;

    Puzzle currentPuzzle = puzzleSystem.getNextPuzzle(selectedDifficulty);
    puzzleSystem.startPuzzle(currentPuzzle);

    loadBoardFromFEN(currentPuzzle.fen);
    whiteTurn = currentPuzzle.whiteToMove;

    bool puzzleComplete = false;
    Clock feedbackClock;
    Clock puzzleTimer;
    puzzleTimer.restart();
    bool showFeedback = false;
    string currentHint = "";

    bool aiThinking = false;
    Clock aiClock;
    const float AI_DELAY = 2.0f;

    RectangleShape box;
    bool dragging = false;
    int dragR = -1, dragC = -1;
    int hoverRow = -1, hoverCol = -1;
    Sprite dragSprite;
    float dragOffsetX = 0, dragOffsetY = 0;

    // Back Button
    RectangleShape backButton(Vector2f(120.f, 50.f));
    backButton.setFillColor(Color(150, 50, 50, 220));
    backButton.setOutlineColor(Color::Red);
    backButton.setOutlineThickness(3.f);
    Text backButtonText("< Back", font, 24);
    backButtonText.setFillColor(Color::White);

    // LEFT STATUS BOX
    RectangleShape statusBox(Vector2f(300.f, 550.f));
    statusBox.setFillColor(Color(30, 30, 30, 240));
    statusBox.setOutlineColor(Color(255, 215, 0));
    statusBox.setOutlineThickness(3.f);

    Text statusTitle("PUZZLE STATUS", titleFont, 28);
    statusTitle.setFillColor(Color(255, 215, 0));
    statusTitle.setStyle(Text::Bold);

    Text statusMessage("Think Smartly!", font, 24);
    statusMessage.setFillColor(Color::White);
    statusMessage.setStyle(Text::Bold);

    Text ratingText("Rating: " + to_string(puzzleSystem.getRating()), font, 22);
    ratingText.setFillColor(Color::Cyan);

    Text streakText("Streak: " + to_string(puzzleSystem.getStreak()) + " days", font, 22);
    streakText.setFillColor(Color::Green);

    Text attemptsText("Attempts: 3/3", font, 22);
    attemptsText.setFillColor(Color::White);

    Text timerText("Time: 00:00", font, 24);
    timerText.setFillColor(Color::Yellow);
    timerText.setStyle(Text::Bold);

    Text hintDisplayText("", font, 18);
    hintDisplayText.setFillColor(Color::Yellow);

    // RIGHT INFO BOX
    RectangleShape infoBox(Vector2f(300.f, 300.f));
    infoBox.setFillColor(Color(40, 40, 40, 240));
    infoBox.setOutlineColor(Color::White);
    infoBox.setOutlineThickness(3.f);

    Text puzzleTitle("CHESS PUZZLE", titleFont, 26);
    puzzleTitle.setFillColor(Color::Yellow);
    puzzleTitle.setStyle(Text::Bold);

    Text objectiveText("Objective:\n" + currentPuzzle.objective, font, 20);
    objectiveText.setFillColor(Color(255, 100, 100));
    objectiveText.setStyle(Text::Bold);

    Text themeText("Theme: " + currentPuzzle.theme, font, 19);
    themeText.setFillColor(Color(255, 200, 100));

    Text difficultyDisplayText("Difficulty: Easy", font, 19);
    difficultyDisplayText.setFillColor(Color::Magenta);

    Text descText(currentPuzzle.description, font, 18);
    descText.setFillColor(Color::White);

    // Difficulty Buttons
    vector<RectangleShape> difficultyButtons(4);
    vector<Text> difficultyTexts(4);
    string diffNames[] = { "Easy", "Intermediate", "Hard", "Ultra Hard" };
    Color diffColors[] = { Color::Green, Color::Yellow, Color(255, 165, 0), Color::Red };

    for (int i = 0; i < 4; i++) {
        difficultyButtons[i].setSize(Vector2f(140.f, 45.f));
        difficultyButtons[i].setFillColor(Color(60, 60, 60, 220));
        difficultyButtons[i].setOutlineThickness(2.f);
        difficultyButtons[i].setOutlineColor(diffColors[i]);

        difficultyTexts[i].setString(diffNames[i]);
        difficultyTexts[i].setFont(font);
        difficultyTexts[i].setCharacterSize(17);
        difficultyTexts[i].setFillColor(Color::White);
    }

    // Action Buttons
    RectangleShape hintButton(Vector2f(140.f, 50.f));
    hintButton.setFillColor(Color(70, 70, 150, 220));
    hintButton.setOutlineColor(Color::Yellow);
    hintButton.setOutlineThickness(3.f);
    Text hintButtonText("Get Hint", font, 22);
    hintButtonText.setFillColor(Color::White);

    RectangleShape nextButton(Vector2f(140.f, 50.f));
    nextButton.setFillColor(Color(50, 150, 50, 220));
    nextButton.setOutlineColor(Color::Green);
    nextButton.setOutlineThickness(3.f);
    Text nextButtonText("Next Puzzle", font, 20);
    nextButtonText.setFillColor(Color::White);

    while (window.isOpen()) {
        float size = std::min((float)window.getSize().x, (float)window.getSize().y) * 0.8f;
        tileW = tileH = size / 8.f;
        offX = ((float)window.getSize().x - size) / 2.f;
        offY = ((float)window.getSize().y - size) / 2.f;

        float sidePadding = 20.0f;
        float boardPadding = 30.0f;

        // LEFT BOX POSITIONING
        float leftBoxWidth = 300.0f;
        float leftBoxX = sidePadding;
        float leftBoxY = 100.0f;

        if (leftBoxX + leftBoxWidth + boardPadding > offX) {
            leftBoxX = offX - leftBoxWidth - boardPadding;
            if (leftBoxX < sidePadding) leftBoxX = sidePadding;
        }

        statusBox.setPosition(leftBoxX, leftBoxY);
        statusTitle.setPosition(statusBox.getPosition().x + 40.f, statusBox.getPosition().y + 15.f);
        statusMessage.setPosition(statusBox.getPosition().x + 25.f, statusBox.getPosition().y + 80.f);
        ratingText.setPosition(statusBox.getPosition().x + 25.f, statusBox.getPosition().y + 160.f);
        streakText.setPosition(statusBox.getPosition().x + 25.f, statusBox.getPosition().y + 205.f);
        attemptsText.setPosition(statusBox.getPosition().x + 25.f, statusBox.getPosition().y + 250.f);
        timerText.setPosition(statusBox.getPosition().x + 25.f, statusBox.getPosition().y + 295.f);
        hintDisplayText.setPosition(statusBox.getPosition().x + 25.f, statusBox.getPosition().y + 360.f);

        backButton.setPosition(leftBoxX, 30.f);
        backButtonText.setPosition(backButton.getPosition().x + 25.f, backButton.getPosition().y + 12.f);

        // RIGHT BOX POSITIONING
        float boardRightEdge = offX + size;
        float rightBoxWidth = 300.0f;
        float rightBoxX = boardRightEdge + boardPadding;
        float screenRightEdge = (float)window.getSize().x - sidePadding;

        if (rightBoxX + rightBoxWidth > screenRightEdge) {
            rightBoxWidth = screenRightEdge - rightBoxX;
            if (rightBoxWidth < 250.f) {
                rightBoxX = offX;
                rightBoxWidth = size;
            }
        }

        float rightBoxY = 80.0f;

        infoBox.setSize(Vector2f(rightBoxWidth, 300.f));
        infoBox.setPosition(rightBoxX, rightBoxY);
        puzzleTitle.setPosition(infoBox.getPosition().x + 50.f, infoBox.getPosition().y + 15.f);
        objectiveText.setPosition(infoBox.getPosition().x + 20.f, infoBox.getPosition().y + 65.f);
        themeText.setPosition(infoBox.getPosition().x + 20.f, infoBox.getPosition().y + 140.f);
        difficultyDisplayText.setPosition(infoBox.getPosition().x + 20.f, infoBox.getPosition().y + 180.f);
        descText.setPosition(infoBox.getPosition().x + 20.f, infoBox.getPosition().y + 220.f);

        // Difficulty buttons positioning
        float diffButtonStartY = infoBox.getPosition().y + 310.f;
        float diffButtonSpacing = 10.0f;
        for (int i = 0; i < 4; i++) {
            int row = i / 2;
            int col = i % 2;
            difficultyButtons[i].setPosition(
                rightBoxX + (float)col * (140.f + diffButtonSpacing),
                diffButtonStartY + (float)row * 55.f
            );
            FloatRect bounds = difficultyTexts[i].getLocalBounds();
            difficultyTexts[i].setPosition(
                difficultyButtons[i].getPosition().x + (140.f - bounds.width) / 2.f,
                difficultyButtons[i].getPosition().y + 12.f
            );
        }

        // Action buttons positioning
        float actionButtonY = diffButtonStartY + 120.f;
        hintButton.setPosition(rightBoxX, actionButtonY);
        hintButtonText.setPosition(hintButton.getPosition().x + 30.f, hintButton.getPosition().y + 12.f);

        nextButton.setPosition(rightBoxX + 150.f, actionButtonY);
        nextButtonText.setPosition(nextButton.getPosition().x + 18.f, nextButton.getPosition().y + 12.f);

        // EVENT HANDLING
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed)
                window.close();

            if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                Vector2f mousePos((float)ev.mouseButton.x, (float)ev.mouseButton.y);

                // Back button
                if (backButton.getGlobalBounds().contains(mousePos)) {
                    puzzleSystem.saveProgress();
                    return;
                }

                // Difficulty buttons
                for (int i = 0; i < 4; i++) {
                    if (difficultyButtons[i].getGlobalBounds().contains(mousePos)) {
                        selectedDifficulty = static_cast<PuzzleDifficulty>(i);
                        currentPuzzle = puzzleSystem.getNextPuzzle(selectedDifficulty);
                        puzzleSystem.startPuzzle(currentPuzzle);
                        loadBoardFromFEN(currentPuzzle.fen);
                        whiteTurn = currentPuzzle.whiteToMove;

                        objectiveText.setString("Objective:\n" + currentPuzzle.objective);
                        themeText.setString("Theme: " + currentPuzzle.theme);
                        difficultyDisplayText.setString("Difficulty: " + diffNames[i]);
                        descText.setString(currentPuzzle.description);

                        puzzleComplete = false;
                        showFeedback = false;
                        currentHint = "";
                        hintDisplayText.setString("");
                        attemptsText.setString("Attempts: 3/3");
                        attemptsText.setFillColor(Color::White);
                        statusMessage.setString("Think Smartly!");
                        statusMessage.setFillColor(Color::White);
                        puzzleTimer.restart();
                        aiThinking = false;
                    }
                }

                // Hint button
                if (hintButton.getGlobalBounds().contains(mousePos) && !puzzleComplete) {
                    string hint = puzzleSystem.getHint();
                    currentHint = hint;
                    hintDisplayText.setString("Hint:\n" + hint);
                    statusMessage.setString("Hint Used!");
                    statusMessage.setFillColor(Color::Yellow);
                    showFeedback = true;
                    feedbackClock.restart();
                }

                // Next puzzle button
                if (nextButton.getGlobalBounds().contains(mousePos)) {
                    currentPuzzle = puzzleSystem.getNextPuzzle(selectedDifficulty);
                    puzzleSystem.startPuzzle(currentPuzzle);
                    loadBoardFromFEN(currentPuzzle.fen);
                    whiteTurn = currentPuzzle.whiteToMove;

                    objectiveText.setString("Objective:\n" + currentPuzzle.objective);
                    themeText.setString("Theme: " + currentPuzzle.theme);
                    descText.setString(currentPuzzle.description);
                    ratingText.setString("Rating: " + to_string(puzzleSystem.getRating()));
                    streakText.setString("Streak: " + to_string(puzzleSystem.getStreak()) + " days");

                    puzzleComplete = false;
                    showFeedback = false;
                    currentHint = "";
                    hintDisplayText.setString("");
                    attemptsText.setString("Attempts: 3/3");
                    attemptsText.setFillColor(Color::White);
                    statusMessage.setString("Think Smartly!");
                    statusMessage.setFillColor(Color::White);
                    puzzleTimer.restart();
                    aiThinking = false;
                }

                // Piece dragging
                if (!puzzleComplete && !aiThinking && whiteTurn == currentPuzzle.whiteToMove) {
                    int mx = ev.mouseButton.x;
                    int my = ev.mouseButton.y;
                    int col = (int)((mx - offX) / tileW);
                    int row = (int)((my - offY) / tileH);

                    if (isInsideBoard(row, col) && boardArr[row][col] != 0) {
                        char p = boardLogic[row][col];
                        if ((currentPuzzle.whiteToMove && isupper(p)) ||
                            (!currentPuzzle.whiteToMove && islower(p))) {
                            dragging = true;
                            dragR = row;
                            dragC = col;
                            int id = boardArr[row][col];
                            dragSprite.setScale(0.75f, 0.75f);
                            dragSprite.setTexture(id > 0 ? texW[id - 1] : texB[-id - 1]);
                            dragOffsetX = (float)mx - ((float)col * tileW + offX);
                            dragOffsetY = (float)my - ((float)row * tileH + offY);
                        }
                    }
                }
            }

            if (ev.type == Event::MouseButtonReleased && ev.mouseButton.button == Mouse::Left) {
                if (dragging && !puzzleComplete && !aiThinking) {
                    dragging = false;
                    int mx = ev.mouseButton.x;
                    int my = ev.mouseButton.y;
                    int col = (int)((mx - offX) / tileW);
                    int row = (int)((my - offY) / tileH);

                    if (isInsideBoard(row, col) && isValidMove(dragR, dragC, row, col)) {
                        string moveSAN = moveToSAN(dragR, dragC, row, col);
                        makeMove(dragR, dragC, row, col);
                        updateboard();

                        PuzzleResult result = puzzleSystem.checkMove(moveSAN);

                        if (result.isComplete) {
                            puzzleComplete = true;
                            if (result.correct) {
                                statusMessage.setString("PUZZLE SOLVED!\n\n+" +
                                    to_string(result.ratingChange) + " Points!");
                                statusMessage.setFillColor(Color::Green);
                            }
                            else {
                                statusMessage.setString("PUZZLE FAILED!\n\n" +
                                    to_string(result.ratingChange) + " Points");
                                statusMessage.setFillColor(Color::Red);
                            }
                            showFeedback = true;
                            feedbackClock.restart();
                            ratingText.setString("Rating: " + to_string(puzzleSystem.getRating()));
                            streakText.setString("Streak: " + to_string(puzzleSystem.getStreak()) + " days");
                        }
                        else if (result.correct) {
                            statusMessage.setString("Correct!\nContinue...");
                            statusMessage.setFillColor(Color::Green);
                            showFeedback = true;
                            feedbackClock.restart();
                            whiteTurn = !whiteTurn;
                            aiThinking = true;
                            aiClock.restart();
                        }
                        else {
                            statusMessage.setString("WRONG!\n\n" + result.message);
                            statusMessage.setFillColor(Color::Red);
                            showFeedback = true;
                            feedbackClock.restart();
                            loadBoardFromFEN(currentPuzzle.fen);

                            int remaining = puzzleSystem.getRemainingAttempts();
                            attemptsText.setString("Attempts: " + to_string(remaining) + "/3");
                            if (remaining <= 1) attemptsText.setFillColor(Color::Red);
                            else if (remaining == 2) attemptsText.setFillColor(Color(255, 165, 0));
                        }
                    }
                }
                dragging = false;
            }

            if (ev.type == Event::MouseMoved) {
                hoverCol = (int)((ev.mouseMove.x - offX) / tileW);
                hoverRow = (int)((ev.mouseMove.y - offY) / tileH);
            }
        }

        // AI move after delay
        if (aiThinking && aiClock.getElapsedTime().asSeconds() >= AI_DELAY) {
            if (puzzleSystem.hasPuzzle()) {
                whiteTurn = !whiteTurn;
            }
            aiThinking = false;
        }

        // Reset status message
        if (showFeedback && feedbackClock.getElapsedTime().asSeconds() > 3.0f && !puzzleComplete) {
            showFeedback = false;
            statusMessage.setString("Think Smartly!");
            statusMessage.setFillColor(Color::White);
        }

        // Update timer
        if (!puzzleComplete) {
            int elapsed = puzzleSystem.getElapsedTime();
            int minutes = elapsed / 60;
            int seconds = elapsed % 60;
            char timeStr[32];
            sprintf_s(timeStr, sizeof(timeStr), "Time: %02d:%02d", minutes, seconds);
            timerText.setString(timeStr);
        }

        // Hover effects
        Vector2i mousePos = Mouse::getPosition(window);

        for (int i = 0; i < 4; i++) {
            if (difficultyButtons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                difficultyButtons[i].setFillColor(Color(100, 100, 100, 240));
            }
            else {
                difficultyButtons[i].setFillColor(
                    static_cast<PuzzleDifficulty>(i) == selectedDifficulty ?
                    Color(90, 90, 90, 240) : Color(60, 60, 60, 220)
                );
            }
        }

        if (hintButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
            hintButton.setFillColor(Color(100, 100, 200, 240));
        }
        else {
            hintButton.setFillColor(Color(70, 70, 150, 220));
        }

        if (nextButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
            nextButton.setFillColor(Color(70, 200, 70, 240));
        }
        else {
            nextButton.setFillColor(Color(50, 150, 50, 220));
        }

        if (backButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
            backButton.setFillColor(Color(200, 70, 70, 240));
        }
        else {
            backButton.setFillColor(Color(150, 50, 50, 220));
        }

        // RENDERING
        window.clear(Color(20, 20, 20));

        drawBoard(window, box, dragging ? dragR : -1, dragging ? dragC : -1,
            hoverRow, hoverCol, lightColor, darkColor);
        drawPieces(window, texW, texB, dragging ? dragR : -1, dragging ? dragC : -1);

        if (dragging) {
            Vector2i mp = Mouse::getPosition(window);
            dragSprite.setPosition((float)mp.x - dragOffsetX, (float)mp.y - dragOffsetY);
            window.draw(dragSprite);
        }

        // Draw UI
        window.draw(statusBox);
        window.draw(statusTitle);
        window.draw(statusMessage);
        window.draw(ratingText);
        window.draw(streakText);
        window.draw(attemptsText);
        window.draw(timerText);
        if (!currentHint.empty()) {
            window.draw(hintDisplayText);
        }

        window.draw(infoBox);
        window.draw(puzzleTitle);
        window.draw(objectiveText);
        window.draw(themeText);
        window.draw(difficultyDisplayText);
        window.draw(descText);

        for (int i = 0; i < 4; i++) {
            window.draw(difficultyButtons[i]);
            window.draw(difficultyTexts[i]);
        }

        window.draw(hintButton);
        window.draw(hintButtonText);
        window.draw(nextButton);
        window.draw(nextButtonText);
        window.draw(backButton);
        window.draw(backButtonText);

        window.display();
    }
}
// =======================
//  MAIN
// =======================
int main() {
    // ---------------- WINDOW SETUP ----------------     
    VideoMode desk = VideoMode::getDesktopMode();
    RenderWindow window(desk, "Chess", Style::Default);
    window.setFramerateLimit(60);

    // ---------------- MENU ----------------  
    if (!backgroundSound.openFromFile("audio/background.wav")) {
        cout << "FAILED TO LOAD MENU MUSIC" << endl;
        return -1;
    }

    backgroundSound.setLoop(true);
    backgroundSound.setVolume(40.f);
    backgroundSound.play();

    bool isMuted = false;
    float menuVolume = 40.f;
    float currentVolume = menuVolume;

    ChessPuzzleSystem puzzleSystem;
    puzzleSystem.initializePuzzles();
    puzzleSystem.loadProgress();

    // ---------------- VOLUME SLIDER (MENU) ----------------
    RectangleShape sliderBar(Vector2f(200.f, 5.f));
    sliderBar.setFillColor(Color::White);
    sliderBar.setPosition(window.getSize().x / 2.f - 100.f, window.getSize().y - 100.f);

    RectangleShape sliderKnob(Vector2f(15.f, 20.f));
    sliderKnob.setFillColor(Color::Red);
    sliderKnob.setOrigin(sliderKnob.getSize().x / 2.f, sliderKnob.getSize().y / 2.f);
    sliderKnob.setPosition(sliderBar.getPosition().x + sliderBar.getSize().x * (menuVolume / 100.f),
        sliderBar.getPosition().y + sliderBar.getSize().y / 2.f);

    bool draggingSlider = false;

    // ---------------- MAIN MENU LOOP ----------------
    int choice = 0;
    bool menuActive = true;
    while (menuActive && window.isOpen()) {
        Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) {
                window.close();
                return 0;  // FIXED: Properly exit the program
            }

            if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::M) {
                isMuted = !isMuted;
                float vol = isMuted ? 0.f : currentVolume;
                backgroundSound.setVolume(vol);
            }

            if (ev.type == Event::MouseButtonPressed &&
                ev.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);
                if (sliderKnob.getGlobalBounds().contains(mousePos)) {
                    draggingSlider = true;
                }
            }

            if (ev.type == Event::MouseButtonReleased &&
                ev.mouseButton.button == Mouse::Left)
            {
                draggingSlider = false;
            }

            if (ev.type == Event::MouseMoved && draggingSlider) {
                float mx = ev.mouseMove.x;
                float barX = sliderBar.getPosition().x;
                float barWidth = sliderBar.getSize().x;

                mx = std::max(barX, std::min(barX + barWidth, mx));
                sliderKnob.setPosition(mx, sliderKnob.getPosition().y);

                currentVolume = ((mx - barX) / barWidth) * 100.f;
                if (!isMuted)
                    backgroundSound.setVolume(currentVolume);
            }
        }

        window.clear(Color::Black);
        window.draw(sliderBar);
        window.draw(sliderKnob);
        window.display();

        choice = showMainMenu(window);

        // FIXED: Check if window was closed in menu
        if (choice == -1 || !window.isOpen()) {
            return 0;  // Exit the program
        }

        menuActive = false;
    }

    if (!window.isOpen()) {
        return 0;  // Exit if window closed
    }

    if (choice == 4) {
        runPuzzleMode(window, puzzleSystem);
        puzzleSystem.saveProgress();
        return main();
    }

    // ---------------- STOP MENU MUSIC ----------------
    backgroundSound.stop();

    AIenabled = false;
    if (choice == 2) { AIenabled = true; AIisWhite = false; }
    else if (choice == 3) { AIenabled = true; AIisWhite = true; }

    if (AIenabled) {
        int diffChoice = showAIDifficultyMenu(window);

        // FIXED: Check if window was closed or back button pressed
        if (diffChoice == -1 || !window.isOpen()) {
            return 0;  // Exit the program
        }

        if (diffChoice == 0) {
            return main();  // Back button - restart from main menu
        }

        aiDifficulty = static_cast<AILevel>(std::max(0, std::min(diffChoice - 1, 2)));
    }

    // ---------------- SELECT BOARD THEME ----------------
    int selectedTheme = showBoardThemeMenu(window);

    // FIXED: Check if window was closed or back button pressed
    if (selectedTheme == -2 || !window.isOpen()) {
        return 0;  // Exit the program (window closed)
    }

    if (selectedTheme == -1) {
        return main();  // Back button - restart from main menu
    }

    // Define board themes
    struct BoardTheme { Color lightTile; Color darkTile; };
    BoardTheme themes[4] = {
        { Color(240, 217, 181), Color(181, 136, 99) },
        { Color(200, 255, 200), Color(50, 150, 50) },
        { Color(180, 210, 255), Color(50, 90, 200) },
        { Color(255, 200, 200), Color(200, 50, 50) }
    };

    lightColor = themes[selectedTheme].lightTile;
    darkColor = themes[selectedTheme].darkTile;
    // ---------------- INIT BOARD ----------------     
    srand(static_cast<unsigned>(time(nullptr)));
    initializeBoardLogic();
    updateboard();

    // ---------------- SOUNDS ----------------     
    moveBuffer.loadFromFile("audio/move.wav");
    startBuffer.loadFromFile("audio/Start.wav");
    completingBuffer.loadFromFile("audio/Completing.wav");
    castling.loadFromFile("audio/castling.wav");
    stalemate.loadFromFile("audio/stalemate.wav");
    checkmate.loadFromFile("audio/checkmate.wav");
    capture.loadFromFile("audio/capture.wav");

    moveSound.setBuffer(moveBuffer);
    startSound.setBuffer(startBuffer);
    completingSound.setBuffer(completingBuffer);
    castlingSound.setBuffer(castling);
    stalemateSound.setBuffer(stalemate);
    checkmateSound.setBuffer(checkmate);
    captureSound.setBuffer(capture);

    float gameVolume = isMuted ? 0.f : currentVolume;
    moveSound.setVolume(gameVolume);
    startSound.setVolume(gameVolume);
    completingSound.setVolume(gameVolume);
    castlingSound.setVolume(gameVolume);
    stalemateSound.setVolume(gameVolume);
    checkmateSound.setVolume(gameVolume);
    captureSound.setVolume(gameVolume);

    startSound.play();

    // ---------------- TEXTURES ----------------     
    Texture texW[6], texB[6];
    texW[0].loadFromFile("pieces/white-pawn.png");   texB[0].loadFromFile("pieces/black-pawn.png");
    texW[1].loadFromFile("pieces/white-rook.png");   texB[1].loadFromFile("pieces/black-rook.png");
    texW[2].loadFromFile("pieces/white-knight.png"); texB[2].loadFromFile("pieces/black-knight.png");
    texW[3].loadFromFile("pieces/white-bishop.png"); texB[3].loadFromFile("pieces/black-bishop.png");
    texW[4].loadFromFile("pieces/white-queen.png");  texB[4].loadFromFile("pieces/black-queen.png");
    texW[5].loadFromFile("pieces/white-king.png");   texB[5].loadFromFile("pieces/black-king.png");

    RectangleShape box;
    Event ev;

    // ---------------- GAME STATE ----------------     
    bool dragging = false;
    bool aiThinking = false;
   // const float AI_DELAY = 0.35f;
    Clock aiClock;

    bool gameOver = false;
    whiteTurn = true;
    int dragR = -1, dragC = -1;
    int hoverRow = -1, hoverCol = -1;

    // ---------------- IN-GAME VOLUME SLIDER ----------------
    RectangleShape gameSliderBar(Vector2f(150.f, 4.f));
    gameSliderBar.setFillColor(Color::White);
    gameSliderBar.setPosition(window.getSize().x - 180.f, 20.f);

    RectangleShape gameSliderKnob(Vector2f(12.f, 16.f));
    gameSliderKnob.setFillColor(Color::Red);
    gameSliderKnob.setOrigin(gameSliderKnob.getSize().x / 2.f, gameSliderKnob.getSize().y / 2.f);
    gameSliderKnob.setPosition(gameSliderBar.getPosition().x + gameSliderBar.getSize().x * (currentVolume / 100.f),
        gameSliderBar.getPosition().y + gameSliderBar.getSize().y / 2.f);

    bool draggingGameSlider = false;

    // ---------------- UNDO / REDO BUTTONS ----------------
    RectangleShape undoButton(Vector2f(80.f, 40.f));
    RectangleShape redoButton(Vector2f(80.f, 40.f));

    undoButton.setPosition(20.f, 20.f);
    redoButton.setPosition(120.f, 20.f);

    undoButton.setFillColor(Color(50, 50, 50, 220));
    redoButton.setFillColor(Color(50, 50, 50, 220));

    undoButton.setOutlineColor(Color::White);
    undoButton.setOutlineThickness(2.f);
    redoButton.setOutlineColor(Color::White);
    redoButton.setOutlineThickness(2.f);

    Font ft;
    if (!ft.loadFromFile("Font/bebasneue.ttf")) {
        cout << "Failed to load font!" << endl;
        return -1;
    }
    Text undoText("Undo", ft, 18);
    Text redoText("Redo", ft, 18);

    undoText.setFillColor(Color::White);
    redoText.setFillColor(Color::White);

    undoText.setPosition(undoButton.getPosition().x + 15.f, undoButton.getPosition().y + 8.f);
    redoText.setPosition(redoButton.getPosition().x + 15.f, redoButton.getPosition().y + 8.f);

    // ---------------- RESET FUNCTION ----------------     
    auto resetGame = [&]() {
        initializeBoardLogic();
        updateboard();
        whiteTurn = true;
        gameOver = false;
        dragR = dragC = hoverRow = hoverCol = -1;
        for (int i = 0; i < 16; i++) {
            whiteCaptured[i] = ' ';
            blackCaptured[i] = ' ';
        }
        whiteCapCount = blackCapCount = 0;
        };

    // ---------------- ENDGAME OVERLAY ----------------     
    auto showEndOverlay = [&](const std::string& msg) {
        RectangleShape overlayBox(Vector2f(400.f, 150.f));
        overlayBox.setFillColor(Color(50, 50, 50, 220));
        overlayBox.setOutlineColor(Color::White);
        overlayBox.setOutlineThickness(3.f);
        overlayBox.setOrigin(overlayBox.getSize() / 2.f);
        overlayBox.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

        Text text(msg, ft, 24);
        text.setFillColor(Color::White);
        text.setStyle(Text::Bold);
        text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
        text.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

        Clock clock;
        while (clock.getElapsedTime().asSeconds() < 3.f) {
            Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == Event::Closed) window.close();
            }

            window.clear();
            drawBoard(window, box, -1, -1, -1, -1, lightColor, darkColor);
            drawPieces(window, texW, texB, -1, -1);
            drawCaptured(window, texW, texB);

            window.draw(overlayBox);
            window.draw(text);
            window.display();
        }
        };

    // ---------------- INIT AI THINKING FOR FIRST MOVE ----------------
    if (AIenabled && whiteTurn == AIisWhite) {
        aiThinking = true;
        aiClock.restart();
    }

    // ---------------- MAIN GAME LOOP ----------------     
    while (window.isOpen()) {
        float size = std::min(window.getSize().x, window.getSize().y);
        tileW = tileH = size / 8.f;
        offX = (window.getSize().x - size) / 2.f;
        offY = (window.getSize().y - size) / 2.f;

        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed)
                window.close();

            // ---------------- IN-GAME MUTE & SLIDER ----------------
            if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::M) {
                isMuted = !isMuted;
                float vol = isMuted ? 0.f : currentVolume;
                moveSound.setVolume(vol);
                startSound.setVolume(vol);
                completingSound.setVolume(vol);
                castlingSound.setVolume(vol);
                stalemateSound.setVolume(vol);
                checkmateSound.setVolume(vol);
                captureSound.setVolume(vol);
            }

            if (ev.type == Event::MouseButtonPressed &&
                ev.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(ev.mouseButton.x, ev.mouseButton.y);

                // ---------------- UNDO / REDO BUTTONS ----------------
                if (undoButton.getGlobalBounds().contains(mousePos) && !aiThinking) {
                    undoMove(aiThinking);
                    updateboard();

                    // ADDED: If it's AI's turn after undo, start AI thinking
                    if (AIenabled && whiteTurn == AIisWhite) {
                        aiThinking = true;
                        aiClock.restart();
                    }
                }

                if (redoButton.getGlobalBounds().contains(mousePos) && !aiThinking) {
                    redoMove(aiThinking);
                    updateboard();

                    // ADDED: If it's AI's turn after redo, start AI thinking
                    if (AIenabled && whiteTurn == AIisWhite) {
                        aiThinking = true;
                        aiClock.restart();
                    }
                }


                // ---------------- GAME SLIDER ----------------
                if (gameSliderKnob.getGlobalBounds().contains(mousePos)) draggingGameSlider = true;

                // ---------------- DRAGGING PIECES ----------------
                bool humanTurn = !(AIenabled && whiteTurn == AIisWhite);
                if (humanTurn) {
                    int mx = ev.mouseButton.x;
                    int my = ev.mouseButton.y;
                    int col = (mx - offX) / tileW;
                    int row = (my - offY) / tileH;

                    if (isInsideBoard(row, col) && boardArr[row][col] != 0) {
                        char p = boardLogic[row][col];
                        if ((whiteTurn && isupper(p)) || (!whiteTurn && islower(p))) {
                            dragging = true;
                            dragR = row; dragC = col;
                            int id = boardArr[row][col];
                            dragSprite.setScale(0.75f, 0.75f);
                            dragSprite.setTexture(id > 0 ? texW[id - 1] : texB[-id - 1]);
                            dragOffsetX = mx - (col * tileW + offX);
                            dragOffsetY = my - (row * tileH + offY);
                        }
                    }
                }
            }

            if (ev.type == Event::MouseButtonReleased &&
                ev.mouseButton.button == Mouse::Left)
            {
                draggingGameSlider = false;

                if (dragging) {
                    dragging = false;
                    int mx = ev.mouseButton.x;
                    int my = ev.mouseButton.y;
                    int col = (mx - offX) / tileW;
                    int row = (my - offY) / tileH;

                    if (isInsideBoard(row, col) && isValidMove(dragR, dragC, row, col)) {
                        recordStateBeforeMove();
                        makeMove(dragR, dragC, row, col);
                        updateboard();
                        moveSound.play();

                        if ((boardLogic[row][col] == 'P' && row == 0) ||
                            (boardLogic[row][col] == 'p' && row == 7)) {
                            bool isWhite = isupper(boardLogic[row][col]);
                            boardLogic[row][col] = showPromotionMenu(window, isWhite, texW, texB);
                            updateboard();
                        }

                        // ---------- CHECK ENDGAME ----------
                        if (isCheckmate(!whiteTurn)) {
                            gameOver = true;
                            checkmateSound.play();
                            showEndOverlay(whiteTurn ? "Checkmate by White!" : "Checkmate by Black!");
                            int res = showEndGameMenu(window, whiteTurn ? "White wins by Checkmate!" : "Black wins by Checkmate!");
                            if (res == 0) {
                                resetGame(); return main();
                            }
                            else return 0;
                        }
                        else if (isStalemate(!whiteTurn)) {
                            gameOver = true;
                            stalemateSound.play();
                            showEndOverlay("Stalemate! Draw!");
                            int res = showEndGameMenu(window, "Stalemate! Draw!");
                            if (res == 0) {
                                resetGame(); return main();
                            }
                            else return 0;
                        }

                        whiteTurn = !whiteTurn;
                        if (AIenabled && whiteTurn == AIisWhite) {
                            aiThinking = true;
                            aiClock.restart();
                        }
                    }
                }
            }

            if (ev.type == Event::MouseMoved) {
                hoverCol = (ev.mouseMove.x - offX) / tileW;
                hoverRow = (ev.mouseMove.y - offY) / tileH;

                if (draggingGameSlider) {
                    float mx = ev.mouseMove.x;
                    float barX = gameSliderBar.getPosition().x;
                    float barWidth = gameSliderBar.getSize().x;

                    mx = std::max(barX, std::min(barX + barWidth, mx));
                    gameSliderKnob.setPosition(mx, gameSliderKnob.getPosition().y);

                    currentVolume = ((mx - barX) / barWidth) * 100.f;
                    if (!isMuted) {
                        moveSound.setVolume(currentVolume);
                        startSound.setVolume(currentVolume);
                        completingSound.setVolume(currentVolume);
                        castlingSound.setVolume(currentVolume);
                        stalemateSound.setVolume(currentVolume);
                        checkmateSound.setVolume(currentVolume);
                        captureSound.setVolume(currentVolume);
                    }
                }
            }
        }

        // ---------------- AI MOVE ----------------
        if (!gameOver && aiThinking) {
            applyAIMove(AIisWhite);
            updateboard();
            moveSound.play();
            whiteTurn = !whiteTurn;
            aiThinking = false;

            // ADDED: Check game over after AI move
            if (isCheckmate(!whiteTurn)) {
                gameOver = true;
                checkmateSound.play();
                showEndOverlay(!whiteTurn ? "Checkmate by White!" : "Checkmate by Black!");
                int res = showEndGameMenu(window, !whiteTurn ? "White wins by Checkmate!" : "Black wins by Checkmate!");
                if (res == 0) {
                    resetGame();
                    // Restart game logic here
                    return main();
                }
                else {
                    window.close();
                }
            }
            else if (isStalemate(!whiteTurn)) {
                gameOver = true;
                stalemateSound.play();
                showEndOverlay("Stalemate! Draw!");
                int res = showEndGameMenu(window, "Stalemate! Draw!");
                if (res == 0) {
                    resetGame();
                    // Restart game logic here
                    return main();
                }
                else {
                    window.close();
                }
            }
        }
        // ---------------- DRAW ----------------
        window.clear();
        drawBoard(window, box,
            dragging ? dragR : -1,
            dragging ? dragC : -1,
            hoverRow, hoverCol,
            lightColor, darkColor);
        Font fot;
        if (!fot.loadFromFile("Font/arial.ttf")) {
            cout << "Failed to load font!" << endl;
            return -1;
        }
        drawBoardLabels(window, fot, tileW, offX, offY);


        drawPieces(window, texW, texB,
            dragging ? dragR : -1,
            dragging ? dragC : -1);
       
        drawCaptured(window, texW, texB);

        if (dragging) {
            Vector2i mp = Mouse::getPosition(window);
            dragSprite.setPosition(mp.x - dragOffsetX,
                mp.y - dragOffsetY);
            window.draw(dragSprite);
        }

        // ---------------- DRAW IN-GAME SLIDERS ----------------
        window.draw(gameSliderBar);
        window.draw(gameSliderKnob);

        // ---------------- DRAW UNDO / REDO BUTTONS ----------------
        if (undoStack.empty()) undoButton.setFillColor(Color(80, 80, 80, 180));
        else undoButton.setFillColor(Color(50, 50, 50, 220));

        if (redoStack.empty()) redoButton.setFillColor(Color(80, 80, 80, 180));
        else redoButton.setFillColor(Color(50, 50, 50, 220));

        window.draw(undoButton);
        window.draw(redoButton);
        window.draw(undoText);
        window.draw(redoText);

        window.display();
    }

    return 0;
}



