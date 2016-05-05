#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(){

	char input;


	while((input = getchar())!= EOF){
		putchar(toupper(input));
	}
	return 0;
}
