#include "game_manager.h" // Include the GameManager class
#include <iostream>       // For input and output
#include <string>         // For using std::string

int main()
{
    while (true) // Loop to allow playing multiple games
    {
        GameManager game; // Create a new game manager (sets up the game)
        game.startGame(); // Start the game loop
        std::string command;                                    // Variable to store user's choice
        std::cout << "Play again? (type 'again' to restart): "; // Ask user if they want to play again
        std::cin >> command;                                    // Read user input

        if (command != "again") // If user does not type "again"
        {
            std::cout << "Thanks for playing!\n"; // Say goodbye
            break;                                // Exit the loop and end the program
        }
    }

    return 0; // Program ends
}
