#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void displayOptions(int optionsCount, char *options[optionsCount], const char *prompt, int selectedItemIndex) {
    printw("%s", prompt);
    for (int i = 0; i < optionsCount; i++) {
        char *currentOption = options[i];
        char prefix;
        if (selectedItemIndex == i) {
            prefix = '*';
            attron(COLOR_PAIR((2)));
        } else {
            prefix = ' ';
            attron(COLOR_PAIR((1)));
        }
        printw("\n%c << %s >>", prefix, currentOption);
    }
    printf("\n");
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
}

int Run(int optionsCount, int selectedItemIndex, char *options[optionsCount], const char *prompt) {
    int ch; // Data to store keypad stuff
    do {
        clear();
        switch (ch) {
            case KEY_UP:
                if (selectedItemIndex > 0) {
                    selectedItemIndex--;
                }
                break;
            case KEY_DOWN:
            if (selectedItemIndex < optionsCount - 1) {
                selectedItemIndex++;
            }
            break;
        }
        displayOptions(optionsCount, options, prompt, selectedItemIndex);
    } while ((ch = getch()) != '\n');
    return selectedItemIndex;
}

void functionOne(int selectedItemIndex) {
    clear();
    char *options[3] = {"Sub 1", "Sub 2", "Sub 3"};
    const char *prompt = "This is a test prompt\n";
    int optionsCount = sizeof(options) / sizeof(options[0]);
    Run(optionsCount, selectedItemIndex, options, prompt);
}

void functionTwo(int selectedItemIndex) {
    clear();
    printw("This will be function two\n");
}

void functionThree(int selectedItemIndex) {
    clear();
    printw("This will be function three\n");
}

void exitFunction() {
    // Clean up ncurses
    endwin();
}

int main() {
    // Keyboard stuff
    initscr(); // ncurses terminal
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Enable keypad

    // Initialize colours
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support colours\n"); // IMPLIMENT SKIP LATER
        return 1;
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    // Visual Stuff
    int selectedItemIndex = 0; // Selected menu item in index form
    const char *prompt = "Library Management System: Welcome, Admin\nSelect an option using the arrow keys on the keyboard.\n"; // Instructions for user
    char *options[4] = {"Function 1", "Function 2", "Function 3", "Exit"};
    int optionsCount = sizeof(options) / sizeof(options[0]);

    selectedItemIndex = Run(optionsCount, selectedItemIndex, options, prompt);
    switch (selectedItemIndex) {
        case 0:
            functionOne(selectedItemIndex);
            break;
        case 1:  printw("\nPress any key to exit...\n");
            functionTwo(selectedItemIndex);
            break;
        case 2:
            functionThree(selectedItemIndex);
            break;
        case 3:
            exitFunction();
            return 0;
            break;
    }
    refresh();
}

