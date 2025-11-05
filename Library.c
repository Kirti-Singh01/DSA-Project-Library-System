#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKS 1000
#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100
#define FILENAME "library_catalog.dat"

// Structure to represent a book
typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    int year;
    float price;
    int copies;
} Book;

// Structure to manage the library catalog
typedef struct {
    Book books[MAX_BOOKS];
    int count;
} LibraryCatalog;

// Function prototypes
void initializeCatalog(LibraryCatalog *catalog);
void displayMenu();
void addBook(LibraryCatalog *catalog);
void displayBooks(LibraryCatalog *catalog);
void searchByTitle(LibraryCatalog *catalog);
void searchByAuthor(LibraryCatalog *catalog);
void sortByTitle(LibraryCatalog *catalog);
void sortByAuthor(LibraryCatalog *catalog);
void deleteBook(LibraryCatalog *catalog);
void updateBook(LibraryCatalog *catalog);
void saveCatalog(LibraryCatalog *catalog);
void loadCatalog(LibraryCatalog *catalog);

// Sorting helper functions
void quickSortByTitle(Book arr[], int low, int high);
int partitionByTitle(Book arr[], int low, int high);
void quickSortByAuthor(Book arr[], int low, int high);
int partitionByAuthor(Book arr[], int low, int high);
void swap(Book *a, Book *b);

// Binary search functions
int binarySearchByTitle(Book arr[], int n, char *title);
int binarySearchByAuthor(Book arr[], int n, char *author);

// Utility functions
void toLowerCase(char *str);
void clearInputBuffer();

int main() {
    LibraryCatalog catalog;
    int choice;

    initializeCatalog(&catalog);
    loadCatalog(&catalog);  // Load existing data if available

    printf("\n=== WELCOME TO LIBRARY CATALOG SYSTEM ===\n");
    printf("Developed for DSA Project - BTech 2nd Year\n");
    printf("==========================================\n");

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
            case 1:
                addBook(&catalog);
                break;
            case 2:
                displayBooks(&catalog);
                break;
            case 3:
                searchByTitle(&catalog);
                break;
            case 4:
                searchByAuthor(&catalog);
                break;
            case 5:
                sortByTitle(&catalog);
                printf("Books sorted by title successfully!\n");
                break;
            case 6:
                sortByAuthor(&catalog);
                printf("Books sorted by author successfully!\n");
                break;
            case 7:
                deleteBook(&catalog);
                break;
            case 8:
                updateBook(&catalog);
                break;
            case 9:
                saveCatalog(&catalog);
                printf("Catalog saved successfully!\n");
                break;
            case 0:
                saveCatalog(&catalog);
                printf("Thank you for using Library Catalog System!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

        if(choice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
        }

    } while(choice != 0);

    return 0;
}

void initializeCatalog(LibraryCatalog *catalog) {
    catalog->count = 0;
}

void displayMenu() {
    printf("\n=== LIBRARY CATALOG SYSTEM MENU ===\n");
    printf("1. Add New Book\n");
    printf("2. Display All Books\n");
    printf("3. Search by Title\n");
    printf("4. Search by Author\n");
    printf("5. Sort by Title\n");
    printf("6. Sort by Author\n");
    printf("7. Delete Book\n");
    printf("8. Update Book\n");
    printf("9. Save Catalog\n");
    printf("0. Exit\n");
    printf("===================================\n");
}

void addBook(LibraryCatalog *catalog) {
    if(catalog->count >= MAX_BOOKS) {
        printf("Catalog is full! Cannot add more books.\n");
        return;
    }

    Book *newBook = &catalog->books[catalog->count];

    printf("\n=== ADD NEW BOOK ===\n");
    printf("Enter Book ID: ");
    scanf("%d", &newBook->id);
    clearInputBuffer();

    printf("Enter Book Title: ");
    fgets(newBook->title, MAX_TITLE_LEN, stdin);
    newBook->title[strcspn(newBook->title, "\n")] = 0; // Remove newline

    printf("Enter Author Name: ");
    fgets(newBook->author, MAX_AUTHOR_LEN, stdin);
    newBook->author[strcspn(newBook->author, "\n")] = 0; // Remove newline

    printf("Enter Publication Year: ");
    scanf("%d", &newBook->year);

    printf("Enter Price: ");
    scanf("%f", &newBook->price);

    printf("Enter Number of Copies: ");
    scanf("%d", &newBook->copies);

    catalog->count++;
    printf("Book added successfully! Total books: %d\n", catalog->count);
}

void displayBooks(LibraryCatalog *catalog) {
    if(catalog->count == 0) {
        printf("No books in the catalog.\n");
        return;
    }

    printf("\n=== LIBRARY CATALOG ===\n");
    printf("%-5s %-25s %-20s %-6s %-8s %-7s\n", 
           "ID", "Title", "Author", "Year", "Price", "Copies");
    printf("------------------------------------------------------------------------\n");

    for(int i = 0; i < catalog->count; i++) {
        printf("%-5d %-25s %-20s %-6d $%-7.2f %-7d\n",
               catalog->books[i].id,
               catalog->books[i].title,
               catalog->books[i].author,
               catalog->books[i].year,
               catalog->books[i].price,
               catalog->books[i].copies);
    }
    printf("\nTotal Books: %d\n", catalog->count);
}

