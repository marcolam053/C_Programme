#include <stdio.h>
#include <ctype.h>
#include <math.h>

int main(){
  float a = 0.00;
  float b = 0.00;
  float div_1 = 0.00;
  float div_2 = 0.00;

  printf("Enter two numbers: ");
  scanf("%f %f",&a,&b);
  printf("\n");
  if(a == 0 || b == 0){
    printf("Invalid input.\n");
    return 0;
  }
  if(b>a){
    div_1 = b/a;
    div_2 = (a+b)/b;
  }else{
    div_1 = a/b;
    div_2 = (a+b)/a;
  }
  div_1 = roundf(1000*div_1)/1000;
  div_2 = roundf(1000*div_2)/1000;
  if(div_1 == div_2){
    printf("Golden ratio!\n");
  }else{
    printf("Maybe next time.\n");
  }
}

