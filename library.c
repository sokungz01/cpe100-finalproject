
#include "library.h"
#include "book.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------[ Private Function ]-------------------  */

/*
 * Convert string that contain space before save as Books.txt file
 * if does not convert string to another symbol data that stored in variable
 * will be invalid
 * [ Arguments ]
 *     str - pointer to char that we want to convert space to dollar sign
 */
void convertSpacetoDollarSign(char *str) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == ' ' && i != strlen(str) - 1) {
      str[i] = '$';
    }
  }
}
/*
 * Convert string that contain symbol after read Books.txt file
 * [ Arguments ]
 *     str - pointer to char that we want to convert dollar sign to space
 */
void convertDollarSigntoSpace(char *str) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == '$' && i != strlen(str) - 1) {
      str[i] = ' ';
    }
  }
}

/*
 * Menu Function that get the user's input and validate that is valid and
 * execute function that user want to do.
 * -------------------------
 * Choice 1 - Add Book
 * Choice 2 - Remove Book
 * Choice 3 - Edit Book
 * Choice 4 - Show all Book
 * Choice 5 - Search Book
 * Choice 6 - Borrow Book
 * Choice 7 - Return Book
 * Choice 8 - Exit
 * -------------------------
 * Otherwise is invalid
 * [ Arguments ]
 *     library - Pointer to LIBRARY_T that stored data of books.
 */

/* -------------------[ Menu Function ]-------------------  */
void SelectMenu(LIBRARY_T *library) {
  char Choice_Char[3];
  int Choice = 0, valid = 0;
  while (Choice != 8) {
    printf("=========================================");
    printf("\nMenu\n1) Add book\n2) Remove Book\n3) Edit book\n4) Show all "
           "book\n5) Search "
           "Book\n6) Borrow Book\n7) Return Book\n8) Exit\n");
    printf("> ");
    /* Begin the user's inputer checker */
    while (valid == 0) {

      fscanf(stdin, "%s", Choice_Char);
      if (Choice_Char[strlen(Choice_Char) - 1] == '\n') {
        Choice_Char[strlen(Choice_Char) - 1] = '\0';
      }

      /* Validate user's input is digits */
      if (isdigit(Choice_Char[0]) == 0 ||
          (isdigit(Choice_Char[1]) == 0 && strlen(Choice_Char) > 1)) {
        printf("%s\n", MESSAGE[ERROR_INVALID_INPUT_MENU]);
        continue;
      }

      /* Convert character that user's input to integer */
      Choice = atoi(&Choice_Char[0]);
      valid = 1;
      fflush(stdin);
    }
    /* Clean up buffer input in stdin */
    while ((getchar()) != '\n')
      ;
    printf("=========================================\n");
    if (Choice == 1) {
      AddBook(library);
    } else if (Choice == 2) {
      RemoveBook(library);
    } else if (Choice == 3) {
      EditBook(library);
    } else if (Choice == 4) {
      if (library->currentSize == 0)
        printf("This library does not have any books.\n");
      for (int i = 0; i < library->currentSize; i++) {
        printf("\t\t\t[ Book ID#%d ]\n", i + 1);
        printf("-----------------------------------------");
        libraryShowBookDetail(library, i);
        printf("-----------------------------------------\n");
      }
    } else if (Choice == 5) {
      SearchBook(library);
    } else if (Choice == 6) {
      BorrowBook(library);
    } else if (Choice == 7) {
      ReturnBook(library);
    } else if (Choice == 8) {
      libraryDestroy(library);
      return;
    } else {
      printf("%s\n", MESSAGE[ERROR_INVALID_INPUT_MENU]);
      SelectMenu(library);
      return;
    }
    valid = 0;
  }
}

/* -------------------[ Read/Write Function ]-------------------  */

/* Read a Books.txt file and create LIBRARY_T struct
 * [ Argument ]
 *     path - path to input file (Books.txt file)
 * [ Returns ]
 *     NULL - if the file is not existed
 *     library - pointer to library_OF_BOOT_T struct that stored data of books
 */
