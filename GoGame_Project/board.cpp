// Include the board class definition
#include "board.h"
// Include standard IO stream for printing
#include <iostream>
// Include queue for flood fill and territory detection
#include <queue>
// Include set to track unique neighboring colors
#include <set>

// Constructor: initializes board with given size and empty cells
Board::Board(int size)
    : size(size),
      grid(size, std::vector<char>(size, '.')),              // Main game grid
      previousGrid(size, std::vector<char>(size, '.')),      // Stores previous state for Ko rule
      blackCaptured(0), whiteCaptured(0) {}                  // Capture counters

// Display the current board state
void Board::display() const
{
    std::cout << "   ";
    for (int col = 0; col < size; ++col) // Print column numbers
    {
        std::cout << (col + 1) << " ";
    }
    std::cout << "\n";
    for (int row = 0; row < size; ++row) // Print each row
    {
        std::cout << static_cast<char>('A' + row) << "  "; // Row label A, B, C, ...
        for (int col = 0; col < size; ++col)
        {
            char cell = grid[row][col];
            if (cell == 'B') // Black stone with blue color
                std::cout << "\033[1;34mB\033[0m ";
            else if (cell == 'W') // White stone with red color
                std::cout << "\033[1;31mW\033[0m ";
            else // Empty spot
                std::cout << ". ";
        }
        std::cout << "\n";
    }
}

// Place a stone on the board and check for validity (e.g., Ko rule, suicide)
bool Board::placeStone(int x, int y, char color)
{
    if (x < 0 || y < 0 || x >= size || y >= size) // Out of bounds
    {
        std::cout << "Invalid position on the board.\n";
        return false;
    }
    if (grid[x][y] != '.') // Already occupied
    {
        std::cout << "This spot is already occupied!\n";
        return false;
    }

    std::vector<std::vector<char>> testGrid = grid; // Simulate move
    testGrid[x][y] = color;
    char opponent = (color == 'B') ? 'W' : 'B'; // Determine opponent

    const int dx[4] = {-1, 1, 0, 0}; // Direction vectors
    const int dy[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) // Check 4 neighbors
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && ny >= 0 && nx < size && ny < size && testGrid[nx][ny] == opponent)
        {
            std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));
            if (!hasLibertySim(nx, ny, opponent, visited, testGrid)) // Check if opponent group has no liberty
            {
                for (int i = 0; i < size; ++i)
                    for (int j = 0; j < size; ++j)
                        if (visited[i][j])
                            testGrid[i][j] = '.'; // Remove captured stones
            }
        }
    }

    if (testGrid == previousGrid) // Ko rule check
    {
        std::cout << "Ko rule violation: move would recreate previous board.\n";
        return false;
    }

    previousGrid = grid; // Save current grid as previous
    grid = testGrid;     // Apply simulated move

    for (int i = 0; i < size; ++i) // Count captures
        for (int j = 0; j < size; ++j)
            if (previousGrid[i][j] != '.' && grid[i][j] == '.')
                (previousGrid[i][j] == 'B') ? blackCaptured++ : whiteCaptured++;

    std::vector<std::vector<bool>> selfCheck(size, std::vector<bool>(size, false));
    if (!hasLiberty(x, y, color, selfCheck)) // Check for suicide
    {
        std::cout << "Invalid move: suicide not allowed.\n";
        grid = previousGrid;
        return false;
    }

    return true;
}

// Helper: remove captured stones from the board
void Board::removeCaptured(int x, int y, char color)
{
    std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));
    if (!hasLiberty(x, y, color, visited)) // Check liberties
    {
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                if (visited[i][j])
                    grid[i][j] = '.'; // Remove
    }
}

// Check if a group of stones has any liberty (normal grid)
bool Board::hasLiberty(int x, int y, char color, std::vector<std::vector<bool>> &visited)
{
    if (x < 0 || y < 0 || x >= size || y >= size)
        return false;
    if (visited[x][y])
        return false;
    if (grid[x][y] != color)
        return false;

    visited[x][y] = true;

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && ny >= 0 && nx < size && ny < size)
        {
            if (grid[nx][ny] == '.') return true;
            if (grid[nx][ny] == color && hasLiberty(nx, ny, color, visited)) return true;
        }
    }
    return false;
}

// Same as hasLiberty, but works on a simulated board state
bool Board::hasLibertySim(int x, int y, char color, std::vector<std::vector<bool>> &visited, const std::vector<std::vector<char>> &state)
{
    if (x < 0 || y < 0 || x >= size || y >= size)
        return false;
    if (visited[x][y])
        return false;
    if (state[x][y] != color)
        return false;

    visited[x][y] = true;

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && ny >= 0 && nx < size && ny < size)
        {
            if (state[nx][ny] == '.') return true;
            if (state[nx][ny] == color && hasLibertySim(nx, ny, color, visited, state)) return true;
        }
    }
    return false;
}

// Return the stone at a given cell
char Board::getCell(int x, int y) const
{
    return grid[x][y];
}

// Return number of captured stones for a color
int Board::getCaptured(char color) const
{
    return (color == 'B') ? blackCaptured : whiteCaptured;
}

// Use flood fill to determine which player owns a surrounded area
char Board::detectTerritoryOwner(int x, int y, std::vector<std::vector<bool>> &visited)
{
    std::queue<std::pair<int, int>> q;
    std::set<char> neighborColors;
    q.push({x, y});
    visited[x][y] = true;
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    while (!q.empty())
    {
        auto [cx, cy] = q.front(); q.pop();
        for (int d = 0; d < 4; ++d)
        {
            int nx = cx + dx[d];
            int ny = cy + dy[d];
            if (nx >= 0 && ny >= 0 && nx < size && ny < size)
            {
                if (grid[nx][ny] == '.' && !visited[nx][ny])
                {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
                else if (grid[nx][ny] != '.')
                {
                    neighborColors.insert(grid[nx][ny]);
                }
            }
        }
    }

    if (neighborColors.size() == 1)
        return *neighborColors.begin(); // Only one neighbor type => owned
    return '.'; // Mixed or none
}

// Count how many empty areas are surrounded fully by black or white
void Board::calculateTerritory(int &blackTerritory, int &whiteTerritory) const
{
    blackTerritory = 0;
    whiteTerritory = 0;
    std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (!visited[i][j] && grid[i][j] == '.')
            {
                char owner = const_cast<Board *>(this)->detectTerritoryOwner(i, j, visited);
                if (owner == 'B') blackTerritory++;
                else if (owner == 'W') whiteTerritory++;
            }
        }
    }
}
