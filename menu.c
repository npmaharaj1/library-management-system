#include <stdio.h>
#include <ncurses.h>

void displayOptions(char *options[3], const char *prompt, int selectedItemIndex) {
    printw("%s", prompt);
    for (int i = 0; i < 3; i++) {
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

int Run(int selectedItemIndex, int ch, char *options[3], const char *prompt) {
    do {
        clear();
        switch (ch) {
            case KEY_UP:
                selectedItemIndex--;
                break;
            case KEY_DOWN:
                selectedItemIndex++;
                break;
        }
        displayOptions(options, prompt, selectedItemIndex);
    } while ((ch = getch()) != '\n');
    return selectedItemIndex;
}

int main() {
    // Keyboard stuff
    initscr(); // ncurses terminal
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Enable keypad
    int ch; // keypad data

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
    const char *prompt = "Welcome, what would you like to do"; // Instructions for user
    char *options[3] = {"Play", "About", "Exit"};

    // displayOptions(options, prompt, selectedItemIndex);
    selectedItemIndex = Run(selectedItemIndex, ch, options, prompt);
    printw("\nPress any key to exit...\n");
    refresh();

    // Clean up ncurses
    endwin();
    return 0;
}

