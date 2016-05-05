
#include <stdio.h>
int main(void){

	printf("Guidance is internal.\n");

	for(int i = 12; i >0; i--){
			printf("%d...\n", i);
  		if(i ==7){
			printf("Ignition sequence start.\n");
	}
		if(i == 1){
			printf("All engine running.\n");
			printf("Lift off on Apollo 11!\n");
		}

  }
}