LIBRARY_T *createlibraryFromFile(char *path) {
  FILE *fp = fopen(path, "r");
  /* opent the Books.txt file , return NULL if file can't be open / not existed
   */
  if (fp == NULL) {
    FILE *fp2 = fopen(path, "w");
    LIBRARY_T *library = libraryCreate(2);
    fclose(fp2);
    return library;
  }

  int year, capacity, history_capacity, max_history_capacity;
  char title[CHAR_SIZE], author[CHAR_SIZE], borrow_status, status;
  /* Read number of books that file have */
  fscanf(fp, "%d", &capacity);
  if (capacity == EOF || capacity == 0) {
    LIBRARY_T *library = libraryCreate(2);
    return library;
  }

  /* Create variable library that's pointer to LIBRARY_T struct that will
   * allocate memory by libraryCreate function with capacity argument */
  LIBRARY_T *library = libraryCreate(capacity);
  for (int i = 0; i < capacity; i++) {
    /* Create variable book that's pointer to BOOK_T struct that will allocate
     * memory by bookCreate function */
    BOOK_T *book = bookCreate();
    /* Read title, author, year, status of book foreach book */
    fscanf(fp, "%s %s %d %c", title, author, &year, &status);
    /* Convert dollar sign to space in the title and author that stored in file
     * before stored data in book's data */
    convertDollarSigntoSpace(title);
    convertDollarSigntoSpace(author);
    /* Copy title to book->title , author to book->author and year to book->year
     */
    strcpy(book->title, title);
    strcpy(book->author, author);
    book->year = year;
    /* set book->status by status that read form file */
    book->status = status;
    /* set book->history->maxSize by 2 for default size */
    book->history->maxSize = 2;
    /* Read history_capacity and max_history_capacity form file */
    fscanf(fp, "%d", &history_capacity);
    fscanf(fp, "%d", &max_history_capacity);

    /* Create variable library_history that's pointer to BORROW_HISTORY_T struct
     * that will allocate memory by borrowHistorylistCreate function with
     * history_capacity argument */
    BORROW_HISTORY_T *library_history =
        borrowHistorylistCreate(history_capacity);
    /* loop read name of borrower and borrow status */
    for (int j = 0; j < history_capacity; j++) {
      /* allocate memory for variable name that's pointer to char by calloc
       * function with CHAR_SIZE and sizeof(char) arguments*/
      char *name = calloc(CHAR_SIZE, sizeof(char));
      /* Create variable history that's pointer to BORROW_T struct that will
       * allocate memory by borrowHistoryCreate function */
      BORROW_T *history = borrowHistoryCreate();
      /* Read borrower's name and borrorwer's status form file foreach record*/
      fscanf(fp, "%s %c", name, &borrow_status);
      /* Convert dollar sign to space in the name that stored in file before
       * stored data in Borrow's data */
      convertDollarSigntoSpace(name);
      /* Copy borrower's name to history->name */
      strcpy(history->name, name);
      /* set history->status by borrow_status */
      history->status = borrow_status;
      /* append history of borrow record to library_history by
       * borrowHistorylistAppend function */
      borrowHistorylistAppend(library_history, history);
      free(name);
    }
    /* set book->history by library_history for each book */
    book->history = library_history;
    /* append book record to library by libraryAppend function */
    libraryAppend(library, book);
  }
  /* Close file stream */
  fclose(fp);
  return library;
}

/* Write a LIBRARY_T struct to a Books.txt file
 * [ Arguments ]
 *     path - path to output file (Books.txt file)
 *     library - pointer to LIBRARY_T struct storing the books's data
 */
void SaveFile(char *path, LIBRARY_T *library) {
  FILE *fp = fopen(path, "w");
  /* open a new file to write by path return; if the file can't be created */
  if (fp == NULL)
    return;
  /* validate input library , return; if the struct is null */
  if (library == NULL)
    return;

  char title[CHAR_SIZE], author[CHAR_SIZE], name[CHAR_SIZE];
  /* Output number of books that stored in library on first line of stream */
  fprintf(fp, "%d \n", library->currentSize);
  /* loop for written data of book foreach book */
  for (int i = 0; i < library->currentSize; i++) {
    /* Copy string form library->books[i].title to title and
     * string form library->books[i].author to author
     */
    strcpy(title, library->books[i].title);
    strcpy(author, library->books[i].author);
    /* Convert space to dollar sign in the title and author
     * before stored data in stream */
    convertSpacetoDollarSign(title);
    convertSpacetoDollarSign(author);
    /* Output title,author,year,status,history->currentSize,history->maxSize
     * that stored in library->books[i] to stream*/
    fprintf(fp, "%s %s %d %c ", title, author, library->books[i].year,
            library->books[i].status);
    fprintf(fp, "%d %d ", library->books[i].history->currentSize,
            library->books[i].history->maxSize);
    /* loop for written borrow history data of book foreach book */
    for (int j = 0; j < library->books[i].history->currentSize; j++) {
      /* Copy string form library->books[i].history->data[j].name to name*/
      strcpy(name, library->books[i].history->data[j].name);
      /* Convert space to dollar sign in the name before stored data in stream
       */
      convertSpacetoDollarSign(name);
      /* Output borrower's name and borrow status that stored in
       * library->books[i].history->data[j] to stream*/
      fprintf(fp, "%s %c ", name, library->books[i].history->data[j].status);
    }
    /* new line for new data of book*/
    fprintf(fp, "\n");
  }
  /* Close file stream */
  fclose(fp);
}

