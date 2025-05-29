#include "game_manager.h"  // Include the header for GameManager
#include "SmartAIPlayer.h" // Include SmartAIPlayer for AI logic
#include <iostream>        // For input and output

GameManager::GameManager()
{
    std::string mode, color; // Variables to store user choices

    std::cout << "Select opponent type:\n";              // Show menu to choose game mode
    std::cout << "1. Human vs Human (type: PVP)\n";      // Option 1: PVP
    std::cout << "2. Human vs SmartBot (type: SMART)\n"; // Option 2: SMART AI
    std::cout << "Enter mode: ";                         // Ask for input
    std::cin >> mode;                                    // Read input

    if (mode == "PVP" || mode == "pvp") // If user chooses human vs human
    {
        player1 = new HumanPlayer("Player 1", 'B'); // Player 1 is black
        player2 = new HumanPlayer("Player 2", 'W'); // Player 2 is white
    }
    else if (mode == "SMART" || mode == "smart") // If user chooses human vs AI
    {
        std::cout << "Do you want to play as Black or White? (B/W): "; // Ask for color
        std::cin >> color;                                             // Read input
        bool playerIsBlack = (color == "B" || color == "b");           // Check if player is black

        if (playerIsBlack)
        {
            player1 = new HumanPlayer("Player", 'B');     // Human is black
            player2 = new SmartAIPlayer("Smart AI", 'W'); // AI is white
        }
        else
        {
            player1 = new SmartAIPlayer("Smart AI", 'B'); // AI is black
            player2 = new HumanPlayer("Player", 'W');     // Human is white
        }
    }
    else // Invalid input, use default
    {
        std::cout << "Invalid mode selected. Defaulting to Human vs Human.\n"; // Warning message
        player1 = new HumanPlayer("Player 1", 'B');                            // Default to human black
        player2 = new HumanPlayer("Player 2", 'W');                            // Default to human white
    }

    currentTurn = 0; // Start from turn 0
    passCount = 0;   // Reset pass count
}

GameManager::~GameManager()
{
    delete player1; // Free memory
    delete player2; // Free memory
}

void GameManager::startGame()
{
    std::cout << "=== Go Game Start ===\n"; // Display game start
    bool running = true;                    // Game loop flag
    bool resigned = false;                  // Check if someone resigned

    while (running)
    {
        board.display();                                                     // Show board
        Player *currentPlayer = (currentTurn % 2 == 0) ? player1 : player2;  // Alternate turns
        Player *opponentPlayer = (currentTurn % 2 == 0) ? player2 : player1; // Other player

        int x, y;                      // Variables for the move
        currentPlayer->makeMove(x, y); // Get move from current player

        if (x == -99 && y == -99) // Check for resign
        {
            std::cout << currentPlayer->getName() << " resigned!\n";      // Show resign message
            std::cout << "Winner: " << opponentPlayer->getName() << "\n"; // Show winner
            resigned = true;                                              // Set flag
            break;                                                        // Exit game loop
        }

        if (x == -2 || y == -2) // Check for invalid input
        {
            std::cout << "Invalid input! Please try again.\n"; // Show error
            continue;                                          // Skip to next iteration
        }
        else if (x == -1 && y == -1) // Check for pass
        {
            std::cout << currentPlayer->getName() << " passed their turn.\n"; // Show pass
            passCount++;                                                      // Increment pass count
            currentTurn++;                                                    // Switch turns
        }
        else // Normal move
        {
            bool placeSuccess = board.placeStone(x, y, currentPlayer->getColor()); // Try placing a stone
            if (!placeSuccess)
            {
                continue; // If illegal move, retry
            }
            passCount = 0; // Reset pass count
            currentTurn++; // Next turn
        }

        // Calculate scores
        int blackTerritory = 0, whiteTerritory = 0;
        board.calculateTerritory(blackTerritory, whiteTerritory); // Count territory

        int blackCaptured = board.getCaptured('W'); // Stones captured by black
        int whiteCaptured = board.getCaptured('B'); // Stones captured by white

        double blackScore = blackTerritory + blackCaptured;       // Black's score
        double whiteScore = whiteTerritory + whiteCaptured + 6.5; // White's score with komi

        // Show score summary
        std::cout << "Current Score:\n";
        std::cout << player1->getName() << ": " << blackScore << " (Territory: " << blackTerritory << ", Captured: " << blackCaptured << ")\n";
        std::cout << player2->getName() << ": " << whiteScore << " (Territory: " << whiteTerritory << ", Captured: " << whiteCaptured << ", Komi: 6.5)\n\n";

        if (passCount >= 2) // If both players passed
        {
            std::cout << "Both players passed. Game over.\n"; // End game
            break;
        }
    }

    if (!resigned) // If game ended by pass, not resign
    {
        std::cout << "=== Final Score ===\n";

        // Recalculate final scores
        int blackTerritory = 0, whiteTerritory = 0;
        board.calculateTerritory(blackTerritory, whiteTerritory);

        int blackCaptured = board.getCaptured('W');
        int whiteCaptured = board.getCaptured('B');

        double blackScore = blackTerritory + blackCaptured;
        double whiteScore = whiteTerritory + whiteCaptured + 6.5;

        // Show final score
        std::cout << player1->getName() << ": " << blackScore << " (Territory: " << blackTerritory << ", Captured: " << blackCaptured << ")\n";
        std::cout << player2->getName() << ": " << whiteScore << " (Territory: " << whiteTerritory << ", Captured: " << whiteCaptured << ", Komi: 6.5)\n";

        if (blackScore > whiteScore) // Black wins
        {
            std::cout << "Winner: " << player1->getName() << "\n";
        }
        else if (whiteScore > blackScore) // White wins
        {
            std::cout << "Winner: " << player2->getName() << "\n";
        }
        else // Tie
        {
            std::cout << "Draw!\n";
        }
    }
}
