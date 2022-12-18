#include "book.h"
#include "library.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------[ Private Function ]-------------------  */

/* Verifying that string user's input is valid number for each character
 * [ Argument ]
 *    str - pointer to char that stored the string user's input
 * [ Returns ]
 *    SUCCESS - if the string that user's input is valid number
 *    ERROR_INVALID_PUBLISH_YEAR - if the string that user's input have invalid
 * character
 */
int isNumber(char *str) {
  int count = 0;
  /* looping for checking foreach character that's numberic by if it's number
   * increase value of count */
  for (int i = 0; i < strlen(str); i++) {
    if (isdigit(str[i])) {
      count++;
    }
  }
  /* compare value of count and length of string str if not equal that's invalid
   */
  if (count != strlen(str)) {
    return ERROR_INVALID_PUBLISH_YEAR;
  } else {
    return SUCCESS;
  }
}

/* Verifying that string user's input is not space
 * [ Argument ]
 *    str - pointer to char that stored the string user's input
 * [ Returns ]
 *    0 - if the string that user's input is space
 *    1 - otherwise
 */
int isBlank(char *str) {
  if (str[0] == '\0')
    return 0;
  else
    return 1;
}

/* Display text and store string that user's input in vairable
 * [ Arguments ]
 *    text - string that want to display before user's input state
 *    str - pointer to char that will stored the string user's input
 */
void getString(char *text, char *str) {
  char buffer[CHAR_SIZE];
  /* output text that argument pass to it*/
  printf("%s: ", text);
  fgets(buffer, CHAR_SIZE, stdin);
  if (buffer[strlen(buffer) - 1] == '\n') {
    buffer[strlen(buffer) - 1] = '\0';
  }
  strcpy(str, buffer);
}

/* Display text and verify that string user's input is valid number
 * passing value to year's variable and validate out of this fucntion
 * *year = 0 means it blank , *year = -1 means length of string not equal to 4
 * [ Arguments ]
 *    text - string that want to display before user's input state
 *    str - pointer to char that will stored the string user's input
 */
void getYear(char *text, int *year) {

  char buffer[YEAR_SIZE];
  int valid = ERROR_INVALID_PUBLISH_YEAR;
  while (valid == ERROR_INVALID_PUBLISH_YEAR) {
    printf("%s: ", text);
    fgets(buffer, YEAR_SIZE, stdin);
    if (buffer[strlen(buffer) - 1] == '\n') {
      buffer[strlen(buffer) - 1] = '\0';
    }
    /* if it blank pass value of year to 0 means is blank */
    if (isBlank(buffer) == 0) {
      *year = 0;
      return;
    }
    /* if the string that user's input not eqaul to 4 pass value to -1 */
    if (strlen(buffer) != 4) {
      *year = -1;
      return;
    }
    valid = isNumber(buffer);
    printf("%s", MESSAGE[valid]);
  }
  /* convert character to integer */
  *year = atoi(buffer);
  /* Clean up buffer input in stdin */
  while ((getchar()) != '\n')
    ;
}

/* Verifying that string user's input is not space
 * [ Arguments ]
 *    library    - pointer to LIBRARY_T that stored data of books
 *    title   - pointer to char that stored the string user's input
 * [ Returns ]
 *    -1      - if the string that user's input does not exists in library
 *    index   - if the string that user's input found in title of books in the
 * library.
 */

int findIndexByTitle(LIBRARY_T *library, char *title) {
  int index = -1;
  for (int i = 0; i < library->currentSize; i++) {
    if (strcmp(title, library->books[i].title) == 0) {
      index = i;
      break;
    }
  }
  return index;
}

/* -------------------[ Public Borrow Function ]-------------------  */

/* Allocate Memory for stored data in struct BORROW_T */
BORROW_T *borrowHistoryCreate() {
  BORROW_T *data = (BORROW_T *)calloc(1, sizeof(BORROW_T));
  data->name = calloc(CHAR_SIZE, sizeof(char));
  data->status = '0';
  return data;
}
/* Allocate Memory for stored data in struct BORROW_HISTORY_T */
BORROW_HISTORY_T *borrowHistorylistCreate(int initialCapacity) {
  BORROW_HISTORY_T *list =
      (BORROW_HISTORY_T *)calloc(1, sizeof(BORROW_HISTORY_T));
  list->data = (BORROW_T *)calloc(initialCapacity, sizeof(BORROW_T));
  list->currentSize = 0;
  list->maxSize = initialCapacity;
  return list;
}

/* Check the current size of struct borrowing history is equal to max size, then
 * expand size for add more history of borrowing books [ Argument ] list -
 * pointer to struct BORROW_HISTORY_T that storing history of borrowing books
 *    value - pointer to BOOK_T struct storing the data in each book
 */
