#define _WIN32_WINNT 0x0500  // Must be before windows.h

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define HEIGHT 20
#define WIDTH 60

enum Direction {
  UP, 
  DOWN,
  LEFT,
  RIGHT,
  STOP
};

enum Direction dir;
int score;
int fruit_x, fruit_y;
int head_x, head_y;
int tail_length;
int tail_x[100];
int tail_y[100];

void setup();
void clear_screen();
void draw();
void input();
void game_play();

void set_terminal_attributes();
void reset_terminal_attributes();
int input_available();

int main() {
  srand(time(NULL));
  set_terminal_attributes();
  setup();

  while (1) {
    draw();
    input();
    game_play();

    int sleep_time = 100 - (score / 2);  // Faster as score increases
    if (sleep_time < 30) sleep_time = 30;  // Min delay
    Sleep(sleep_time);
  }
}

void input() {
  if (input_available()) {
    char ch = getch();
    switch (ch)
    {
    case 'a':
    case 'A':
      if (dir != RIGHT) dir = LEFT;
      break;
    case 's':
    case 'S':
      if (dir != UP) dir = DOWN;
      break;
    case 'd':
    case 'D':
      if (dir != LEFT) dir = RIGHT;
      break;
    case 'w':
    case 'W':
      if (dir != DOWN) dir = UP;
      break;
    case 'x':
    case 'X':
      exit(0);
      break;
    }
  }
}

void game_play() {
  // Move tail
  for (int i = tail_length - 1; i > 0; i--) {
    tail_x[i] = tail_x[i - 1];
    tail_y[i] = tail_y[i - 1];
  }
  tail_x[0] = head_x;
  tail_y[0] = head_y;

  // Move head
  switch (dir)
  {
  case UP:
    head_y--;
    break;
  case DOWN:
    head_y++;
    break;
  case LEFT:
    head_x--;
    break;
  case RIGHT:
    head_x++;
    break;
  case STOP:
    // no movement
    break;
  }

  // Wrap around screen edges
  if (head_x < 0) head_x = WIDTH - 1;
  if (head_x >= WIDTH) head_x = 0;
  if (head_y < 0) head_y = HEIGHT - 1;
  if (head_y >= HEIGHT) head_y = 0;

  // Check tail collision
  for (int i = 0; i < tail_length; i++) {
    if (tail_x[i] == head_x && tail_y[i] == head_y) {
      printf("\nYou have hit your tail. GAME OVER !!!\n");
      reset_terminal_attributes();
      exit(0);
    }
  }

  // Check if fruit eaten
  if (head_x == fruit_x && head_y == fruit_y) {
    score += 10;
    tail_length++;
    fruit_x = rand() % WIDTH;
    fruit_y = rand() % HEIGHT;
  }
}

int input_available() {
  return _kbhit();
}

void setup() {
  head_x = WIDTH / 2;
  head_y = HEIGHT / 2;
  fruit_x = rand() % WIDTH;
  fruit_y = rand() % HEIGHT;
  dir = RIGHT;  // Start moving right immediately
  score = 0;
  tail_length = 0;
}

void draw() {
  clear_screen();

  printf("\t\tWelcome to The Snake Game!!!\n");

  // Top border
  for (int i = 0; i < WIDTH + 2; i++) printf("#");
  printf("\n");

  for (int i = 0; i < HEIGHT; i++) {
    printf("#");
    for (int j = 0; j < WIDTH; j++) {
      if (i == head_y && j == head_x) {
        printf("O");
      } else if (i == fruit_y && j == fruit_x) {
        printf("F");
      } else {
        int printed = 0;
        for (int k = 0; k < tail_length; k++) {
          if (tail_x[k] == j && tail_y[k] == i) {
            printf("o");
            printed = 1;
            break;
          }
        }
        if (!printed) printf(" ");
      }
    }
    printf("#\n");
  }

  // Bottom border
  for (int i = 0; i < WIDTH + 2; i++) printf("#");
  printf("\n");

  printf("Score: %d\n", score);

  fflush(stdout);
}

static DWORD old_mode;

void reset_terminal_attributes() {
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), old_mode);

  // Show cursor again
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(hOut, &cursorInfo);
  cursorInfo.bVisible = TRUE;
  SetConsoleCursorInfo(hOut, &cursorInfo);
}

void set_terminal_attributes() {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;

  GetConsoleMode(hStdin, &old_mode);
  atexit(reset_terminal_attributes);

  mode = old_mode;
  mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
  SetConsoleMode(hStdin, mode);

  // Hide blinking cursor
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(hOut, &cursorInfo);
  cursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(hOut, &cursorInfo);
}

void clear_screen() {
  COORD topLeft = {0, 0};
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(console, topLeft);
}
