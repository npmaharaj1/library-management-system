#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "otherFunctions.h"

void deleteBook(List** head, const char* booklist) {
    
    // Temporary Search -- To be replaced by the search function
    List* current = *head;
    List* previous = NULL;
    if (current == NULL) {
        printf("No nodes to search");
        return;
    }

    // Searchfor boko to be deleted
    char searchedBook[MAXTITLELENGTH];
    printf("Enter the book title to find: ");
    fgets(searchedBook, MAXTITLELENGTH, stdin); // Accept Input
    searchedBook[strcspn(searchedBook, "\n")] = 0; // Remove Newline

    // List all books with matching title
    List* deleteHead = NULL; // New list for books with matching title
    List* deleteCurrent = deleteHead; // New traversal node

    while (current != NULL) {
        if (strcmp(current->book->Title, searchedBook) == 0){
            if (deleteHead == NULL) { // If the list is empty
                deleteHead->book = current->book; // Set head to the match
                deleteCurrent = deleteCurrent->next; // Traverse delete list
            } else {
                deleteCurrent->book = current->book; // Otherwise set the next node in the list
                deleteCurrent = deleteCurrent->next; // Traverse delete list
            }
        } 
        previous = current;
        current = current->next;
    }

    // Print the book ID and Title ()
    deleteCurrent = deleteHead; // Reset delete traversal node
    if (deleteHead == NULL) {
        printf("Book not found!\n");
    } else {
        while (deleteCurrent != NULL) {
            printf("%d, %s\n", current->book->ID, current->book->Title);
            deleteCurrent = deleteCurrent->next;
        }
    }

    return;
}