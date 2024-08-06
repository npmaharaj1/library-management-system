#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "otherFunctions.h"

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
    int ch = 0; // Data to store keypad stuff
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

void optionOne(int selectedItemIndex, List* head) {
    clear();
    char search[100]; // Prompts users for input

    printw("\nEnter your search query: ");
    refresh();
    curs_set(1);
    echo();
    wgetnstr(stdscr, search, sizeof(search) - 1);
    curs_set(0);
    noecho();
    searchBooks(head, search);
    List* result = searchBooks(head, search);

    if (result == NULL) { // If no books found
        printw("No books found.\n");
    }
    // Loop through results list and display
    while (result != NULL) {
        printw("Title: %s\n", result->book->Title);
        printw("Author: %s\n", result->book->Author);
        printw("ID: %d\n", result->book->ID);
        printw("\n");
        result = result->next;
    }
    printw("\nPress any key to continue...");
    refresh();
    getch();
}

void optionTwo(int selectedItemIndex, List** head, const char* booklist) {
    clear();
    char *options[2] = {"Add Books", "Delete Books"};
    const char *prompt = "Home/Modify Book Data/\n";
    int optionsCount = sizeof(options) / sizeof(options[0]);
    selectedItemIndex = 0;
    selectedItemIndex = Run(optionsCount, selectedItemIndex, options, prompt);
    
    switch (selectedItemIndex) {
        case 0:
            addBook(head, booklist);
            break;
        case 1:
            deleteBook(head, booklist);
            break;
    }
}

void optionThree(int selectedItemIndex, List* head, const char* booklist) {
    clear();
    // TODO: Make a better list function
    List *current = head;
    printw("Book list:\n\n");
    refresh();
    while (current != NULL) {
        printw("ID: %d, Title: %s, Author: %s\n", current->book->ID, current->book->Title, current->book->Author);
        refresh();
        current = current->next;
    }

    printw("Press any key to continue...");
    refresh();
    getch();
}

void optionFour(int selectedItemIndex, List** head, const char* booklist) {
    clear();
    char *options[2] = {"Apply Changes", "Restore Changes"};
    const char *prompt = "Home/Save or Restore\n";
    int optionsCount = sizeof(options) / sizeof(options[0]);
    selectedItemIndex = 0;
    selectedItemIndex = Run(optionsCount, selectedItemIndex, options, prompt);

    switch (selectedItemIndex) {
        case 0:
            writeBooksToFile(*head, booklist);
            break;
        case 1:
            *head = readBooksFromFile(booklist);
            break;
        }
}

void exitFunction(List* head) {
    clear();
    freeBooks(head);
    endwin();
    curs_set(1);
}

void menuHome(int* selectedItemIndex, int optionsCount, char* options[optionsCount], const char *prompt, List** head, const char* booklist) {
    *selectedItemIndex = Run(optionsCount, *selectedItemIndex, options, prompt);
    switch (*selectedItemIndex) {
        case 0:
            optionOne(*selectedItemIndex, *head);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 1:
            optionTwo(*selectedItemIndex, head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 2:
            optionThree(*selectedItemIndex, *head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 3:
            optionFour(*selectedItemIndex, head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 4:
            exitFunction(*head);
            break;
    }
    refresh();
}