void borrowHistorylistAppend(BORROW_HISTORY_T *list, BORROW_T *value) {

  if (list->currentSize == list->maxSize) {
    borrowHistorylistExpand(list);
  }
  /* Set the list->data[list->currentSize] is new borrowing history */
  list->data[list->currentSize] = *value;
  /* Update library->currentSize  */
  list->currentSize += 1;
}

/* Expand allocate memmory of library and books
 * [ Argument ]
 *    list  -  pointer to struct BORROW_HISTORY_T that storing history of
 * borrowing books
 */
void borrowHistorylistExpand(BORROW_HISTORY_T *list) {
  /* Allocate double of list->maxSize memmory for temporary BORROW_T */
  BORROW_T *tmp = (BORROW_T *)calloc(list->maxSize * 2, sizeof(BORROW_T));

  int i;
  /* Move borrowing history in library to temporary BORROW_HISTORY_T */
  for (i = 0; i < list->currentSize; i++) {
    tmp[i] = list->data[i];
  }
  /* Clear borrowing history from initial history */
  free(list->data);
  /* Set list->data is temporary BORROW_HISTORY_T list */
  list->data = tmp;
  /* double list->maxSize */
  list->maxSize *= 2;
}

/* -------------------[ Public Book Function ]-------------------  */

/* Allocate memmory for pointer to struct BOOK_T and detail of book
 * (title,author,history) */
BOOK_T *bookCreate() {
  BOOK_T *book = (BOOK_T *)calloc(1, sizeof(BOOK_T));
  book->title = calloc(CHAR_SIZE, sizeof(char));
  book->author = calloc(CHAR_SIZE, sizeof(char));
  book->year = 0;
  book->status = 'A';
  book->history = (BORROW_HISTORY_T *)calloc(1, sizeof(BORROW_HISTORY_T));
  return book;
}

/* Get detail in a book from user
 * [ Argument ]
 *    library  -  pointer to LIBRARY_T that stored data of books
 */
void AddBook(LIBRARY_T *library) {

  // Annouce Variable
  char title[CHAR_SIZE];
  char author[CHAR_SIZE];
  int year = 0;
  /* Annouce valid for validate input from user */
  int valid = 0;
  int index = -1;
  /* Call function bookCreate to allocate memmory for a book */
  BOOK_T *book = bookCreate();
  /* Validate the book's title from user */
  while (valid == 0) {
    getString("Enter title", title);
    valid = isBlank(title);
    if (valid == 0) {
      printf("%s\n", MESSAGE[ERROR_INVALID_TITLE]);
    } else {
      index = findIndexByTitle(library, title);
      /* Check if the title is already exists in library, input again */
      if (index != -1) {
        valid = 0;
        index = -1;
        printf("%s \n", MESSAGE[ERROR_BOOK_ALREADY_EXISTS]);
      }
    }
  }

  valid = 0;
  /* Validate the book's author from user */
  while (valid == 0) {
    getString("Enter author name", author);
    valid = isBlank(author);
    if (valid == 0)
      printf("%s\n", MESSAGE[ERROR_INVALID_AUTHOR]);
  }

  /* Validate the book's publish year from user */
  while (year == 0) {
    getYear("Enter publish year", &year);
    if (year == 0 || year == -1 || year < 1000 || year > 2022) {
      printf("%s\n", MESSAGE[ERROR_INVALID_PUBLISH_YEAR]);
      year = 0;
    }
  }

  /* Move detail in a book to Book Struct */
  strcpy(book->title, title);
  strcpy(book->author, author);
  book->year = year;
  book->history = borrowHistorylistCreate(2);
  /* Add the book in library */
  libraryAppend(library, book);
  /* Save book's detail in file (Books.txt) */
  SaveFile(FILE_PATH, library);
  printf("%s\n", MESSAGE[NEW_DATA_SUCCESS]);
}

/* Remove book from library
 * [ Argument ]
 *    library  -   pointer to LIBRARY_T that stored data of books
 */
void RemoveBook(LIBRARY_T *library) {
  int index = -1;
  char title[CHAR_SIZE];
  getString("Enter title to remove", title);
  /* Search title, if it not exist, then return */
  index = findIndexByTitle(library, title);
  if (index == -1) {
    printf("%s\n", MESSAGE[ERROR_NOT_FOUND]);
    return;
  }

  /* Keep book that want to remove in BOOK_T removeData */
  BOOK_T removeData = library->books[index];
  /* Sort the new order (the ordering of later removed book have changed ) */
  for (int i = index; i < library->currentSize; i++) {
    library->books[i] = library->books[i + 1];
  }

  /* Decrease the total of books */
  library->currentSize -= 1;
  /* Clear all allocated data of removed book */
  for (int i = 0; i < removeData.history->currentSize; i++) {
    free(removeData.history->data[i].name);
  }
  free(removeData.history->data);
  free(removeData.history);
  free(removeData.title);
  free(removeData.author);
  SaveFile(FILE_PATH, library);
  printf("%s\n", MESSAGE[DELETE_DATA_SUCCESS]);
}

