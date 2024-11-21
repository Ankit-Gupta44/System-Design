#include <bits/stdc++.h>
using namespace std;

// Class to represent the Snake and Ladder board
class Board {
private:
    int size; // Number of cells on the board
    unordered_map<int, int> snakes; // Map for snakes (start -> end)
    unordered_map<int, int> ladders; // Map for ladders (start -> end)

public:
    Board(int size) : size(size) {}

    // Add a snake to the board
    void addSnake(int start, int end) {
        snakes[start] = end;
    }

    // Add a ladder to the board
    void addLadder(int start, int end) {
        ladders[start] = end;
    }

    // Get the final position after encountering a snake or ladder
    int getFinalPosition(int position) {
        if (snakes.count(position)) return snakes[position];
        if (ladders.count(position)) return ladders[position];
        return position;
    }

    // Get the size of the board
    int getSize() const {
        return size;
    }
};

// Class to represent a player
class Player {
private:
    string name;
    int position;

public:
    Player(string name) : name(name), position(0) {}

    string getName() const {
        return name;
    }

    int getPosition() const {
        return position;
    }

    void setPosition(int pos) {
        position = pos;
    }
};

// Class to represent the Snake and Ladder game
class SnakeAndLadder {
private:
    Board board;
    vector<Player> players;
    int currentPlayerIndex;

    // Simulate rolling a die (returns a number between 1 and 6)
    int rollDie() {
        return rand() % 6 + 1;
    }

public:
    SnakeAndLadder(int boardSize) : board(boardSize), currentPlayerIndex(0) {}

    // Add a snake to the board
    void addSnake(int start, int end) {
        board.addSnake(start, end);
    }

    // Add a ladder to the board
    void addLadder(int start, int end) {
        board.addLadder(start, end);
    }

    // Add a player to the game
    void addPlayer(string name) {
        players.push_back(Player(name));
    }

    // Play the game
    void play() {
        while (true) {
            Player& currentPlayer = players[currentPlayerIndex];
            cout << currentPlayer.getName() << "'s turn. Current position: " << currentPlayer.getPosition() << endl;

            // Roll the die
            int dieRoll = rollDie();
            cout << "Rolled a " << dieRoll << "!" << endl;

            // Move the player
            int newPosition = currentPlayer.getPosition() + dieRoll;
            if (newPosition > board.getSize()) {
                cout << currentPlayer.getName() << " rolled too high to finish this turn.\n";
            } else {
                newPosition = board.getFinalPosition(newPosition);
                cout << currentPlayer.getName() << " moved to position " << newPosition << endl;
                currentPlayer.setPosition(newPosition);

                // Check for a win condition
                if (newPosition == board.getSize()) {
                    cout << currentPlayer.getName() << " wins the game!\n";
                    break;
                }
            }

            // Move to the next player
            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            cout << "-----------------------------------\n";
        }
    }
};

// Driver code
int main() {
    // Initialize the game
    SnakeAndLadder game(100);

    // Add snakes
    game.addSnake(16, 6);
    game.addSnake(48, 26);
    game.addSnake(49, 11);
    game.addSnake(56, 53);
    game.addSnake(62, 19);
    game.addSnake(87, 24);
    game.addSnake(93, 73);
    game.addSnake(95, 75);
    game.addSnake(98, 78);

    // Add ladders
    game.addLadder(2, 38);
    game.addLadder(7, 14);
    game.addLadder(8, 31);
    game.addLadder(15, 26);
    game.addLadder(21, 42);
    game.addLadder(28, 84);
    game.addLadder(36, 44);
    game.addLadder(51, 67);
    game.addLadder(71, 91);
    game.addLadder(78, 98);

    // Add players
    game.addPlayer("Alice");
    game.addPlayer("Bob");

    // Play the game
    game.play();

    return 0;
}
