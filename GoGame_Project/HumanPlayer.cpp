#include "HumanPlayer.h"
#include <iostream>

// Constructor that passes name and color to the base Player class
HumanPlayer::HumanPlayer(const std::string &name, char color)
    : Player(name, color) {} // Call the base class constructor

// Asks the human player to enter their move
void HumanPlayer::makeMove(int &x, int &y)
{
    std::cout << getName() << " (" << getColor() << "), enter your move (row and column): "; // Prompt the player for input
    std::cin >> x >> y; // Read row and column input from the player
};
