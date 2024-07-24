#include <stdio.h>
#include <stdlib.h>

typedef struct books {
    int ID;
    char* Title;
    char* Author;
} Books;

typedef struct list {
    Books* book;
    Books* next;
} List;

List add(List* head) {
    List* newBook = head;
    int ID = 0;
    for(int i = 0; newBook->next != NULL; i++) {
        if(newBook->next != NULL) {
            newBook = newBook->next;
            ID++;
        }
    }
    newBook->book->ID = ID;
    char Title[100];
    printf("Enter the book title: ");
    scanf("%s", Title);
 }

int main() {
    List* head = NULL;
    *head = add(head);
    return 0;
}