#include "SmartAIPlayer.h"
#include <iostream>

SmartAIPlayer::SmartAIPlayer(const std::string& name, char color) 
    : Player(name, color) {} // Call base Player constructor to set name and color

void SmartAIPlayer::makeMove(int& x, int& y) {
    std::cout << getName() << " (Smart AI) is thinking...\n"; // Display AI thinking message
    x = rand() % 9; // Choose a random row from 0 to 8
    y = rand() % 9; // Choose a random column from 0 to 8
}
