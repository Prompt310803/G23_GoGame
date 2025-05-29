#include "player.h"
#include <iostream>

// Constructor for base class Player. Sets name, color, and starts score at 0.
Player::Player(const std::string &name, char color) : name(name), color(color), score(0) {}

// Returns the player's name
std::string Player::getName() const
{
    return name;
}

// Returns the player's color ('B' or 'W')
char Player::getColor() const
{
    return color;
}

// Returns the player's current score
int Player::getScore() const
{
    return score;
}

// Adds points to the player's score
void Player::addScore(int s)
{
    score += s;
}

// Constructor for a human player (inherits from Player)
HumanPlayer::HumanPlayer(const std::string &name, char color) : Player(name, color) {}

// Lets the human player type in their move
void HumanPlayer::makeMove(int &x, int &y)
{
    std::string input;
    std::cout << getName() << " (" << getColor() << ") - Enter your move (e.g., A3) or P to pass, R to resign: ";
    std::cin >> input;

    // If the player wants to pass
    if (input == "P" || input == "p")
    {
        x = y = -1;  // -1,-1 means pass
        return;
    }

    // If the player wants to resign
    if (input == "R" || input == "r" || input == "resign")
    {
        x = y = -99;  // -99,-99 means resign
        return;
    }

    // If the input is like A3, B5, etc.
    if (input.length() >= 2 && std::isalpha(input[0]))
    {
        // Convert the letter A, B, C... to 0, 1, 2...
        x = std::toupper(input[0]) - 'A';

        try
        {
            // Convert the number part (e.g., "3") to y = 2
            y = std::stoi(input.substr(1)) - 1;
        }
        catch (...)
        {
            // If something goes wrong, mark it as invalid input
            x = y = -2;
        }
    }
    else
    {
        // If the input format is wrong, also mark it as invalid
        x = y = -2;
    }
}
