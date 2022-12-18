#include "book.h"
#include "library.h"
#include <stdio.h>

int main(void) {
  printf("Welcome to CPE Library Management System\n");
  LIBRARY_T *Library = createlibraryFromFile(FILE_PATH);
  SelectMenu(Library);
  printf("Thank you");
  return 0;
}
