#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct books {
    char Title[151]; // The book titles will only be able to be 150 characters
    struct books* nextNode;
} books;

void insertAtHead(books** headRef, char* data) {
    books* newNode = (books*)malloc(sizeof(books));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newNode->Title, data); // Literally just copies is so it ignores pointer issues
    newNode->nextNode = *headRef;
    *headRef = newNode;
}

int main() {
    books *head = NULL;
    char title[150];

    FILE *bookFile = fopen("books.txt", "r");
    if (bookFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    while (fgets(title, sizeof(title), bookFile) != NULL) {
        insertAtHead(&head, title);
    }

    fclose(bookFile);

    // Print the book titles
    books* current = head;
    while (current != NULL) {
        printf("%s", current->Title);
        current = current->nextNode;
    }

    // Free the allocated memory
    books* temp;
    while (head != NULL) {
        temp = head;
        head = head->nextNode;
        free(temp);
    }

    return 0;
}