void searchByTitle(LibraryCatalog *catalog) {
    if(catalog->count == 0) {
        printf("No books in the catalog to search.\n");
        return;
    }

    char searchTitle[MAX_TITLE_LEN];
    printf("\nEnter title to search: ");
    fgets(searchTitle, MAX_TITLE_LEN, stdin);
    searchTitle[strcspn(searchTitle, "\n")] = 0;

    // First sort by title for binary search efficiency
    sortByTitle(catalog);

    int index = binarySearchByTitle(catalog->books, catalog->count, searchTitle);

    if(index != -1) {
        printf("\n=== BOOK FOUND ===\n");
        printf("ID: %d\n", catalog->books[index].id);
        printf("Title: %s\n", catalog->books[index].title);
        printf("Author: %s\n", catalog->books[index].author);
        printf("Year: %d\n", catalog->books[index].year);
        printf("Price: $%.2f\n", catalog->books[index].price);
        printf("Copies: %d\n", catalog->books[index].copies);
    } else {
        printf("Book with title '%s' not found.\n", searchTitle);
    }
}

void searchByAuthor(LibraryCatalog *catalog) {
    if(catalog->count == 0) {
        printf("No books in the catalog to search.\n");
        return;
    }

    char searchAuthor[MAX_AUTHOR_LEN];
    printf("\nEnter author name to search: ");
    fgets(searchAuthor, MAX_AUTHOR_LEN, stdin);
    searchAuthor[strcspn(searchAuthor, "\n")] = 0;

    printf("\n=== BOOKS BY AUTHOR: %s ===\n", searchAuthor);
    int found = 0;

    for(int i = 0; i < catalog->count; i++) {
        char authorCopy[MAX_AUTHOR_LEN];
        strcpy(authorCopy, catalog->books[i].author);
        toLowerCase(authorCopy);

        char searchCopy[MAX_AUTHOR_LEN];
        strcpy(searchCopy, searchAuthor);
        toLowerCase(searchCopy);

        if(strstr(authorCopy, searchCopy) != NULL) {
            if(!found) {
                printf("%-5s %-25s %-20s %-6s %-8s %-7s\n", 
                       "ID", "Title", "Author", "Year", "Price", "Copies");
                printf("------------------------------------------------------------------------\n");
            }
            printf("%-5d %-25s %-20s %-6d $%-7.2f %-7d\n",
                   catalog->books[i].id,
                   catalog->books[i].title,
                   catalog->books[i].author,
                   catalog->books[i].year,
                   catalog->books[i].price,
                   catalog->books[i].copies);
            found = 1;
        }
    }

    if(!found) {
        printf("No books found by author '%s'.\n", searchAuthor);
    }
}

void sortByTitle(LibraryCatalog *catalog) {
    if(catalog->count <= 1) return;
    quickSortByTitle(catalog->books, 0, catalog->count - 1);
}

void sortByAuthor(LibraryCatalog *catalog) {
    if(catalog->count <= 1) return;
    quickSortByAuthor(catalog->books, 0, catalog->count - 1);
}

void deleteBook(LibraryCatalog *catalog) {
    if(catalog->count == 0) {
        printf("No books in the catalog to delete.\n");
        return;
    }

    int deleteId;
    printf("\nEnter Book ID to delete: ");
    scanf("%d", &deleteId);

    int index = -1;
    for(int i = 0; i < catalog->count; i++) {
        if(catalog->books[i].id == deleteId) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        printf("Book with ID %d not found.\n", deleteId);
        return;
    }

    printf("Book '%s' by %s will be deleted. Are you sure? (y/n): ",
           catalog->books[index].title, catalog->books[index].author);

    char confirm;
    scanf(" %c", &confirm);

    if(confirm == 'y' || confirm == 'Y') {
        // Shift all books after the deleted book
        for(int i = index; i < catalog->count - 1; i++) {
            catalog->books[i] = catalog->books[i + 1];
        }
        catalog->count--;
        printf("Book deleted successfully! Total books: %d\n", catalog->count);
    } else {
        printf("Deletion cancelled.\n");
    }
}

