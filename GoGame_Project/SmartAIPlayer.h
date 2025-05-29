#pragma once
#include "Player.h"
#include <string>

class SmartAIPlayer : public Player { // SmartAIPlayer inherits from Player
public:
    SmartAIPlayer(const std::string& name, char color); // Constructor that sets the name and color
    void makeMove(int& x, int& y) override; // Function that lets the AI make a move
};