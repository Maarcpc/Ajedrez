#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib> // Para exit()

using namespace std;

const int SIZE = 8;
const char EMPTY = '*';

class ChessBoard {
private:
    vector<vector<char>> board;

    void initializePieces() {
        // Peones
        for (int i = 0; i < SIZE; i++) {
            board[1][i] = 'p';
            board[6][i] = 'P';
        }

        // Piezas negras (minúsculas)
        board[0][0] = board[0][7] = 'r';
        board[0][1] = board[0][6] = 'n';
        board[0][2] = board[0][5] = 'b';
        board[0][3] = 'q';
        board[0][4] = 'k';

        // Piezas blancas (mayúsculas)
        board[7][0] = board[7][7] = 'R';
        board[7][1] = board[7][6] = 'N';
        board[7][2] = board[7][5] = 'B';
        board[7][3] = 'Q';
        board[7][4] = 'K';
    }

    bool isValidPosition(int x, int y) const {
        return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
    }

    bool isPawnMoveValid(int fromX, int fromY, int toX, int toY, bool isWhite) const {
        int direction = isWhite ? -1 : 1;

        // Movimiento hacia adelante
        if (fromY == toY && board[toX][toY] == EMPTY) {
            // Movimiento normal (1 casilla)
            if (toX == fromX + direction) return true;
            // Movimiento inicial (2 casillas)
            if ((fromX == 1 && !isWhite) || (fromX == 6 && isWhite)) {
                if (toX == fromX + 2 * direction && board[fromX + direction][fromY] == EMPTY) {
                    return true;
                }
            }
        }
        // Captura
        else if (abs(toY - fromY) == 1 && toX == fromX + direction) {
            if (board[toX][toY] != EMPTY && isupper(board[toX][toY]) != isWhite) {
                return true;
            }
        }
        return false;
    }

    bool isRookMoveValid(int fromX, int fromY, int toX, int toY) const {
        // Misma fila o misma columna
        if (fromX != toX && fromY != toY) return false;

        // Verificar camino despejado
        if (fromX == toX) { // Movimiento horizontal
            int step = (fromY < toY) ? 1 : -1;
            for (int y = fromY + step; y != toY; y += step) {
                if (board[fromX][y] != EMPTY) return false;
            }
        }
        else { // Movimiento vertical
            int step = (fromX < toX) ? 1 : -1;
            for (int x = fromX + step; x != toX; x += step) {
                if (board[x][fromY] != EMPTY) return false;
            }
        }
        return true;
    }

public:
    ChessBoard() : board(SIZE, vector<char>(SIZE, EMPTY)) {
        initializePieces();
    }

    void display() const {
        cout << "  a b c d e f g h" << endl;
        for (int i = 0; i < SIZE; i++) {
            cout << SIZE - i << " ";
            for (int j = 0; j < SIZE; j++) {
                cout << board[i][j] << " ";
            }
            cout << SIZE - i << endl;
        }
        cout << "  a b c d e f g h" << endl;
    }

    bool movePiece(int fromX, int fromY, int toX, int toY, bool isWhiteTurn) {
        if (!isValidPosition(fromX, fromY) || !isValidPosition(toX, toY)) {
            return false;
        }

        char piece = board[fromX][fromY];
        if (piece == EMPTY) return false;
        if (isupper(piece) != isWhiteTurn) return false;

        // Validación de movimiento según tipo de pieza
        switch (tolower(piece)) {
        case 'p':
            if (!isPawnMoveValid(fromX, fromY, toX, toY, isWhiteTurn)) return false;
            break;
        case 'r':
            if (!isRookMoveValid(fromX, fromY, toX, toY)) return false;
            break;
            // Implementar otras piezas aquí...
        default:
            return false;
        }

        // Verificar jaque mate (rey capturado)
        if (tolower(board[toX][toY]) == 'k') {
            board[toX][toY] = piece;
            board[fromX][fromY] = EMPTY;
            display();
            cout << (isWhiteTurn ? "Blancas" : "Negras") << " ganan por jaque mate!" << endl;
            exit(0);
        }

        // Promoción de peón
        if (tolower(piece) == 'p' && (toX == 0 || toX == 7)) {
            piece = isWhiteTurn ? 'Q' : 'q';
        }

        // Realizar movimiento
        board[toX][toY] = piece;
        board[fromX][fromY] = EMPTY;
        return true;
    }
};

int main() {
    ChessBoard game;
    bool whiteTurn = true;

    cout << "=== AJEDREZ SIMPLIFICADO ===" << endl;
    cout << "Instrucciones: ingresa movimientos como 'e2 e4'" << endl;
    cout << "Las piezas son: R=Torre, N=Caballo, B=Alfil, Q=Reina, K=Rey, P=Peon" << endl;

    while (true) {
        game.display();
        cout << "Turno de las " << (whiteTurn ? "blancas" : "negras") << ": ";

        string from, to;
        cin >> from >> to;

        if (from.length() != 2 || to.length() != 2) {
            cout << "Formato incorrecto. Ejemplo: e2 e4" << endl;
            continue;
        }

        int fromX = SIZE - (from[1] - '0');
        int fromY = tolower(from[0]) - 'a';
        int toX = SIZE - (to[1] - '0');
        int toY = tolower(to[0]) - 'a';

        if (!game.movePiece(fromX, fromY, toX, toY, whiteTurn)) {
            cout << "Movimiento no válido. Intenta nuevamente." << endl;
        }
        else {
            whiteTurn = !whiteTurn;
        }
    }

    return 0;
}