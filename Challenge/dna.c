#include <stdio.h>
#define str_len 100
int main(){
	printf("Enter strand: ");
	char input[str_len];
	if(fgets(input,str_len,stdin)==NULL){
		printf("\n\nNo strand provided.\n");
	}else{
	int size = 0;
	while(input[size]!= '\0'){
		size++;
	}
	
	for(int i = 0; i < size-1;i++){
		// Translate the strand
		if(input[i] == 'a'){
			input[i] = 't';
		}else if(input[i] == 't'){
			input[i] = 'a';
		}else if (input[i] == 'A'){
			input[i] = 'T';
		}else if (input[i] == 'T'){
			input[i] = 'A';
		}else if (input[i] == 'c'){
			input[i] = 'g';
		}else if (input[i] == 'g'){
			input[i] = 'c';
		}else if (input[i] == 'C'){
			input[i] = 'G';
		}else if (input[i] == 'G'){
			input[i] = 'C';
		}else{
			input[i] = 'x';
		}
	}
	printf("\nComplementary strand is %s",input);
	}
}
