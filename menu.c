#include <stdio.h>
#include <ncurses.h>

void keyUpArrow() {
    printw("Up\n");
}

void keyDownArrow() {
    printw("Down\n");
}

void keyLeftArrow() {
    printw("Left\n");
}

void keyRightarrow() {
    printw("Right\n");
}

void getArrowKey(int ch) {
    getch();
    switch (ch) {
        case KEY_UP:
            keyUpArrow();
            break;
        case KEY_DOWN:
            keyDownArrow();
            break;
        case KEY_LEFT:
            keyLeftArrow();
            break;
        case KEY_RIGHT:
            keyRightarrow();
            break;
    }
}

void displayOptions(char *options[3], const char *prompt) {
    printw("%s", prompt);
    for (int i = 0; i < 3; i++) {
        char *currentOption = options[i];
        printw("\n<< %s >>", currentOption);
    }
    printw("\n");
}

int main() {
    // Keyboard stuff
    initscr(); // ncurses terminal
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Enable keypad
    int ch; // keypad data

    // Visual Stuff
    int selectedItemIndex = 0; // Selected menu item in index form
    const char *prompt = "Welcome, what would you like to do"; // Instructions for user
    char *options[3] = {"Play", "About", "Exit"};

    displayOptions(options, prompt);
    printw("Press any key to exit...\n");
    refresh();

    getArrowKey(ch);
    refresh();

    // Clean up ncurses
    endwin();
    return 0;
}

