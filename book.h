#include "library.h"
#ifndef BOOK_H
#define BOOK_H

/* -------------------[ Public Borrow Function ]-------------------  */

BORROW_T *borrowHistoryCreate();

BORROW_HISTORY_T *borrowHistorylistCreate(int initialCapacity);

void borrowHistorylistAppend(BORROW_HISTORY_T *library, BORROW_T *value);

void borrowHistorylistExpand(BORROW_HISTORY_T *library);

/* -------------------[ Public Book Function ]-------------------  */

BOOK_T *bookCreate();

void AddBook(LIBRARY_T *library);

void RemoveBook(LIBRARY_T *library);

void EditBook(LIBRARY_T *library);

void SearchBook(LIBRARY_T *library);

void ReturnBook(LIBRARY_T *library);

void BorrowBook(LIBRARY_T *library);

#endif