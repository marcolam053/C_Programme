#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

size_t size;
char* first;
char* second;
char** input;
int num_input;

void init_pair(char* arg[]){
  first = malloc(strlen(arg[1])*sizeof(char));
  for (size_t i = 0; i < strlen(arg[1]); i++) {
    first[i] = arg[1][i];
  }
  second = malloc(strlen(arg[2])*sizeof(char));
  for (size_t i = 0; i < strlen(arg[2]); i++) {
    second[i] = arg[2][i];
  }
}

int check_arg (char* arg[]){
  size_t arg_a = strlen(arg[1]);
  size_t arg_b = strlen(arg[2]);
  return (arg_a == arg_b);
}

int check(int argc, char* arg[]){
  if(argc == 1){
    printf("No arguments\n");
  }
  if(argc == 2){
    printf("Not enough arguments\n");
  }
  if(argc > 3){
    printf("Too many arguments\n");
  }
  if(argc == 3){
    return 1;
  }
    return 0;
}

void command(int index,size_t len, char* ln){
  for (size_t i = 0; i < len; i++) {
    input[index][i] = ln[i];
  }
}

void trans(size_t len){

  for (size_t p = 0; p < num_input; p++) {

    for (size_t i = 0;input[p][i] != '\0'; i++) {
      for (size_t j = 0; j < len; j++) {
        if(input [p][i] == first[j]){
          input[p][i] = second[j];
          break;
        }
      }
    }
  }
}

int main (int argc, char** arg){

  if(check(argc,arg) == 0){
    return 0;
  }else{
    if(check_arg(arg) == 1){
      init_pair(arg);
      size = strlen(arg[2]);
      input = (char**)malloc(100*sizeof(char*));
      int count = 0;
      char tr[100];
      while (fgets(tr, sizeof(tr), stdin)!= NULL) {
        size_t len = strlen(tr);

        tr[len-1] = '\0';
        len = strlen(tr);

        input[count] = calloc(len+1,sizeof(char));
        command(count,len,tr);
        count++;
      }
      num_input = count;
      trans(size);

      for (size_t i = 0; i < num_input; i++) {
          printf("%s\n", input[i]);
      }

      // free memory
      for (size_t i = 0; i < num_input; i++) {
        free(input[i]);
      }
      free(input);
      free(first);
      free(second);
    }else{
      printf("Invalid arguments\n");
      return 0;
    }
  }
  return 0;
}