/* Edit title, author and publish year of book
 *  [ Argument ]
 *    library - pointer to LIBRARY_T that stored data of books
 */
void EditBook(LIBRARY_T *library) {
  int index = -1;
  int search_index = -1;
  char title[CHAR_SIZE];
  getString("Enter title to edit", title);
  index = findIndexByTitle(library, title);
  /* Search the title from user, if title don't have in library, then return */
  int NewYear;
  if (index == -1) {
    printf("%s", MESSAGE[ERROR_NOT_FOUND]);
    return;
  }
  /* Validate title, if title of book that user's input is have is Library so
   * input again  */
  int valid = 0;
  while (valid == 0) {
    char *NewTitle = calloc(CHAR_SIZE, sizeof(char));
    /* stored user's input title in NewTitle variable. */
    getString("New title (leave blank to retain old value)", NewTitle);
    /* check NewTitle variable : Is it blank? , if blank break; */
    if (isBlank(NewTitle) == 0) {
      free(NewTitle);
      break;
    }
    search_index = findIndexByTitle(library, NewTitle);
    /* if search_index == -1 that means not have title is same user's input */
    if (search_index == -1 && NewTitle[0] != '\n') {
      free(library->books[index].title);
      library->books[index].title = NewTitle;
      valid = 1;
    } else {
      printf("%s\n", MESSAGE[ERROR_BOOK_ALREADY_EXISTS]);
      free(NewTitle);
      continue;
    }
  }

  char *NewAuthor = calloc(CHAR_SIZE, sizeof(char));
  /* Validate author, if it not correct, input again */
  getString("New author name(leave blank to retain old value)", NewAuthor);
  /* Check input, if it blank, library->books[index].author don't change */
  if (isBlank(NewAuthor) == 1) {
    free(library->books[index].author);
    library->books[index].author = NewAuthor;
  }

  /* Validate year, if it not correct, input again */
  NewYear = -1;
  while (NewYear == -1) {
    getYear("New pulblish year(leave blank to retain old value)", &NewYear);
    if (NewYear >= 1000 && NewYear <= 2022) {
      library->books[index].year = NewYear;
      break;
    } else if (NewYear == 0) {
      break;
    }
    printf("%s\n", MESSAGE[ERROR_INVALID_PUBLISH_YEAR]);
    NewYear = -1;
  }

  SaveFile(FILE_PATH, library);
  printf("%s\n", MESSAGE[NEW_DATA_SUCCESS]);
}

/*
 * search books by title and author if leave blank will include all titles /
 * authors [ Argument ] library -  pointer to LIBRARY_T that stored data of
 * books
 */
void SearchBook(LIBRARY_T *library) {
  int valid = 0;
  char title[CHAR_SIZE], author[CHAR_SIZE];
  /* stored user's input to local variables(title,author) */
  getString("Enter title (leave blank to include all titles) ", title);
  getString("Enter author name (leave blank to include all titles) ", author);
  /* for looping set lower all characters in variables */
  for (int i = 0; i < strlen(title); i++) {
    title[i] = tolower(title[i]);
  }
  for (int i = 0; i < strlen(author); i++) {
    author[i] = tolower(author[i]);
  }

  printf("\n---------------[ Results ]---------------\n\n");
  int count = 0;
  for (int i = 0; i < library->currentSize; i++) {
    /* allocate memory for stored string */
    char *toLowerTitle = calloc(CHAR_SIZE, sizeof(char));
    char *toLowerAuthor = calloc(CHAR_SIZE, sizeof(char));
    /* for looping set lower all characters from library->book[i].title and
     * stored in new local variable that's toLowerTitle */
    for (int j = 0; j < strlen(library->books[i].title); j++) {
      toLowerTitle[j] = tolower(library->books[i].title[j]);
    }
    /* for looping set lower all characters from library->book[i].author and
     * stored in new local variable that's toLowerAuthor */
    for (int j = 0; j < strlen(library->books[i].author); j++) {
      toLowerAuthor[j] = tolower(library->books[i].author[j]);
    }
    /* Compare toLowerTitle that stored data form Library and title that user's
     * input AND toLowerAuthor that stored data form Library and author that
     * user's input
     */
    if (strstr(toLowerTitle, title) != NULL &&
        strstr(toLowerAuthor, author) != NULL) {
      count++;
      printf("\t\t\t[ Book ID#%d ]\n", i + 1);
      printf("-----------------------------------------");
      libraryShowBookDetail(library, i);
      printf("-----------------------------------------\n");
    }
    /* free memory that allocate */
    free(toLowerTitle);
    free(toLowerAuthor);
  }
  printf("Match about %d results\n", count);
  printf("-----------------------------------------\n");
}
/* Borrow the book by input the borrower name and book that want to borrow
 * if book's status is Borrowed that means is Unavailable
 * if index == - 1 that means not have that book in Library
 * [ Argument ]
 *       library -  pointer to LIBRARY_T that stored data of books
 */
