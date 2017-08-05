#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//// find the Bionomial coefficient
//int pascal(int n, int r){
//    int result = 1;
//    if(r > n-r){
//        r = n-r;
//    }
//    for( int i = 0; i < r; ++i){
//        result =result*(n-i);
//        result = result/(i+1);
//    }
//    return result;
//}

int main(int argc, char **argv){
    // Check the number of Argument
    if(argc < 2){
        printf("Missing Argument\n");
        exit(1);
    }
    // Check the value of argument
    int line = atoi(argv[1]);
    if(line < 0){
        printf("Invalid Argument\n");
        exit(1);
    }

    if( line == 1){
        printf("1\n");
        return 0;
    }
    // print the pascal triangle
    for(int row = 0; row < line; row++) {
        int c = 1;
        for (int i = 0; i < row; i++) {
            printf("%d ", c);
            c = c*(row-i)/i;
        }
        printf("\n");
    }
    return 0;
}