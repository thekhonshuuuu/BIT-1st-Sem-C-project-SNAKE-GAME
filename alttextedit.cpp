#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_LINES 1000
#define MAX_LINE_LEN 1024
#define FILE_NAME "output.txt"

char* buffer[MAX_LINES];
int line_count = 1;
int cursorX = 0, cursorY = 0;
int running = 1;

void clear_screen() {
    COORD topLeft = {0, 0};
    DWORD written;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacter(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, screen.wAttributes, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void move_cursor(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw_buffer() {
    clear_screen();
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", buffer[i]);
    }
    move_cursor(cursorX, cursorY);
}

void save_to_file() {
    FILE* fp = fopen(FILE_NAME, "w");
    if (!fp) return;
    for (int i = 0; i < line_count; i++) {
        fprintf(fp, "%s\n", buffer[i]);
    }
    fclose(fp);
}

void insert_char(char ch) {
    char* line = buffer[cursorY];
    int len = strlen(line);
    if (len < MAX_LINE_LEN - 1) {
        memmove(&line[cursorX + 1], &line[cursorX], len - cursorX + 1);
        line[cursorX++] = ch;
    }
}

void handle_command() {
    char cmd[10] = {0};
    int i = 0;
    move_cursor(0, line_count + 1);
    printf(":");
    while (i < 9) {
        int c = _getch();
        if (c == 13) break;
        if (c == 8 && i > 0) {
            i--; printf("\b \b");
        } else if (c >= 32 && c <= 126) {
            cmd[i++] = c;
            printf("%c", c);
        }
    }
    cmd[i] = '\0';
    if (strcmp(cmd, "w") == 0) save_to_file(), running = 0;
    else if (strcmp(cmd, "q") == 0) running = 0;
}

void process_input() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        switch (_getch()) {
            case 75: if (cursorX > 0) cursorX--; break;
            case 77: if (cursorX < strlen(buffer[cursorY])) cursorX++; break;
            case 72: if (cursorY > 0) cursorY--; break;
            case 80: if (cursorY < line_count - 1) cursorY++; break;
        }
    } else if (ch == 13 && line_count < MAX_LINES - 1) {
        for (int i = line_count; i > cursorY + 1; i--) buffer[i] = buffer[i - 1];
        buffer[++cursorY] = calloc(MAX_LINE_LEN, 1);
        line_count++;
        cursorX = 0;
    } else if (ch == 8 && cursorX > 0) {
        char* line = buffer[cursorY];
        memmove(&line[cursorX - 1], &line[cursorX], strlen(line) - cursorX + 1);
        cursorX--;
    } else if (ch == ':') {
        handle_command();
    } else if (ch >= 32 && ch <= 126) {
        insert_char(ch);
    }
}

int main() {
    for (int i = 0; i < MAX_LINES; i++) {
        buffer[i] = calloc(MAX_LINE_LEN, 1);
    }

    clear_screen();
    printf("Mini Text Editor — Arrows to move | :w to save & quit | :q to quit\n");

    while (running) {
        draw_buffer();
        process_input();
    }

    for (int i = 0; i < MAX_LINES; i++) free(buffer[i]);
    return 0;
}
