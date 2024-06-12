#include <stdio.h>
#include <ncurses.h>

#define COLS 80
#define ROWS 25
#define CELL '1'
#define VOID_ELEM '0'
#define INIT_SPEED 16

int init_matrix(char matrix[ROWS][COLS]);
int find_alive_neighbors(int x, int y, char matrix[ROWS][COLS]);

void move_speed(int *speed, char user_buffer);
void update_matrix(char matrix[ROWS][COLS]);
void print_matrix(char (*matrix)[COLS]);

int main() {
    char matrix[ROWS][COLS];
    int flaq;

    if (init_matrix(matrix)) {
        initscr();
        print_matrix(matrix);
        noecho();
        nodelay(stdscr, 1);
        curs_set(0);
        
        int speed = 200;
        char user_buffer = VOID_ELEM;

        while (user_buffer != 'q') {
            print_matrix(matrix);
            user_buffer = getch();
            move_speed(&speed, user_buffer);
            timeout(speed);
            update_matrix(matrix);
        }
        endwin();
    } else {
        flaq = 1;
    }

    return flaq;
}

int init_matrix(char matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        if (!fgets(matrix[i], COLS + 1, stdin)) {
            break;
        }
    }

    return (freopen("/dev/tty", "r", stdin) != NULL);
}

void update_matrix(char matrix[ROWS][COLS]) {
    char next_matrix[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int amount_alive_neighbors = find_alive_neighbors(j, i, matrix);
            
            if (matrix[i][j] == CELL) {
                if ((amount_alive_neighbors == 2) || (amount_alive_neighbors == 3)) {
                    next_matrix[i][j] = CELL;
                } else {
                    next_matrix[i][j] = VOID_ELEM;
                }
            } else {
                if (amount_alive_neighbors == 3) {
                    next_matrix[i][j] = CELL;
                } else {
                    next_matrix[i][j] = VOID_ELEM;
                }
            }
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix[i][j] = next_matrix[i][j];
        }
    }
}

int find_alive_neighbors(int x, int y, char matrix[ROWS][COLS]) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int lx = (x + i + COLS) % COLS;
            int ly = (y + j + ROWS) % ROWS;
            count += (matrix[lx][ly] == CELL);
        }
    }

    return count;
}

void print_matrix(char (*matrix)[COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            mvaddch(i, j, matrix[i][j]);
        }
    }
}

void move_speed(int *speed, char user_buffer) {
    switch (user_buffer) {
        case '+':
            *speed -= (*speed == INIT_SPEED) ? 0 : INIT_SPEED;
            break;
        case '-':
            *speed += INIT_SPEED;
            break;
        default:
            *speed += 0;
    }
}