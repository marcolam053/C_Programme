  #include <stdio.h>
  #include <math.h>

  int main(){
      char input[200];

      printf("Enter binary: ");
      fgets(input,200,stdin);

      int size = 0;

      while(input[size] != '\0'){
		  size++;
	  }
	  
      int integer[size-1];
      for(int i = 0; i < size-1; i++){
        integer[i]=input[i]-'0';
      }

      for(int j = 0; j < size-1;j++){
        if(integer[j]!=0 && integer[j] != 1){
          printf("\nNot binary!\n");
          return 0;
        }
      }

      int power[size-1];
      int count = size-2;
      for(int k = 0; k<size-1;k++){
        power[k] = pow(2,count);
        count--;
      }

      int decimal[size-1];
      for(int x = 0; x < size-1;x++){
        decimal[x] = integer[x]*power[x];
      }

      int sum = 0;
      for(int h = 0; h < size-1; h++){
        sum+=decimal[h];
      }

      printf("\n");
      printf("%d in decimal\n",sum);
      return 0;
  }