void updateBook(LibraryCatalog *catalog) {
    if(catalog->count == 0) {
        printf("No books in the catalog to update.\n");
        return;
    }

    int updateId;
    printf("\nEnter Book ID to update: ");
    scanf("%d", &updateId);
    clearInputBuffer();

    int index = -1;
    for(int i = 0; i < catalog->count; i++) {
        if(catalog->books[i].id == updateId) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        printf("Book with ID %d not found.\n", updateId);
        return;
    }

    Book *book = &catalog->books[index];

    printf("\n=== UPDATE BOOK (ID: %d) ===\n", book->id);
    printf("Current Title: %s\n", book->title);
    printf("Enter new title (or press Enter to keep current): ");
    char newTitle[MAX_TITLE_LEN];
    fgets(newTitle, MAX_TITLE_LEN, stdin);
    if(strlen(newTitle) > 1) {
        newTitle[strcspn(newTitle, "\n")] = 0;
        strcpy(book->title, newTitle);
    }

    printf("Current Author: %s\n", book->author);
    printf("Enter new author (or press Enter to keep current): ");
    char newAuthor[MAX_AUTHOR_LEN];
    fgets(newAuthor, MAX_AUTHOR_LEN, stdin);
    if(strlen(newAuthor) > 1) {
        newAuthor[strcspn(newAuthor, "\n")] = 0;
        strcpy(book->author, newAuthor);
    }

    printf("Current Year: %d\n", book->year);
    printf("Enter new year (or 0 to keep current): ");
    int newYear;
    scanf("%d", &newYear);
    if(newYear != 0) book->year = newYear;

    printf("Current Price: $%.2f\n", book->price);
    printf("Enter new price (or 0 to keep current): ");
    float newPrice;
    scanf("%f", &newPrice);
    if(newPrice != 0) book->price = newPrice;

    printf("Current Copies: %d\n", book->copies);
    printf("Enter new number of copies (or 0 to keep current): ");
    int newCopies;
    scanf("%d", &newCopies);
    if(newCopies != 0) book->copies = newCopies;

    printf("Book updated successfully!\n");
}

void saveCatalog(LibraryCatalog *catalog) {
    FILE *file = fopen(FILENAME, "wb");
    if(file == NULL) {
        printf("Error: Cannot open file for saving.\n");
        return;
    }

    fwrite(&catalog->count, sizeof(int), 1, file);
    fwrite(catalog->books, sizeof(Book), catalog->count, file);
    fclose(file);
}

void loadCatalog(LibraryCatalog *catalog) {
    FILE *file = fopen(FILENAME, "rb");
    if(file == NULL) {
        printf("No existing catalog file found. Starting with empty catalog.\n");
        return;
    }

    fread(&catalog->count, sizeof(int), 1, file);
    fread(catalog->books, sizeof(Book), catalog->count, file);
    fclose(file);

    printf("Loaded %d books from existing catalog.\n", catalog->count);
}

// Quick Sort implementation for sorting by title
void quickSortByTitle(Book arr[], int low, int high) {
    if(low < high) {
        int pi = partitionByTitle(arr, low, high);
        quickSortByTitle(arr, low, pi - 1);
        quickSortByTitle(arr, pi + 1, high);
    }
}

int partitionByTitle(Book arr[], int low, int high) {
    char pivotTitle[MAX_TITLE_LEN];
    strcpy(pivotTitle, arr[high].title);
    toLowerCase(pivotTitle);

    int i = (low - 1);

    for(int j = low; j <= high - 1; j++) {
        char currentTitle[MAX_TITLE_LEN];
        strcpy(currentTitle, arr[j].title);
        toLowerCase(currentTitle);

        if(strcmp(currentTitle, pivotTitle) < 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quick Sort implementation for sorting by author
void quickSortByAuthor(Book arr[], int low, int high) {
    if(low < high) {
        int pi = partitionByAuthor(arr, low, high);
        quickSortByAuthor(arr, low, pi - 1);
        quickSortByAuthor(arr, pi + 1, high);
    }
}

int partitionByAuthor(Book arr[], int low, int high) {
    char pivotAuthor[MAX_AUTHOR_LEN];
    strcpy(pivotAuthor, arr[high].author);
    toLowerCase(pivotAuthor);

    int i = (low - 1);

    for(int j = low; j <= high - 1; j++) {
        char currentAuthor[MAX_AUTHOR_LEN];
        strcpy(currentAuthor, arr[j].author);
        toLowerCase(currentAuthor);

        if(strcmp(currentAuthor, pivotAuthor) < 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void swap(Book *a, Book *b) {
    Book temp = *a;
    *a = *b;
    *b = temp;
}

// Binary search implementation for title search
int binarySearchByTitle(Book arr[], int n, char *title) {
    int left = 0, right = n - 1;
    char searchTitle[MAX_TITLE_LEN];
    strcpy(searchTitle, title);
    toLowerCase(searchTitle);

    while(left <= right) {
        int mid = left + (right - left) / 2;

        char midTitle[MAX_TITLE_LEN];
        strcpy(midTitle, arr[mid].title);
        toLowerCase(midTitle);

        int cmp = strcmp(midTitle, searchTitle);

        if(cmp == 0) return mid;
        if(cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Binary search implementation for author search
int binarySearchByAuthor(Book arr[], int n, char *author) {
    int left = 0, right = n - 1;
    char searchAuthor[MAX_AUTHOR_LEN];
    strcpy(searchAuthor, author);
    toLowerCase(searchAuthor);

    while(left <= right) {
        int mid = left + (right - left) / 2;

        char midAuthor[MAX_AUTHOR_LEN];
        strcpy(midAuthor, arr[mid].author);
        toLowerCase(midAuthor);

        int cmp = strcmp(midAuthor, searchAuthor);

        if(cmp == 0) return mid;
        if(cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void toLowerCase(char *str) {
    for(int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void clearInputBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}
