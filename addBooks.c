#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "otherFunctions.h"

void addBook(List** head, const char* booklist) {
    List* newNode = (List*)malloc(sizeof(List)); // Initialise new book node

    if(newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Initialise book
    newNode->book = (Book*)malloc(sizeof(Book)); // Initialise book structure
    if(newNode->book == NULL) {
        printf("Memory allocation for book failed\n");
        free(newNode);
        return;
    }

    // Book id
    List* current = *head;
    int i = 0;
    while(current != NULL && current->next != NULL) {
        i++;
        current = current->next;
    }
    newNode->book->ID = i + 1;

    // Book Title
    printf("Enter Book Title: ");
    fgets(newNode->book->Title, MAXTITLELENGTH, stdin); // Accept Book Title Input
    newNode->book->Title[strcspn(newNode->book->Title, "\n")] = 0; // Remove newline from end of title

    // Book Author
    printf("Enter Book Title: ");
    fgets(newNode->book->Author, MAXAUTHORLENGTH, stdin); // Accept Author Input
    newNode->book->Author[strcspn(newNode->book->Author, "\n")] = 0; // Remove newline from end of Author

    // Append book to end of list
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        current->next = newNode;
    }

    // Add to file
    FILE* file = fopen(booklist, "rb+"); // Open booklist file in append bit mode

    if(file == NULL) {
        printf("Error reading file!\n"); // Throw error if the file could not be opened
        return;
    }

    int count = 0;
    fread(&count, sizeof(int), 1, file); // Read current count
    count++; // Increment count for new book

    fseek(file, 0, SEEK_SET); // Move to the start of the file to update the count
    fwrite(&count, sizeof(int), 1, file); // Write updated count

    fseek(file, 0, SEEK_END); // Move to the end of to append the new book
    fwrite(newNode->book, sizeof(Book), 1, file); // Write the new book
    fclose(file);
    
    printf("Book added and saved successfully!\n");
    return;
}