#pragma once
#include "board.h"
#include "player.h"

// GameManager controls the full game flow
class GameManager
{
private:
    Board board;     // The Go board
    Player *player1; // Pointer to first player (can be HumanPlayer or AIPlayer)
    Player *player2; // Pointer to second player
    int currentTurn; // Tracks whose turn it is (even = player1, odd = player2)
    int passCount;   // Tracks consecutive passes (game ends when both players pass)

public:
    GameManager();    // Constructor: set up players and the board
    ~GameManager();   // Destructor: clean up player pointers
    void startGame(); // Starts the main game loop
};