/* -------------------[ library Function ]-------------------  */

/* Allocate memmory for pointer to struct LIBRARY_T and pointer to struct
 * BOOK_T, set total of books in library [ Argument ] initialcapacity - initial
 * number of books in library
 */
LIBRARY_T *libraryCreate(int initialCapacity) {
  /* Allocate memmory for struct LIBRARY_T* and BOOK_T* for initalCapacity */
  LIBRARY_T *library = (LIBRARY_T *)calloc(1, sizeof(LIBRARY_T));
  library->books = (BOOK_T *)calloc(initialCapacity, sizeof(BOOK_T));
  /* Set the total of book that didn't add any book in library */
  library->currentSize = 0;
  /* Set the total books can keep in library */
  library->maxSize = initialCapacity;

  return library;
}

/* Check the library->currentSize is equal to library->maxSize, then expand
 * library for add more book [ Argument ] library - pointer to LIBRARY_T struct
 * storing the books's data value - pointer to BOOK_T struct storing the data in
 * each book
 */
void libraryAppend(LIBRARY_T *library, BOOK_T *value) {
  /* Check size of library if library not enough to keep book, expand library */
  if (library->currentSize == library->maxSize) {
    libraryExpand(library);
  }
  /* Set the library->books[library->currentSize] is new added book */
  library->books[library->currentSize] = *value;
  /* Update library->currentSize  */
  library->currentSize += 1;
}

/* Expand allocate memmory of library and books
 * [ Argument ]
 *    library - pointer to LIBRARY_T struct storing the books's data
 */
void libraryExpand(LIBRARY_T *library) {
  /* Allocate double of library->maxSize memmory for temporary BOOK_T */
  BOOK_T *tmp = (BOOK_T *)calloc(library->maxSize * 2, sizeof(BOOK_T));
  /* Move books in library to temporary BOOK_T*/
  int i;
  for (i = 0; i < library->currentSize; i++) {
    tmp[i] = library->books[i];
  }
  /* Clear books from initial library */
  free(library->books);
  /* Set library->books is temporary BOOK_T list */
  library->books = tmp;
  /* Double library->maxSize */
  library->maxSize *= 2;
}

/* Show detail of the book in library
 * [ Argument ]
 *    library - pointer to LIBRARY_T struct storing the books's data
 *    index - order of the book in library
 */
void libraryShowBookDetail(LIBRARY_T *library, int index) {
  char status[16];
  char borrowing[16];
  /* Check library->books[index].status  */
  if (library->books[index].status == 'A') {
    strcpy(status, "Available");
  } else {
    strcpy(status, "Borrowed");
  }
  /* Show detail of the book*/
  printf("\nTitle \t\t\t\t: %s\n", library->books[index].title);
  printf("Author Name \t\t: %s\n", library->books[index].author);
  printf("Publish Year \t\t: %d\n", library->books[index].year);
  printf("Status \t\t\t\t: %s\n", status);
  printf("Borrowing History \t:\n");
  /* Show status of book from lastest borrowing history */
  for (int i = library->books[index].history->currentSize - 1; i >= 0; i--) {
    /* Check status in borrowing history to show status  and borrowers's name */
    if (library->books[index].history->data[i].status == 'R') {
      strcpy(borrowing, "Return");
    } else {
      strcpy(borrowing, "Borrowed");
    }
    printf("\t- %s by %s\n", borrowing,
           library->books[index].history->data[i].name);
  }
  printf("\n");
}
/* Free the allocate memory for every calloc function in LIBRARY_T struct
 * [ Argument ]
 *    library - pointer to LIBRARY_T struct storing the books's data
 */
void libraryDestroy(LIBRARY_T *library) {
  /* Before free the memory save the data to file */
  SaveFile(FILE_PATH, library);
  /* reach the memory of data foreach book by looping with for-loop
   * by free the memory that is deepest variable in LIBRARY_T struct
   */
  for (int i = 0; i < library->currentSize; i++) {

    for (int j = 0; j < library->books[i].history->currentSize; j++) {
      free(library->books[i].history->data[j].name);
    }
    free(library->books[i].history->data);
    free(library->books[i].history);
    free(library->books[i].title);
    free(library->books[i].author);
  }
  free(library->books);
  free(library);
}
