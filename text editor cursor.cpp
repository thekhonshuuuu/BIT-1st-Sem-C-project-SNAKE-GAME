#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX_LINES 1000
#define MAX_LINE_LEN 1024
#define FILE_NAME "output.txt"

char* buffer[MAX_LINES];
int line_count = 0;
int cursorX = 0, cursorY = 0;

void gotoxy(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void render() {
    system("cls");
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", buffer[i]);
    }
    gotoxy(cursorX, cursorY);
}

void save_to_file(const char* filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error saving file");
        return;
    }
    for (int i = 0; i < line_count; i++) {
        fprintf(fp, "%s\n", buffer[i]);
    }
    fclose(fp);
    printf("File saved to '%s'\n", filename);
}

void insert_char(int ch) {
    if (cursorY >= line_count) {
        for (int i = line_count; i <= cursorY; i++) {
            buffer[i] = (char*)malloc(MAX_LINE_LEN);
            buffer[i][0] = '\0';
        }
        line_count = cursorY + 1;
    }
    int len = strlen(buffer[cursorY]);
    if (len < MAX_LINE_LEN - 1) {
        for (int i = len; i >= cursorX; i--) {
            buffer[cursorY][i+1] = buffer[cursorY][i];
        }
        buffer[cursorY][cursorX] = ch;
        cursorX++;
    }
}

void handle_input() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {  // Arrow key or function key
        int arrow = _getch();
        switch (arrow) {
            case 72: // Up
                if (cursorY > 0) cursorY--;
                if (cursorX > strlen(buffer[cursorY])) cursorX = strlen(buffer[cursorY]);
                break;
            case 80: // Down
                if (cursorY < line_count - 1) cursorY++;
                if (cursorX > strlen(buffer[cursorY])) cursorX = strlen(buffer[cursorY]);
                break;
            case 75: // Left
                if (cursorX > 0) cursorX--;
                break;
            case 77: // Right
                if (cursorX < strlen(buffer[cursorY])) cursorX++;
                break;
        }
    } else if (ch == 13) { // Enter
        if (line_count < MAX_LINES - 1) {
            for (int i = line_count; i > cursorY; i--) {
                buffer[i] = buffer[i-1];
            }
            buffer[cursorY+1] = (char*)malloc(MAX_LINE_LEN);
            strcpy(buffer[cursorY+1], &buffer[cursorY][cursorX]);
            buffer[cursorY][cursorX] = '\0';
            cursorY++;
            cursorX = 0;
            line_count++;
        }
    } else if (ch == 8) { // Backspace
        if (cursorX > 0) {
            int len = strlen(buffer[cursorY]);
            for (int i = cursorX - 1; i < len; i++) {
                buffer[cursorY][i] = buffer[cursorY][i+1];
            }
            cursorX--;
        }
    } else if (ch == 27) { // ESC
        printf("\nExit without saving? (y/n): ");
        char confirm = _getch();
        if (confirm == 'y' || confirm == 'Y') {
            exit(0);
        }
    } else if (ch == ':') {
        char command[10];
        int pos = 0;
        printf(":");
        while ((ch = _getch()) != 13 && pos < 9) {
            if (ch == 8 && pos > 0) {
                pos--;
                printf("\b \b");
            } else {
                command[pos++] = ch;
                putchar(ch);
            }
        }
        command[pos] = '\0';

        if (strcmp(command, "w") == 0) {
            save_to_file(FILE_NAME);
        } else if (strcmp(command, "q") == 0) {
            exit(0);
        }
    } else {
        insert_char(ch);
    }
}

int main() {
    for (int i = 0; i < MAX_LINES; i++) {
        buffer[i] = (char*)malloc(MAX_LINE_LEN);
        buffer[i][0] = '\0';
    }

    printf("Mini Text Editor (Arrow keys to move, :w to save, :q to quit)\n");

    while (1) {
        render();
        handle_input();
    }

    for (int i = 0; i < MAX_LINES; i++) {
        free(buffer[i]);
    }

    return 0;
}
