#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_LINES 1000
#define MAX_LINE_LEN 1024
#define FILE_NAME "output.txt"

char* buffer[MAX_LINES];
int line_lengths[MAX_LINES];
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

void save_to_file(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("Error saving file");
        return;
    }
    for (int i = 0; i < line_count; i++) {
        fprintf(fp, "%s\n", buffer[i]);
    }
    fclose(fp);
    printf("\nFile saved to '%s'\n", filename);
}

void insert_char(char ch) {
    if (cursorY >= MAX_LINES) return;

    char* line = buffer[cursorY];
    int len = strlen(line);

    if (len >= MAX_LINE_LEN - 1) return;

    // Shift characters to the right
    for (int i = len; i >= cursorX; i--) {
        line[i + 1] = line[i];
    }

    line[cursorX] = ch;
    cursorX++;
}

void handleCommand(const char* command) {
    if (strcmp(command, "w") == 0) {
        save_to_file(FILE_NAME);
        running = 0;  // Exit after save
    } else if (strcmp(command, "q") == 0) {
        running = 0;
    }
}

void process_input() {
    int ch = _getch();

    if (ch == 0 || ch == 224) {
        // Arrow key
        ch = _getch();
        switch (ch) {
            case 75: if (cursorX > 0) cursorX--; break;             // Left
            case 77: if (cursorX < strlen(buffer[cursorY])) cursorX++; break;  // Right
            case 72: if (cursorY > 0) cursorY--; break;             // Up
            case 80: if (cursorY < line_count - 1) cursorY++; break; // Down
        }
    } else if (ch == 13) { // Enter key
        if (line_count < MAX_LINES - 1) {
            for (int i = line_count; i > cursorY + 1; i--) {
                buffer[i] = buffer[i - 1];
            }
            buffer[cursorY + 1] = (char*)malloc(MAX_LINE_LEN);
            buffer[cursorY + 1][0] = '\0';
            line_count++;
            cursorY++;
            cursorX = 0;
        }
    } else if (ch == 8) { // Backspace
        if (cursorX > 0) {
            char* line = buffer[cursorY];
            int len = strlen(line);
            for (int i = cursorX - 1; i < len; i++) {
                line[i] = line[i + 1];
            }
            cursorX--;
        }
    } else if (ch == ':') {
        char cmd[10] = {0};
        int i = 0;
        move_cursor(0, line_count + 1);
        printf(":");
        while (i < 9) {
            int c = _getch();
            if (c == 13) break; // Enter
            else if (c == 8 && i > 0) { // Backspace
                i--;
                printf("\b \b");
            } else if (c >= 32 && c <= 126) {
                cmd[i++] = (char)c;
                printf("%c", c);
            }
        }
        cmd[i] = '\0';
        handleCommand(cmd);
    } else if (ch >= 32 && ch <= 126) {
        insert_char((char)ch);
    }
}

int main() {
    for (int i = 0; i < MAX_LINES; i++) {
        buffer[i] = (char*)malloc(MAX_LINE_LEN);
        buffer[i][0] = '\0';
    }

    clear_screen();
    printf("Mini Text Editor — Use arrow keys, :w to save & exit, :q to quit\n");

    while (running) {
        draw_buffer();
        process_input();
    }

    // Cleanup
    for (int i = 0; i < MAX_LINES; i++) {
        free(buffer[i]);
    }

    return 0;
}
