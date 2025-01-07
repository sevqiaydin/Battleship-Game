// SEVGÝ AYDIN
// 220101028

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

// Symbols used in the game board
#define WATER 'W'  // Water
#define SHIP 'S'   // Ship
#define HIT 'H'    // Hit
#define MISS 'M'   // Miss

// Boards
char playerBoard[SIZE][SIZE];
char computerBoard[SIZE][SIZE];
char playerGuessBoard[SIZE][SIZE];
char computerGuessBoard[SIZE][SIZE];

// Function prototypes
void initializeBoard(char board[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE]);
void placeShipRandom(char board[SIZE][SIZE], int size, char shipType);
int isValidPlacement(char board[SIZE][SIZE], int row, int col, int size, char orientation);
void playerTurn();
void computerTurn();
int checkWin(char board[SIZE][SIZE]);

int main() {
    // Seed for generating random numbers
    srand(time(NULL));

    // Initialize the boards
    initializeBoard(playerBoard);
    initializeBoard(computerBoard);
    initializeBoard(playerGuessBoard);
    initializeBoard(computerGuessBoard);

    // Player places ships
    printf("You are placing your ships.\n");
    placeShipRandom(playerBoard, 5, 'C'); // Carrier (5 cells)
    placeShipRandom(playerBoard, 4, 'B'); // Battleship (4 cells)
    placeShipRandom(playerBoard, 3, 'Z'); // Cruiser (3 cells)
    placeShipRandom(playerBoard, 3, 'S'); // Submarine (3 cells)
    placeShipRandom(playerBoard, 2, 'D'); // Destroyer (2 cells)

    // Computer places ships randomly
    printf("\nThe computer is placing its ships...\n");
    placeShipRandom(computerBoard, 5, 'C'); // Carrier (5 cells)
    placeShipRandom(computerBoard, 4, 'B'); // Battleship (4 cells)
    placeShipRandom(computerBoard, 3, 'Z'); // Cruiser (3 cells)
    placeShipRandom(computerBoard, 3, 'S'); // Submarine (3 cells)
    placeShipRandom(computerBoard, 2, 'D'); // Destroyer (2 cells)

    // Start the game
    while (1) {
        printBoard(playerGuessBoard);
        playerTurn();

        if (checkWin(computerBoard)) {
            printf("\nCongratulations! You sunk all of the computer's ships.\n");
            break;
        }

        printBoard(computerGuessBoard);
        computerTurn();

        if (checkWin(playerBoard)) {
            printf("\nThe computer won! All your ships have been sunk.\n");
            break;
        }
    }

    return 0;
}

// Function to initialize the board
void initializeBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = WATER;
        }
    }
}

// Function to print the board
void printBoard(char board[SIZE][SIZE]) {
    printf("\n  0 1 2 3 4 5 6 7 8 9\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to place a ship randomly
void placeShipRandom(char board[SIZE][SIZE], int size, char shipType) {
    int row, col, valid = 0;
    char orientation;

    // Keep placing the ship randomly until a valid position is found
    while (!valid) {
        // Randomly choose horizontal or vertical orientation
        orientation = (rand() % 2 == 0) ? 'H' : 'V';

        // Randomly choose the starting coordinates
        row = rand() % SIZE;
        col = rand() % SIZE;

        valid = isValidPlacement(board, row, col, size, orientation);
    }

    // Place the ship
    for (int i = 0; i < size; i++) {
        if (orientation == 'V') {
            board[row + i][col] = shipType;
        } else {
            board[row][col + i] = shipType;
        }
    }

    printf("The %c type ship was placed with %d cells.\n", shipType, size);
}

// Function to check if the placement is valid
int isValidPlacement(char board[SIZE][SIZE], int row, int col, int size, char orientation) {
    if (orientation == 'V') {
        if (row + size > SIZE) return 0; // Out of bounds
        for (int i = 0; i < size; i++) {
            if (board[row + i][col] != WATER) return 0; // Collision
        }
    } else {
        if (col + size > SIZE) return 0; // Out of bounds
        for (int i = 0; i < size; i++) {
            if (board[row][col + i] != WATER) return 0; // Collision
        }
    }
    return 1;
}

// Player's turn
void playerTurn() {
    int row, col;
    printf("\nEnter a coordinate (x y) (0-9): ");
    scanf("%d %d", &row, &col);

      if (row>9 || col>9){
        printf ("Please enter a value between 0-9");
      }
       else {

            if (computerBoard[row][col] != WATER) {
                printf("Hit! You sank the computer's ship.\n");
                computerGuessBoard[row][col] = HIT;
                computerBoard[row][col] = WATER;  // Ship sunk
    }      else {
                printf("Miss! You did not find the computer's ship.\n");
               computerGuessBoard[row][col] = MISS;
                }
             }
}
// Computer's turn
void computerTurn() {
    int row, col;
    do {
        row = rand() % SIZE;
        col = rand() % SIZE;
    } while (playerGuessBoard[row][col] != WATER);  // Try not to hit the same spot again

    printf("Computer's turn: %d %d\n", row, col);

    if (playerBoard[row][col] != WATER) {
        printf("The computer hit! Your ship is sunk.\n");
        playerGuessBoard[row][col] = HIT;
        playerBoard[row][col] = WATER;  // Ship sunk
    } else {
        printf("The computer missed.\n");
        playerGuessBoard[row][col] = MISS;
    }
}

// Function to check the winner
int checkWin(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != WATER) {
                return 0; // Ships are still present
            }
        }
    }
    return 1; // All ships are sunk
}

