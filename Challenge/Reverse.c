#include <stdio.h>
#include <string.h>

//global variables
char original[50];
char reverse[50];


void revstring(char original[], char reverse[]) {
	int i = 0;
	int end = strlen(original) - 1;
	reverse[end] = '\0';

	while (original[i] != '\0') {
		reverse[end-1 - i] = original[i];
		++i;
	}
}

int main()
{

	while(fgets(original, sizeof(original), stdin) != 0){
		revstring(original, reverse);

		printf("%s", reverse);
    printf("\n");
	}
   return 0;
}
