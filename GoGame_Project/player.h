#pragma once // Prevents multiple inclusions of this header file
#include <string>

// Base Class
class Player
{
private:
    std::string name; // Player's name
    char color;       // 'B' is black and 'W' is white
    int score;        // Player's score

public:
    Player(const std::string &name, char color); // Constructor of Player including name and color

    // Virtual function: must be implemented by subclasses (HumanPlayer, AIPlayer)
    virtual void makeMove(int &x, int &y) = 0; // Receive the pos x and y using the reference pass

    // Getters
    std::string getName() const; // Const behind func means "No edit inside the object"
    char getColor() const;
    int getScore() const;

    // Score update
    void addScore(int s);
};