#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
using namespace std;
//telling
//checking a git change 
//preprocessors  macros best way to define constant
#define BEGINNER 0
#define INTERMEDIATE 1
#define ADVANCED 2
#define MAXSIDE 25
#define MAXMINES 99
#define MOVESIZE 526 // (25 * 25 - 99)

//involved classes
class Cell;
class MineCell;
class EmptyCell;

// Base class
class Cell {
public:
    virtual bool isMine() const = 0;//to check mine
    virtual char getSymbol() const = 0;
    virtual ~Cell() {}//destructor 
};

// Derived class 
class MineCell : public Cell {//inherit public prop of parent
public:
    bool isMine() const override { return true; }//polymorphism overiding parent member functions
    char getSymbol() const override { return '*'; }
};

// Derived class 
class EmptyCell : public Cell {
public:
    bool isMine() const override { return false; }
    char getSymbol() const override { return '-'; }
};

class MinesweeperBoard {
private:
    int SIDE; // side length of the board
    int MINES; // number of mines on the board
    Cell* board[MAXSIDE][MAXSIDE]; // 2D array to hold cells
    bool revealed[MAXSIDE][MAXSIDE]; // to track which cells are revealed
    bool gameWon;
    bool gameLost;
    chrono::steady_clock::time_point startTime; //for introducing timer

public:
    MinesweeperBoard(int side, int mines) : SIDE(side), MINES(mines), gameWon(false), gameLost(false) {
        srand(time(NULL));
        // Initialize the board with EmptyCells
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                board[i][j] = new EmptyCell();
                revealed[i][j] = false;
            }
        }
        startTime = chrono::steady_clock::now(); // Start the timer in the constructor
    }
    ~MinesweeperBoard() {
  
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                delete board[i][j];
            }
        }
    }

    void placeMines() {
        int minesPlaced = 0;
        while (minesPlaced < MINES) {
            int x = rand() % SIDE;//builtin function to randomly place mines on board for rows
            int y = rand() % SIDE;// for columns
            if (!board[x][y]->isMine()) {
                delete board[x][y];
                board[x][y] = new MineCell();
                minesPlaced++;
            }
        }
    }

    void printBoard() const {
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                if (revealed[i][j]) {
                    if (board[i][j]->isMine()) {
                        cout << board[i][j]->getSymbol() << " ";
                    }
                    else {
                        int count = countAdjacentMines(i, j);
                        cout << count << " ";
                    }
                }
                else {
                    cout << "- ";
                }
            }
            cout << std::endl;
        }
    }

    bool isGameWon() const {
        return gameWon;//getter for game win
    }

    bool isGameLost() const {//getter for  game lose
        return gameLost;
    }

    void revealCell(int row, int col) {
        if (row >= 0 && row < SIDE && col >= 0 && col < SIDE && !revealed[row][col]) {
            revealed[row][col] = true;
            if (board[row][col]->isMine()) {
                gameLost = true;
                cout << "Game Over! You hit a mine." << endl;
            }
            else {
                //  win condition
                int revealedCount = 0;
                for (int i = 0; i < SIDE; i++) {
                    for (int j = 0; j < SIDE; j++) {
                        if (revealed[i][j]) {
                            revealedCount++;
                        }
                    }
                }
                if (revealedCount == (SIDE * SIDE - MINES)) {
                    gameWon = true;
                    auto endTime = chrono::steady_clock::now();
                    auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime);
                    std::cout << "Congratulations! You won the game in " << duration.count() << " seconds!" << endl;
                }
            }
        }
        else {
            cout << "Invalid cell." << endl;
        }
    }

    int countAdjacentMines(int row, int col) const {
        int count = 0;
        for (int i = row - 1; i <= row + 1; ++i) {
            for (int j = col - 1; j <= col + 1; ++j) {
                if (i >= 0 && i < SIDE && j >= 0 && j < SIDE && board[i][j]->isMine()) {
                    ++count;
                }
            }
        }
        return count;
    }
};

// Functions
void playMinesweeper(int SIDE, int MINES) {
    MinesweeperBoard board(SIDE, MINES);
    board.placeMines();
    board.printBoard(); // Print the initial board

    // Start the timer
    auto startTime = chrono::steady_clock::now();

    // Game loop
    while (!board.isGameWon() && !board.isGameLost()) {
        // Calculate and display elapsed time
        auto currentTime = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(currentTime - startTime);
        cout << "Time elapsed: " << duration.count() << " seconds" << endl;

        int row, col;
        cout << "Enter the row and column to reveal: ";
        cin >> row >> col;
        board.revealCell(row, col);
        board.printBoard(); // Print the updated board after revealing the cell
    }
}
//for dificulty
void chooseDifficultyLevel(int& SIDE, int& MINES) {
    int level;
    cout << "Enter the Difficulty Level\n";
    cout << "Press 0 for BEGINNER (9 * 9 Cells and 10 Mines)\n";
    cout << "Press 1 for INTERMEDIATE (16 * 16 Cells and 40 Mines)\n";
    cout << "Press 2 for ADVANCED (24 * 24 Cells and 99 Mines)\n";

    cin >> level;

    if (level == BEGINNER) {
        SIDE = 9;
        MINES = 10;
    }

    if (level == INTERMEDIATE) {
        SIDE = 16;
        MINES = 40;
    }

    if (level == ADVANCED) {
        SIDE = 24;
        MINES = 99;
    }
}

//driver
int main() {
    int SIDE, MINES;
    chooseDifficultyLevel(SIDE, MINES);
    playMinesweeper(SIDE, MINES);
    return 0;
}
