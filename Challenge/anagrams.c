#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_Ana(char *word1, char *word2);
int main(){
	char word1[256];
	char word2[256];
	int size = 0;
	int size2=0;
	int p =0;
	int k =0;
	printf("Enter line: ");
  fgets(word1,sizeof(word1),stdin);
	while(word1[size] != '\0'){size++;}

	for(int i =0; i < size-1;i++){
		if(!ispunct(word1[i])){
			word1[p] = tolower(word1[p]);
			p++;
		}
	}

	printf("Enter anagram: ");
	fgets(word2,sizeof(word2),stdin);
	while(word2[size2] != '\0'){size2++;}
	for(int i =0; i < size-1;i++){
		if(!ispunct(word2[i]))
		word2[k] = tolower(word2[k]);
		k++;
	}

	if(is_Ana(word1,word2) == 0){
		printf("\nAnagram!\n");
	}else{
		printf("\nNot an anagram.\n");
	}

}

int is_Ana(char word1[], char word2[]){

int alpha1[26] = {0}, alpha2[26] = {0}, i = 0;

    while (word1[i] != '\0')
    {
        alpha1[word1[i] - 'a']++;
        i++;
    }
    i = 0;
    while (word2[i] != '\0')
    {
        alpha2[word2[i] -'a']++;
        i++;
    }
    for (i = 0; i < 26; i++)
    {
        if (alpha1[i] != alpha2[i])
            return 1;
    }
    return 0;

}

