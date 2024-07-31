#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "otherFunctions.h"

void deleteBook(List** head, const char* booklist) {

    if (head == NULL || *head == NULL) {
        printf("No nodes to search\n");
        return;
    }
    
    // Temporary Search -- To be replaced by the search function
    List* current = *head;
    List* previous = NULL;
    if (current == NULL) {
        printf("No nodes to search");
        return;
    }

    // Searchfor book to be deleted
    char searchedBook[MAXTITLELENGTH];
    printf("Enter the book title to find: ");
    fgets(searchedBook, MAXTITLELENGTH, stdin); // Accept Input
    searchedBook[strcspn(searchedBook, "\n")] = 0; // Remove Newline

    // List all books with matching title
    List* deleteHead = NULL; // New list for books with matching title
    List* deleteCurrent = deleteHead; // New traversal node

    while (current != NULL) {
        if (strcmp(current->book->Title, searchedBook) == 0){
            List* newNode = (List *)malloc(sizeof(List));
            if (newNode == NULL) {
                printf("Memory Allocation Error");
                return;
            }

            newNode->book = current->book;
            newNode->next = NULL;

            if (deleteHead == NULL) { // If the list is empty
                deleteHead = newNode; // Set head to the match
                deleteCurrent = deleteHead; // Traverse delete list
            } else {
                deleteCurrent->next = newNode; // Otherwise set the next node in the list
                deleteCurrent = deleteCurrent->next; // Traverse delete list
            }
        } 
        previous = current;
        current = current->next;
    }

    // Print the book/s ID and Title
    deleteCurrent = deleteHead; // Reset delete traversal node
    if (deleteHead == NULL) {
        printf("Book not found!\n");
    } else {
        // Print all of the books found
        while (deleteCurrent != NULL) {
            printf("%d, %s\n", deleteCurrent->book->ID, deleteCurrent->book->Title);
            deleteCurrent = deleteCurrent->next;
        }
    }


    // Confirm which book is to be deleted
    int deleteBookID;
    printf("Enter Book ID to be Deleted: ");
    scanf("%d", &deleteBookID);
    getchar();


    // Iterate through Book again to find the ID
    current = *head;
    previous = NULL;

    while (current != NULL && current->book->ID != deleteBookID) {
        previous = current;
        current = current->next;
    }

    // Check if book ID is valid, print deleting message
    if (current != NULL && strcmp(current->book->Title, searchedBook) == 0) {
        printf("Deleting book %d: %s by %s ...\n", current->book->ID, current->book->Title, current->book->Author);
    } else {
        printf("Invalid Input!\n");
        return;
    }

    // Delete the book from the List
    if (previous == NULL) {
        *head = current->next; // If the first node was deleted, new head
    } else {
        previous->next = current->next; // Otherwise set the previous's next to the next node
    }
    free(current->book);
    free(current);

    // Free the delete list
    deleteCurrent = deleteHead;
    while (deleteCurrent != NULL) {
        List* temp = deleteCurrent;
        deleteCurrent = deleteCurrent->next;
        free(temp);
    }

    return;
}