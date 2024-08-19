#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    List* result = searchBooks(head, search);

    if (result == NULL) { // If no books found
        printw("No books found.\n");
    } else {
        int pageSize = 5; // Number of results per page
        int pageCount = 0; // Number of pages
        int resultCount = 0; // Number of results
        List* current = result;

        // Count the number of results
        while (current != NULL) {
            resultCount++;
            current = current->next;
        }

        // Calculate the number of pages
        if (resultCount % pageSize == 0) {
            pageCount = resultCount / pageSize;
        } else {
            pageCount = resultCount / pageSize + 1;
        }

        // Display the results page by page
        int currentPage = 1;
        int startIndex = 0;
        int endIndex = pageSize - 1;
        int ch = 0; // Data to store keypad stuff

        do {
            clear();
            printw("Search Results (%d/%d):\n\n", currentPage, pageCount);

            // Loop through the results and display the current page
            int i = 0;
            current = result;
            while (current != NULL && i < endIndex + 1) {
                if (i >= startIndex) {
                    printw("Title: %s\n", current->book->Title);
                    printw("Author: %s\n", current->book->Author);
                    printw("ID: %d\n", current->book->ID);
                    printw("\n");
                }
                current = current->next;
                i++;
            }

            // Display navigation options
            printw("Press 'n' for next page, 'p' for previous page, or any other key to exit...");
            refresh();

            // Get user input
            ch = getch();

            // Update the start and end index based on user input
            if (ch == 'n' && endIndex < resultCount - 1) {
                startIndex += pageSize;
                endIndex += pageSize;
                currentPage++;
            } else if (ch == 'p' && startIndex > 0) {
                startIndex -= pageSize;
                endIndex -= pageSize;
                currentPage--;
            }
        } while (ch == 'n' || ch == 'p');
    }
}

void optionTwo(int selectedItemIndex, List** head, const char* booklist) {
    clear();
    char *options[3] = {"Add Books", "Delete Books", "Back"};
    const char *prompt = "Home/Modify Book Data/\n";
    int optionsCount = sizeof(options) / sizeof(options[0]);
    selectedItemIndex = 0;
    selectedItemIndex = Run(optionsCount, selectedItemIndex, options, prompt);

    switch (selectedItemIndex) {
        case 0:
            addBook(head, booklist, 0);
            break;
        case 1:
            deleteBook(head, booklist);
            break;
        case 2: //back button
            endwin();
            return;
    }
    optionTwo(selectedItemIndex, head, booklist);
}

void optionThree(int selectedItemIndex, List* head, const char* booklist) {

    clear();
    char *options[5] = {"Loaned Items", "Unloaned Items", "Overdue Items", "All Items", "Back"};
    const char *prompt = "Home/List Books\n";
    int optionsCount = sizeof(options) / sizeof(options[0]);
    List *current = head;
    selectedItemIndex = 0;
    selectedItemIndex = Run(optionsCount, selectedItemIndex, options, prompt);
    switch (selectedItemIndex) {
        case 0: // List all loaned Items
            current = head;
            clear();
            printw("Book list:\n\n");
            refresh();
            while (current != NULL) {
                if (strlen(current->book->borrowedTo.name) != 0) {
                    printw(">>> ID: %d, Title: %s, Author: %s, Borrowed to: %s, Due: %d/%d/%d\n", 
                        current->book->ID, current->book->Title, current->book->Author, 
                        current->book->borrowedTo.name, current->book->dueDate.day, 
                        current->book->dueDate.month, current->book->dueDate.year);
                }
                refresh();
                current = current->next;
            }
            printw("Press any key to continue...");
            refresh();
            getch();
            break;
        case 1:
            current = head;
            clear();
            printw("Book list:\n\n");
            refresh();
            while (current != NULL) {
                if (strlen(current->book->borrowedTo.name) == 0) {
                    printw(">>> ID: %d, Title: %s, Author: %s, Borrowed to: %s, Due: %d/%d/%d\n", 
                        current->book->ID, current->book->Title, current->book->Author, 
                        current->book->borrowedTo.name, current->book->dueDate.day, 
                        current->book->dueDate.month, current->book->dueDate.year);
                }
                refresh();
                current = current->next;
            }
            printw("Press any key to continue...");
            refresh();
            getch();
            break;
        case 2:
            break;
        case 3:
            current = head;
            clear();
            // TODO: Make a better list function
            printw("Book list:\n\n");
            refresh();
            while (current != NULL) {
                // Only displaying borrowing data if applicable.
                if (strlen(current->book->borrowedTo.name) != 0) {
                    printw(">>> ID: %d, Title: %s, Author: %s, Borrowed to: %s, Due: %d/%d/%d\n", 
                        current->book->ID, current->book->Title, current->book->Author, 
                        current->book->borrowedTo.name, current->book->dueDate.day, 
                        current->book->dueDate.month, current->book->dueDate.year);
                } else {
                    printw(">>> ID: %d, Title: %s, Author: %s\n", current->book->ID, current->book->Title, current->book->Author);
                }
                refresh();
                current = current->next;
            }

            printw("Press any key to continue...");
            refresh();
            getch();
            break;
        case 4:
            endwin();
            break;
    }
}

void optionFour(int selectedItemIndex, List* head, const char* booklist) {
    clear();
    char input[100]; 

    printw("\nEnter the book title: ");
    refresh();
    curs_set(1);
    echo();
    wgetnstr(stdscr, input, sizeof(input) - 1); // Prompts users for input (book title)
    curs_set(0);
    noecho();
    loanBook(head, input);
    refresh();

    printw("Press any key to continue...");
    refresh();
    getch();
}

void optionFive(int selectedItemIndex, List** head, const char* booklist) {
    clear();
    char *options[3] = {"Apply Changes", "Restore Changes", "Back"};
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
        case 2:
            endwin();
            break;
        }
}

void exitFunction(List* head, const char* booklist) {
    clear();

    // Ask to apply changes
    char applyChanges; // Store yes/no
    printw("\nApply Changes? (y/N): ");
    refresh();
    echo();
    curs_set(1);
    scanw("%c", &applyChanges);
    noecho();
    curs_set(0);
    if (applyChanges == 'Y' || applyChanges == 'y') {
        writeBooksToFile(head, booklist);
    }

    // Free memory
    freeBooks(head);
    
    // Fix terminal
    echo();
    curs_set(1);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    use_default_colors();
    attron(COLOR_PAIR(0));


    endwin();
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
            // Add options for listing by currently loaned items, overdue items, all, not loaned.
        case 3:
            optionFour(*selectedItemIndex, *head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
            // Add options for time enter due date or how many days in the future.
        case 4:
            optionFive(*selectedItemIndex, head, booklist);
            menuHome(selectedItemIndex, optionsCount, options, prompt, head, booklist);
            break;
        case 5:
            exitFunction(*head, booklist);
            break;
    }
    refresh();
}