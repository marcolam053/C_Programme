#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
  char data[10] = "";
  printf("Enter height: ");
  scanf ("%s", data);

  for (int i = 0; i < strlen(data); i++) {
    if (!isdigit(data[i])) {
      printf("\nInvalid height.\n");
      return 0;
    }
  }


  int base = atol(data);
  int height = base * 2;

  if (base < 2 || base > 20) {
    printf("\nInvalid height.\n");
    return 0;
  }
	printf("\n");
  for (int i = (height - 1); i >= 0; i--) {
    for (int j = i; j > 0; j--) {
      printf(" ");
    }
    printf("/");

    if (i % base == 0 && i != 0) {
      for (int k = (i + 1); k < height; k++) {
        printf("__");
      }
    }else {
      if (i == 0) {
        for (int k = (i + 1); k < base; k++) {
          printf("__");
        }
        printf("\\");
        for (int k = (i - 1); k >= 0; k--) {
          printf("__");
        }
        printf("/");
        for (int k = (i + 1); k < base; k++) {
          printf("__");
        }
      }else if (i <= base - 1) {
        for (int k = (i + 1); k < base; k++) {
          printf("  ");
        }

        printf("\\");
        for (int k = (i - 1); k >= 0; k--) {
          printf("  ");
        }

        printf("/");
        for (int k = (i + 1); k < base; k++) {
          printf("  ");
        }

      }else {
        for (int k = (i + 1); k < height; k++) {
          printf("  ");
        }
      }
    }

    printf("\\");
    printf("\n");

  }

  return 0;
}

