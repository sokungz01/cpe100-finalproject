#ifndef LIBRARY_H
#define LIBRARY_H

#define SUCCESS 0
#define ERROR_INVALID_TITLE 1
#define ERROR_INVALID_AUTHOR 2
#define ERROR_INVALID_PUBLISH_YEAR 3
#define ERROR_INVALID_STRING 4
#define ERROR_INVALID_INPUT_MENU 5
#define ERROR_BOOK_ALREADY_EXISTS 6
#define ERROR_NOT_FOUND 7
#define ERROR_BOOK_UNAVAILABLE 8
#define ERROR_BOOK_ALREADY_RETURNED 9
#define NEW_DATA_SUCCESS 10
#define DELETE_DATA_SUCCESS 11
#define FILE_PATH "Books.txt"
#define CHAR_SIZE 128
#define YEAR_SIZE 5

static const char *MESSAGE[] = {
    "",                                       // SUCCESS
    "Error! | Invalid title",                 // ERROR_INVALID_TITLE
    "Error! | Invalid author",                // ERROR_INVALID_AUTHOR
    "Error! | Invalid publish year",          // ERROR_INVALID_PUBLISH_YEAR
    "Error! | Invalid string",                // ERROR_INVALID_STRING
    "Error! | Please choose number 1 to 8 !", // ERROR_INVALID_INPUT_MENU
    "Error! | The book is already exists",    // ERROR_BOOK_ALREADY_EXISTS
    "Error! | Not found",                     // ERROR_NOT_FOUND
    "Error! | The Book is unavailable",       // ERROR_BOOK_UNAVAILABLE
    "Error! | The book is already returned",  // ERROR_BOOK_ALREADY_RETURNED
    "Success | Add the data of book successfully",    // NEW_DATA_SUCCESS
    "Success | Remove the data of book successfully", // DELETE_DATA_SUCCESS
};

typedef struct {
  char *name;
  char status; // B -> borrow , R-> return
} BORROW_T;

typedef struct {
  BORROW_T *data; // array of BORROW
  int currentSize;
  int maxSize;
} BORROW_HISTORY_T;

typedef struct {
  char *title;
  char *author;
  int year;
  char status;
  BORROW_HISTORY_T *history;
} BOOK_T;

typedef struct {
  BOOK_T *books; // array of BOOK_T
  int currentSize;
  int maxSize;
} LIBRARY_T;

void SelectMenu(LIBRARY_T *library);

LIBRARY_T *createlibraryFromFile(char *path);

void SaveFile(char *path, LIBRARY_T *library);

LIBRARY_T *libraryCreate(int initialCapacity);

void libraryAppend(LIBRARY_T *library, BOOK_T *value);

void libraryExpand(LIBRARY_T *library);

void libraryShowBookDetail(LIBRARY_T *library, int index);

void libraryDestroy(LIBRARY_T *library);

#endif