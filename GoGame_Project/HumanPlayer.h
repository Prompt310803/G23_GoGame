#pragma once
#include "Player.h"
#include <string>

class HumanPlayer : public Player { // HumanPlayer inherits from Player
public:
    HumanPlayer(const std::string& name, char color); // Constructor that sets name and color
    void makeMove(int& x, int& y) override; // Function for making a move, overrides base class
};
