#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // this will be needed for date handling later

#define MAXTITLELENGTH 100 // Max length a books title can be
#define MAXAUTHORLENGTH 50 // Max length an Authors name can be
#define MAXNAMELENGTH 50 // Max length a name can be
#define MAXEMAILLENGTH 100 // Max length an email can be

typedef struct date { // this might need to go in a separate time.c file so that we can separate out the date handling later.
    int day;
    int month;
    int year;
} Date;

typedef struct person {
    int ID;
    char name[MAXNAMELENGTH];
    char email[MAXEMAILLENGTH];
} Person;

typedef struct books {
    int ID;
    char Title[MAXTITLELENGTH];
    char Author[MAXAUTHORLENGTH];
    Date* issueDate;
    Date* dueDate; 
    Person* borrowedTo; // name of person who has borrowed the book.   
} Book;

typedef struct list {
    Book* book;
    struct list* next;
} List;

//Read From File Function
List* readBooksFromFile(const char* booklist) {
    FILE* file = fopen(booklist, "rb");
    if(file == NULL) {
        printf("Error reading file\n"); // Throw error if the file could not be opened
        return NULL;
    }
    
    int count; // The file will contain the number of books
    fread(&count, sizeof(int), 1, file); // Read number of books from file

    List* head = NULL; // Initialise head node
    List* current = NULL; // Initialise traversal node

    for(int i = 0; i < count; i++) {
        List* newNode = (List*)malloc(sizeof(List));
        if (newNode == NULL) {
            printf("Memory Allocation Failed!\n");
            fclose(file);
            return NULL;
        }

        newNode->book = (Book*)malloc(sizeof(Book)); // Allocate memory for book
        if (newNode->book == NULL) {
            printf("Memory Allocation for Book Failed!\n");
            free(newNode);
            fclose(file);
            return NULL;
        } 

        fread(newNode->book, sizeof(Book), 1, file); // Read the book from the file
        newNode->next == NULL;

        if(head == NULL) {
            head = newNode; // If the head is null, assign this book to be the head
            current = head; // Set the current position to the head 
        } else {
            current->next = newNode; // Set the next node to be the book
            current = current->next; // Traverse to the next book
        }
    }
    fclose(file); // Close the file
    printf("Books loaded successfully!\n");
    return head;
}

// Add book function
void addBook(List** head, const char* booklist) {
    List* newNode = (List*)malloc(sizeof(List)); // Initialise new book node

    if(newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Initialise the book
    newNode->book = (Book*)malloc(sizeof(Book)); // Initialize the Book struct
    if (newNode->book == NULL) {
        printf("Memory allocation for book failed!\n");
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
    fgets(newNode->book->Title, MAXTITLELENGTH, stdin); // Accept input for Title
    newNode->book->Title[strcspn(newNode->book->Title, "\n")] = 0; // Remove Newline

    // Book Author
    printf("Enter Author Name: ");
    fgets(newNode->book->Author, MAXAUTHORLENGTH, stdin); // Accept input for Author
    newNode->book->Author[strcspn(newNode->book->Author, "\n")] = 0; // Remove Newline

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

void freeBooks(List *head) {
    List *current = head;
    List *next;
    while (current != NULL) {
        next = current->next;
        free(current->book);
        free(current);
        current = next;
    }
}

int main() {
    List* head = NULL; // Initialise the head of the list
    const char* booklist = "booklist.dat"; // File name of the booklist data file

    // Check if the file exists and is not empty
    FILE* file = fopen(booklist, "rb");
    if (file == NULL) {
        // If the file doesn't exist, create one with count set to 0
        file = fopen(booklist, "wb");
        if (file == NULL) {
            printf("Error creating file");
            return 1;
        }
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        fclose(file);
    }

    head = readBooksFromFile(booklist); // Parse the booklist file and set the head node

    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. List Books\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline left by scanf

        switch (choice) {
            case 1:
                addBook(&head, booklist);
                break;
            case 2: {
                List *current = head;
                printf("Book list:\n\n");
                while (current != NULL) {
                    printf("ID: %d, Title: %s, Author: %s\n",
                           current->book->ID, current->book->Title, current->book->Author);
                    current = current->next;
                }
                break;
            default:
                printf("Invalid choice, please try again!\n");
                break;
            }
            case 3:
                printf("Exiting....\n");
                break;
        }
    } while (choice != 3);
    
    freeBooks(head);

    return 0;
}