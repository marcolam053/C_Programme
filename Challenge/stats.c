#include<stdio.h>
#include <math.h>
#include<string.h>
#include <stdbool.h>

int main(void){

  float input[50];
  float data = 0;
  int size = 0;
  float mean = 0.0000;
  float var = 0.0000;
  float sd = 0.0000;

  printf("Enter data set:\n");

  while(scanf("%f",&data)!=EOF){
    input[size]=data;
    size++;
  }
  printf("\n");

  if(size == 0){
    printf("No data!\n");
  }else {

    // Mean
    for(int i = 0; i < size; i++){
      mean = mean+input[i];
    }
    mean = mean/size;

    // Variance
    for(int i = 0; i < size; i++){
      var = var + (input[i]-mean)*(input[i]-mean);
    }
    var = var/size;

    sd = sqrt(var);

    printf("Mean = %.4f\n",mean);
    printf("Variance = %.4f\n",var);
    printf("Standard deviation = %.4f\n",sd);
  }

  return 0;
}

