#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void display_board(char board[3][3]) {
    printf("+-------+-------+-------+\n");
    for (int row = 0; row < 3; row++) {
        printf("|       |       |       |\n");
        for (int col = 0; col < 3; col++) {
            printf("|   %c   ", board[row][col]);
        }
        printf("|\n|       |       |       |\n");
        printf("+-------+-------+-------+\n");
    }
}

void enter_move(char board[3][3]) {
    int move;
    while (true) {
        printf("Enter your move (1-9): ");
        if (scanf("%d", &move) == 1 && move >= 1 && move <= 9) {
            int row = (move - 1) / 3;
            int col = (move - 1) % 3;
            if (board[row][col] != 'O' && board[row][col] != 'X') {
                board[row][col] = 'O';
                break;
            } else {
                printf("Field already occupied - try again!\n");
            }
        } else {
            printf("Invalid input - try again!\n");
            while (getchar() != '\n'); // Clear input buffer
        }
    }
}

void make_list_of_free_fields(char board[3][3], int free_fields[9][2], int *count) {
    *count = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] != 'O' && board[row][col] != 'X') {
                free_fields[*count][0] = row;
                free_fields[*count][1] = col;
                (*count)++;
            }
        }
    }
}

bool victory_for(char board[3][3], char sgn) {
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == sgn && board[i][1] == sgn && board[i][2] == sgn) ||
            (board[0][i] == sgn && board[1][i] == sgn && board[2][i] == sgn)) {
            return true;
        }
    }
    if ((board[0][0] == sgn && board[1][1] == sgn && board[2][2] == sgn) ||
        (board[0][2] == sgn && board[1][1] == sgn && board[2][0] == sgn)) {
        return true;
    }
    return false;
}

void draw_move(char board[3][3]) {
    int free_fields[9][2], count;
    make_list_of_free_fields(board, free_fields, &count);

    // Check if the bot can win
    for (int i = 0; i < count; i++) {
        int row = free_fields[i][0];
        int col = free_fields[i][1];
        char original = board[row][col];
        board[row][col] = 'X';
        if (victory_for(board, 'X')) return;
        board[row][col] = original;
    }

    // Block the player's winning move
    for (int i = 0; i < count; i++) {
        int row = free_fields[i][0];
        int col = free_fields[i][1];
        char original = board[row][col];
        board[row][col] = 'O';
        if (victory_for(board, 'O')) {
            board[row][col] = 'X';
            return;
        }
        board[row][col] = original;
    }

    // Otherwise, choose a random free spot
    srand(time(NULL));
    int choice = rand() % count;
    int row = free_fields[choice][0];
    int col = free_fields[choice][1];
    board[row][col] = 'X';
}

int main() {
    char board[3][3];
    int index = 1;

    // Initialize the board
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            board[row][col] = '0' + index++;
        }
    }

    srand(time(NULL));
    bool human_turn = rand() % 2;

    // If the bot starts, make a single initial move
    if (!human_turn) {
        int free_fields[9][2], count;
        make_list_of_free_fields(board, free_fields, &count);
        int choice = rand() % count;
        int row = free_fields[choice][0];
        int col = free_fields[choice][1];
        board[row][col] = 'X';
        human_turn = true;
    }

    while (true) {
        display_board(board);
        if (human_turn) {
            enter_move(board);
            if (victory_for(board, 'O')) {
                display_board(board);
                printf("You won!\n");
                break;
            }
        } else {
            draw_move(board);
            if (victory_for(board, 'X')) {
                display_board(board);
                printf("I won!\n");
                break;
            }
        }

        // Check for tie
        int free_fields[9][2], count;
        make_list_of_free_fields(board, free_fields, &count);
        if (count == 0) {
            display_board(board);
            printf("It's a tie!\n");
            break;
        }

        human_turn = !human_turn;
    }

    return 0;
}
