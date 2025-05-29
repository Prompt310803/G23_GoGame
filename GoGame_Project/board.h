#pragma once
#include <vector>

class Board
{
private:
    std::vector<std::vector<char>> grid; // 2D grid
    std::vector<std::vector<char>> previousGrid;  // Stores the previous board state for ko rule checking
    int size; // Size of the board
    int blackCaptured; // Counter for captured black stones by white
    int whiteCaptured; // Counter for captured white stones by black
    void removeCaptured(int x, int y, char color); // Removes a group of stones of 'color'
    bool hasLiberty(int x, int y, char color, std::vector<std::vector<bool>> &visited); // Checks if the group of 'color' has any liberties
    bool hasLibertySim(int x, int y, char color, std::vector<std::vector<bool>> &visited, const std::vector<std::vector<char>> &state); // Similar to hasLiberty(), but works on a simulated board state (used for ko/legal move checking)
    char detectTerritoryOwner(int x, int y, std::vector<std::vector<bool>> &visited); // Performs flood fill to determine territory ownership (used in scoring)

public:
    Board(int size = 9); // Constructor initializes board
    void display() const; // Prints the current board state to console
    bool placeStone(int x, int y, char color);
    char getCell(int x, int y) const; // Returns the value at the given cell
    int getCaptured(char color) const; // Returns number of opponent stones captured by the specified color
    void calculateTerritory(int &blackTerritory, int &whiteTerritory) const; // Calculates territory controlled by each player and outputs via reference
};