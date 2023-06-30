#include <stdio.h>  // for printf and scanf
#include <stdbool.h> // for bool
#include <ctype.h>
#include <stdlib.h>   //for abs, labs, llabs

#define BOARD_SIZE 8
typedef enum { WHITE , BLACK } Color;
char board[BOARD_SIZE][BOARD_SIZE] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

bool isKingInCheck(char board[BOARD_SIZE][BOARD_SIZE], Color currentPlayer);
void printBoard();
bool isMoveValid(char board[BOARD_SIZE][BOARD_SIZE], char fromCol, char toCol,int fromRow, int toRow, Color currentPlayer);

int main(){

    bool gameOver = false;
    Color currentPlayer = WHITE;
    int checkcountW = 0;
    int checkcountB = 0;

    printf("\n\t\tWhite's turn!\n\n");

    while (!gameOver) {

        if (isKingInCheck(board, currentPlayer)) {
            if(currentPlayer == WHITE){
                printf("\n\t\tWhite's king is under check!\n\n");
                checkcountW++;
            }
            else{
                printf("\n\t\tBlack's king is under check!\n\n");
                checkcountB++;
            }
        }

        if(checkcountW == 2){
            gameOver = true;
            printf("\nBlack won!\n");
            break;
        }
        else if(checkcountB == 2){
            gameOver = true;
            printf("\nWhite won!\n");
            break;
        }

        printBoard();
        char fromCol, toCol;
        int fromRow, toRow;

        printf("Enter your move (e.g., A2 A4): ");
        scanf(" %c%d %c%d", &fromCol, &fromRow, &toCol, &toRow);

        fromRow = BOARD_SIZE - fromRow; // e.g. a2 => 6, a7 => 1
        toRow = BOARD_SIZE - toRow; // e.g. a4 => 4, a5 => 3
        fromCol -= 'a'; // Using ASCII system, e.g. a => 0, b => 1
        toCol -= 'a'; // Using ASCII system, e.g. a => 0. b => 1
        bool right = isMoveValid(board,fromCol, toCol, fromRow, toRow, currentPlayer); // right can be both true and false
        // for kings from here
        if(board[fromRow][fromCol] == 'K' && (toCol - fromCol) == 2 && fromRow == toRow || board[fromRow][fromCol] == 'k' && (toCol - fromCol) == 2 && fromRow == toRow){
            if(board[fromRow][fromCol + 1] == '.' && board[toRow][toCol] == '.'){
                board[toRow][toCol] = board[fromRow][fromCol];
                board[fromRow][fromCol + 1] = board[fromRow][fromCol + 3];
                board[fromRow][fromCol + 3] = '.';
                board[fromRow][fromCol] = '.';
            }
            currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
            if(currentPlayer == WHITE)
                printf("\n\t\tWhite's turn!\n\n");
            else{
                printf("\n\t\tBlack's turn!\n\n");
            }
        } // to here
        else if(right == false){
            printf("\n\nThe move is not valid!\n\n");
            if(currentPlayer == WHITE)
                printf("\n\t\tWhite's turn!\n\n");
            else{
                printf("\n\t\tBlack's turn!\n\n");
            } 
        }
        else{
            board[toRow][toCol] = board[fromRow][fromCol];  // bug with king 
            board[fromRow][fromCol] = '.';

            currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
            if(currentPlayer == WHITE)
                printf("\n\t\tWhite's turn!\n\n");
            else{
                printf("\n\t\tBlack's turn!\n\n");
            }
        }
    }
    return 0;
}



