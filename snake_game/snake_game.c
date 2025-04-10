#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int i, j, height = 20, width = 40;
int gameover, score;
int x, y, fruitX, fruitY, flag;
int tailX[100], tailY[100];
int tailLength;
int direction; // 0=no movement, 1=left, 2=down, 3=right, 4=up

// Function to display welcome screen
void welcomeScreen() {
    system("cls");
    printf("\n\n\n\n");
    printf("\t\t    SNAKE GAME\n");
    printf("\t\t    ==========\n\n");
    printf("\t    Controls:\n");
    printf("\t    Arrow Keys - Move Snake\n");
    printf("\t    W, A, S, D - Alternative Controls\n");
    printf("\t    X - Quit Game\n\n");
    printf("\t    Collect fruits (F) to grow and score points!\n\n");
    printf("\t    Press any key to start...");
    getch();
}

void setup() {
    srand(time(NULL)); // Initialize random seed
    gameover = 0;
    x = height / 2;
    y = width / 2;
    
    // Make sure fruit doesn't appear on the border
    fruitX = 1 + rand() % (height - 2);
    fruitY = 1 + rand() % (width - 2);
    
    score = 0;
    tailLength = 0;
    direction = 0; // No initial movement
}

void draw() {
    system("cls");
    
    // Game area
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                printf("#");
            else if (i == x && j == y)
                printf("O");
            else if (i == fruitX && j == fruitY)
                printf("F");
            else {
                int printTail = 0;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == i && tailY[k] == j) {
                        printf("o");
                        printTail = 1;
                    }
                }
                if (!printTail)
                    printf(" ");
            }
        }
        printf("\n");
    }
    
    // Status and controls help
    printf("Score: %d\n", score);
    printf("Controls: Arrow Keys or WASD to move, X to quit\n");
}

void input() {
    if (_kbhit()) {
        int key = _getch();
        
        // Check for extended keys (arrow keys)
        if (key == 224) {
            switch(_getch()) {
                case 75: // Left arrow
                    if (direction != 3) direction = 1; // Don't allow 180 degree turns
                    break;
                case 80: // Down arrow
                    if (direction != 4) direction = 2;
                    break;
                case 77: // Right arrow
                    if (direction != 1) direction = 3;
                    break;
                case 72: // Up arrow
                    if (direction != 2) direction = 4;
                    break;
            }
        } else {
            // Regular keys
            switch (key) {
                case 'a':
                case 'A':
                    if (direction != 3) direction = 1;
                    break;
                case 's':
                case 'S':
                    if (direction != 4) direction = 2;
                    break;
                case 'd':
                case 'D':
                    if (direction != 1) direction = 3;
                    break;
                case 'w':
                case 'W':
                    if (direction != 2) direction = 4;
                    break;
                case 'x':
                case 'X':
                    gameover = 1;
                    break;
            }
        }
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direction) {
        case 1: // Left
            y--;
            break;
        case 2: // Down
            x++;
            break;
        case 3: // Right
            y++;
            break;
        case 4: // Up
            x--;
            break;
        default:
            break;
    }

    // Wall collision
    if (x < 1 || x >= height - 1 || y < 1 || y >= width - 1)
        gameover = 1;

    // Self collision
    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameover = 1;
    }

    // Fruit collection
    if (x == fruitX && y == fruitY) {
        score += 10;
        // Make sure new fruit doesn't appear on the border or on the snake
        bool validPosition;
        do {
            validPosition = true;
            fruitX = 1 + rand() % (height - 2);
            fruitY = 1 + rand() % (width - 2);
            
            // Check if fruit is on snake's body
            if (fruitX == x && fruitY == y) {
                validPosition = false;
            }
            
            for (int i = 0; i < tailLength; i++) {
                if (fruitX == tailX[i] && fruitY == tailY[i]) {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);
        
        tailLength++;
    }
}

int main() {
    welcomeScreen();
    setup();
    
    while (!gameover) {
        draw();
        input();
        logic();
        
        // Game speed - decrease Sleep time for higher difficulty
        Sleep(100); 
    }
    
    system("cls");
    printf("\n\n\n\n");
    printf("\t\t    GAME OVER!\n");
    printf("\t\t    ==========\n\n");
    printf("\t\t  Final Score: %d\n\n", score);
    printf("\t\tPress any key to exit...");
    getch();
    
    return 0;
}
