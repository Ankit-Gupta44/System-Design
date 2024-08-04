#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <cmath>

enum Color { WHITE, BLACK };

// Base class for all pieces
class Piece {
public:
    Piece(Color color) : color(color) {}
    virtual ~Piece() {}
    virtual std::string getType() const = 0;
    virtual bool isValidMove(int startRow, int startCol, int endRow, int endCol) const = 0;
    Color getColor() const { return color; }

protected:
    Color color;
};

class Pawn : public Piece {
public:
    Pawn(Color color) : Piece(color) {}
    std::string getType() const override { return "Pawn"; }
    bool isValidMove(int startRow, int startCol, int endRow, int endCol) const override {
        int direction = (color == WHITE) ? 1 : -1;
        if (startCol == endCol) {
            if ((endRow - startRow) == direction) {
                return true;
            }
            if ((startRow == ((color == WHITE) ? 1 : 6)) && ((endRow - startRow) == 2 * direction)) {
                return true;
            }
        }
        if (std::abs(endCol - startCol) == 1 && (endRow - startRow) == direction) {
            return true;
        }
        return false;
    }
};

class Knight : public Piece {
public:
    Knight(Color color) : Piece(color) {}
    std::string getType() const override { return "Knight"; }
    bool isValidMove(int startRow, int startCol, int endRow, int endCol) const override {
        int rowDiff = std::abs(startRow - endRow);
        int colDiff = std::abs(startCol - endCol);
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
};

class Bishop : public Piece {
public:
    Bishop(Color color) : Piece(color) {}
    std::string getType() const override { return "Bishop"; }
    bool isValidMove(int startRow, int startCol, int endRow, int endCol) const override {
        return std::abs(startRow - endRow) == std::abs(startCol - endCol);
    }
};

class Rook : public Piece {
public:
    Rook(Color color) : Piece(color) {}
    std::string getType() const override { return "Rook"; }
    bool isValidMove(int startRow, int startCol, int endRow, int endCol) const override {
        return (startRow == endRow) || (startCol == endCol);
    }
};

class Queen : public Piece {
public:
    Queen(Color color) : Piece(color) {}
    std::string getType() const override { return "Queen"; }
    bool isValidMove(int startRow, int startCol, int endRow, int endCol) const override {
        return (startRow == endRow) || (startCol == endCol) || (std::abs(startRow - endRow) == std::abs(startCol - endCol));
    }
};

class King : public Piece {
public:
    King(Color color) : Piece(color) {}
    std::string getType() const override { return "King"; }
    bool isValidMove(int startRow, int startCol, int endRow, int endCol) const override {
        int rowDiff = std::abs(startRow - endRow);
        int colDiff = std::abs(startCol - endCol);
        return (rowDiff <= 1 && colDiff <= 1);
    }
};

class Board {
public:
    Board() : board(8, std::vector<Piece*>(8, nullptr)) {
        for (int i = 0; i < 8; ++i) {
            board[1][i] = new Pawn(WHITE);
            board[6][i] = new Pawn(BLACK);
        }
        board[0][0] = board[0][7] = new Rook(WHITE);
        board[7][0] = board[7][7] = new Rook(BLACK);
        board[0][1] = board[0][6] = new Knight(WHITE);
        board[7][1] = board[7][6] = new Knight(BLACK);
        board[0][2] = board[0][5] = new Bishop(WHITE);
        board[7][2] = board[7][5] = new Bishop(BLACK);
        board[0][3] = new Queen(WHITE);
        board[7][3] = new Queen(BLACK);
        board[0][4] = new King(WHITE);
        board[7][4] = new King(BLACK);
    }

    Piece* getPiece(int row, int col) const {
        if (row < 0 || row >= 8 || col < 0 || col >= 8) return nullptr;
        return board[row][col];
    }

    void setPiece(int row, int col, Piece* piece) {
        if (row >= 0 && row < 8 && col >= 0 && col < 8) {
            board[row][col] = piece;
        }
    }

    bool movePiece(int startRow, int startCol, int endRow, int endCol) {
        Piece* piece = getPiece(startRow, startCol);
        if (!piece || !piece->isValidMove(startRow, startCol, endRow, endCol)) {
            return false;
        }
        Piece* destPiece = getPiece(endRow, endCol);
        if (destPiece && destPiece->getColor() == piece->getColor()) {
            return false;
        }
        setPiece(endRow, endCol, piece);
        setPiece(startRow, startCol, nullptr);
        return true;
    }

private:
    std::vector<std::vector<Piece*>> board;
};

class Move {
public:
    Move(int startRow, int startCol, int endRow, int endCol)
        : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol) {}

    int getStartRow() const { return startRow; }
    int getStartCol() const { return startCol; }
    int getEndRow() const { return endRow; }
    int getEndCol() const { return endCol; }

private:
    int startRow, startCol, endRow, endCol;
};

class Game {
public:
    Game() : currentTurn(WHITE) {}

    void start() {
        while (true) {
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    Piece* piece = board.getPiece(row, col);
                    if (piece) {
                        std::cout << piece->getType()[0] << " ";
                    } else {
                        std::cout << ". ";
                    }
                }
                std::cout << std::endl;
            }

            int startRow, startCol, endRow, endCol;
            std::cout << "Enter move (startRow startCol endRow endCol): ";
            std::cin >> startRow >> startCol >> endRow >> endCol;

            Move move(startRow, startCol, endRow, endCol);
            if (makeMove(move)) {
                switchTurn();
            } else {
                std::cout << "Invalid move. Try again." << std::endl;
            }

            if (isCheckmate(currentTurn)) {
                std::cout << (currentTurn == WHITE ? "Black" : "White") << " wins!" << std::endl;
                break;
            }
        }
    }

    bool makeMove(const Move& move) {
        return board.movePiece(move.getStartRow(), move.getStartCol(), move.getEndRow(), move.getEndCol());
    }

    void switchTurn() {
        currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
    }

    bool isCheck(Color color) const {
         // to be implemented
        return false;
    }

    bool isCheckmate(Color color) const {
      // to be implemented
        return false;
    }

private:
    Board board;
    Color currentTurn;
};

int main() {
    Game game;
    game.start();
    return 0;
}