void BorrowBook(LIBRARY_T *library) {

  char title[CHAR_SIZE], name[CHAR_SIZE];
  int valid = 0;
  int index = -1;
  /* Allocate memory by borrowHistoryCreate function in borrow that's pointer to
   * BORROW_T struct */
  BORROW_T *borrow = borrowHistoryCreate();

  /* get user's input that stored in name variable and validate if blank input
   * again */
  while (valid == 0) {
    getString("Enter borrower name", name);
    valid = isBlank(name);
    if (valid == 0)
      printf("%s\n", MESSAGE[ERROR_INVALID_STRING]);
  }
  /* get user's input that stored in title variable and validate if blank input
   * again */
  valid = 0;
  while (valid == 0) {
    getString("Enter title to borrow", title);
    valid = isBlank(title);
    if (valid == 0)
      printf("%s\n", MESSAGE[ERROR_INVALID_TITLE]);
  }
  /* search book's index by title if found set variable index = index of title
   * that found otherwise -1 */
  index = findIndexByTitle(library, title);

  if (index == -1) {
    printf("%s\n", MESSAGE[ERROR_NOT_FOUND]);
    return;
  }
  /* validate status of book that is available to borrow by status
   * B = Borrowed , A = Available
   */
  if (library->books[index].status == 'B') {
    printf("%s\n", MESSAGE[ERROR_BOOK_UNAVAILABLE]);
    return;
  }
  printf("------------[ Borrow Success ]-----------\n");
  /* copy name to borrow->name , set borrow->status to B | B = Borrow , R =
   * Returned and set library->books[index].status to B that show this book is
   * unavailable for borrow */
  strcpy(borrow->name, name);
  borrow->status = 'B';
  library->books[index].status = 'B';
  /* append data of borrow to list of history that stored in book */
  borrowHistorylistAppend(library->books[index].history, borrow);
  SaveFile(FILE_PATH, library);
}
/* Return the book by title that want to return
 * if book's status is Available that means is Returned
 * if index == - 1 that means not have that book in Library
 * [ Argument ]
 *       library -  pointer to LIBRARY_T that stored data of books
 */
void ReturnBook(LIBRARY_T *library) {
  char title[CHAR_SIZE];
  int valid = 0;
  int index = -1;
  /* Allocate memory by borrowHistoryCreate function in borrow that's pointer to
   * BORROW_T struct */
  BORROW_T *borrow = borrowHistoryCreate();
  /* get user's input that stored in title variable and validate if blank input
   * again */
  while (valid == 0) {
    getString("Enter title to return", title);
    valid = isBlank(title);
    if (valid == 0)
      printf("%s\n", MESSAGE[ERROR_INVALID_TITLE]);
  }
  /* search book's index by title if found set variable index = index of title
   * that found otherwise -1 */
  index = findIndexByTitle(library, title);

  if (index == -1) {
    printf("%s", MESSAGE[ERROR_NOT_FOUND]);
    return;
  }
  /* validate status of book that is Borrowed to return by status
   * B = Borrowed , A = Available
   */
  if (library->books[index].status == 'A') {
    printf("%s\n", MESSAGE[ERROR_BOOK_ALREADY_RETURNED]);
    return;
  }

  printf("------------[ Return Success ]-----------\n");
  int currentSize = library->books[index].history->currentSize;
  /* copy name form previous record of this book to borrow->name and
   * set borrow->status = R | R = Return , B , Borrow
set library->books[index].status to A that show this book is unavailable for
return */
  strcpy(borrow->name,
         library->books[index].history->data[currentSize - 1].name);
  borrow->status = 'R';
  library->books[index].status = 'A';
  /* append data of borrow to list of history that stored in book */
  borrowHistorylistAppend(library->books[index].history, borrow);
  SaveFile(FILE_PATH, library);
}