bool isMoveValid(char board[BOARD_SIZE][BOARD_SIZE], char fromCol, char toCol,int fromRow, int toRow, Color currentPlayer){
    if(fromCol < 0 || fromCol > 7 || toCol < 0 || toCol > 7 || fromRow > 7 || fromRow < 0 || fromRow > 7 || toRow < 0 || toRow > 7){
        return false; // checking the numbers and letters 
    }
    if(currentPlayer == WHITE && islower(board[fromRow][fromCol]) || currentPlayer == BLACK && isupper(board[fromRow][fromCol])){
        return false; // checking who will play and with which colors piece
    }
    if(currentPlayer == WHITE && board[toRow][toCol] > 'A' && board[toRow][toCol] < 'S' || currentPlayer == BLACK && board[toRow][toCol] > 'a' && board[toRow][toCol] < 's'){
        return false; // checking can he eat the piece
    }
    switch(board[fromRow][fromCol]){
        case '.': // does player play with empty piece ?
            return false;
            break;
        case 'k':
        case 'K':
            if(abs(toCol - fromCol) == 1 && abs(toRow - fromRow) == 1 || toCol == fromCol && abs(toRow - fromRow) == 1 || toRow == fromRow && abs(toCol - fromCol) == 1){
                return true; // checking all directions in 1 step
            }
            else{
                return false;
            }
            break;
        case 'N': // the knights
        case 'n':
            if(abs(toRow - fromRow) == 2 && abs(toCol - fromCol) == 1 || abs(toRow - fromRow) == 1 && abs(toCol - fromCol) == 2){
                return true; // the jump
            }
            else
                return false;
            break;
        case 'P': // white pawn
            if(fromRow == 6 && fromRow - toRow <= 2 && fromCol == toCol  && toRow < fromRow){ // When Pawn is on 2nd line
                return true;
            }
            else if((fromRow - toRow) == 1 && abs(toCol - fromCol) == 1 && toRow < fromRow){// When it is e.g. b4 c5
                return true;
            }
            else if((fromRow - toRow) == 1 && fromCol == toCol && toRow < fromRow){ // When it is empty cell
                if(board[toRow][toCol] != '.'){
                    return false;
                }
                else{
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 'p': // black pawn
            if(fromRow == 1 && toRow - fromRow <= 2 && fromCol == toCol  && toRow > fromRow){ // when Pawn is on 7th line
                return true;
            }
            else if((toRow - fromRow) == 1 && abs(toCol - fromCol) == 1 && toRow > fromRow){// when it is e.g. b5 c4
                return true;
            }
            else if((toRow - fromRow) == 1 && fromCol == toCol && toRow > fromRow){ // when it is empty cell
                if(board[toRow][toCol] != '.'){
                    return false;
                }
                else{
                    return true;
                }
            }
            else{
                return false;
            }
            break;
        case 'R': // the rocks
        case 'r':
            // move vertically or horizontally without jumping over other pieces
            if(toRow != fromRow && fromCol == toCol) {
                int step = (toRow > fromRow) ? +1 : -1;
                        
                for (int row = fromRow + step;(step < 0) ? row > toRow : row < toRow; row += step) {
                    if (board[row][toCol] != '.') {
                        return false;  // there's a piece blocking the path
                    }
                }   
                return true;
            }
            else if(toRow == fromRow && toCol != fromCol) {
                int step = (toCol > fromCol) ? 1 : -1;
                for (int col = fromCol + step;(step < 0) ? col > toCol : col < toCol; col += step) {
                    if (board[toRow][col] != '.') {
                        return false;  // there's a piece blocking the path
                    }
                }   
                return true;
            }
            else{
                return false;
            }
            break;
        case 'b': // the bishops
        case 'B':
            // checking diogonally without jumping over other pieces
            if(abs(fromCol - toCol) == abs(fromRow - toRow)){ 
                int stepCol = (fromCol > toCol) ? -1 : 1;
                int stepRow = (fromRow > toRow) ? -1 : 1;
                int col = fromCol + stepCol;
                int row  = fromRow + stepRow;
                while(row != toRow){
                    if(board[row][col] != '.'){
                        return false; // there's a piece blocking the path
                    }
                    col += stepCol;
                    row += stepRow;
                }
                return true;
            }
            else{
                return false;
            }
            break;
        case 'q': // the queens
        case 'Q':
            // move vertically or horizontally without jumping over other pieces
            if(toRow != fromRow && fromCol == toCol) {
                int step = (toRow > fromRow) ? +1 : -1;  
                for (int row = fromRow + step;(step < 0) ? row > toRow : row < toRow; row += step) {
                    if (board[row][toCol] != '.') {
                        return false;  // there's a piece blocking the path
                    }
                }   
                return true;
            }
            else if(toRow == fromRow && toCol != fromCol) {
                int step = (toCol > fromCol) ? 1 : -1;
                for (int col = fromCol + step;(step < 0) ? col > toCol : col < toCol; col += step) {
                    if (board[toRow][col] != '.') {
                        return false;  // there's a piece blocking the path
                    }
                }   
                return true;
            }
            else if(abs(fromCol - toCol) == abs(fromRow - toRow)){ // move diagonally
                int stepCol = (fromCol > toCol) ? -1 : 1;
                int stepRow = (fromRow > toRow) ? -1 : 1;
                int col = fromCol + stepCol;
                int row  = fromRow + stepRow;
                while(row != toRow){
                    if(board[row][col] != '.'){
                        return false; // there's a piece blocking the path
                    }
                    col += stepCol;
                    row += stepRow;
                }
                return true;
            }
            else{
                return false;
            }
            break;
    }
}

void printBoard(){
    printf("    A  B  C  D  E  F  G  H\n");
    printf("   ------------------------\n");
    for(int row = 0; row < BOARD_SIZE; row++){
        printf("%d ", BOARD_SIZE - row);
        printf("|");
        for(int col = 0; col < BOARD_SIZE; col++){
            printf(" %c ", board[row][col]);
        }
        printf("|");
        printf(" %d", BOARD_SIZE - row);
        printf("\n");
    }
    printf("   ------------------------\n");
    printf("    A  B  C  D  E  F  G  H\n");

}

bool isKingInCheck(char board[BOARD_SIZE][BOARD_SIZE], Color currentPlayer) {
    int kingRow = 0;
    int kingCol = 0;

    char king = (currentPlayer == WHITE) ? 'K' : 'k';
    Color opponent = (currentPlayer == WHITE) ? BLACK : WHITE;

    // Find the position of the king
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == king) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
    }

    // Check if any opponent piece can attack the king
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (currentPlayer == WHITE && islower(board[row][col]) || currentPlayer == BLACK && isupper(board[row][col])){
                if(isMoveValid(board, col, kingCol, row, kingRow, opponent))
                    return true;
            }
        }
    }

    return false;
}