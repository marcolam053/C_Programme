/**
 * comp2129 - assignment 1
 * <Chung Lai Lam>
 * <clam7738>
 */

#include <stdio.h>
#include <stdlib.h>

/** Properties
 * 1 = mine-ed cell
 * 2 = safe cell
 * 3 = flagged cell
 * 4 = disovered cell
 * hidden_grid --> grid for action and store
 * display_grid --> grid for printing.
 * single array is used instead of 2 d array for shorter runtime
 */

//declare global variables;
int mine_no = 10; // only ten mines is allowed
int gameover = 0;
char *display_grid; // The grid which will be displayed to the player.
int *hidden_grid; // The grid which store all the mine locations and actions.


// initiate the display grid.
char* init_display_grid(char *display_grid, int width, int height){
    for(int i = 0; i < height+2; i++){
        for (int j = 0; j < width+2;j++){
            if(i == 0 || i == height+1){
                if(j == 0 || j == width+1){
                    display_grid[i*(width+2)+j] = '+';
                    continue;
                }
                display_grid[i*(width+2)+j] = '-';
                continue;
            }
            if(j == 0 || j == width+1){
                display_grid[i*(width+2)+j] = '|';
                continue;
            }
            display_grid[i*(width+2)+j] = '*';
        }
        
    }
    return display_grid;
}

// initiate the hidden grid
int* init_hidden(int *hidden_grid,int width, int height){
    for(int i = 0; i < height+2; i++){
        for(int j = 0; j < width+2; j++){
            hidden_grid[width*i+j] = 2;
        }
    }
    return hidden_grid;
}

// funtion for displaying the display_grid
void print_grid(char *display_grid,int width, int height){
    for (int i = 0; i < height+2; i++){
        for (int j =0; j < width+2; j++){
            printf("%c",display_grid[(width+2)*i+j]);
        }
        printf("\n");
    }
}

void isError(){
    printf("error\n");
    exit(0);
}

void isLost(){
    printf("lost\n");
    exit(0);
}

// function to place mine into hidden_grid
void place_mine(int *hidden_grid, int width, int height){
    
    char firstLetter;
    char input[10];
    
    for(int i = 0; i < mine_no; i++){
        int width_mine, height_mine;
        
        
        fgets(input,10,stdin);
        
        // initiate a variable "Valid" for use in future checking purpose
        int Valid = sscanf(input,"%c %d %d", &firstLetter, &width_mine, &height_mine);
        
        // if the length of input is less than 3, print error
        if (Valid < 3){
            isError();
        }
        if(Valid > 3){
            isError();
        }
        
        // if the firstLetter of command is not "b" --> "error"
        if (firstLetter != 'b'){
            isError();
        }
        
        // if one of the dimensions is negative --> "error"
        if (width_mine < 0 || height_mine < 0){
            isError();
        }
        
        //if input out of boundaries
        if (width_mine > width-1 || height_mine > height-1){
            isError();
        }
        
        //check if there is duplicated input
        if(hidden_grid[width*height_mine+width_mine] == 1){
            isError();
        }
        
        //proceed if there is no error
        hidden_grid[width*height_mine+width_mine] = 1;
        
        //print out the command again
        printf("%s",input);
    }
}

// update discvered cell on both grid
int discover(char *display_grid,int *hidden_grid, int width, int height, int width_d, int height_d){
    
    // check for duplicated discover cell command
    if(hidden_grid[width*height_d + width_d] == 4){
        isError();
    }
    
    // check for duplicated flagged cell cell command
    if(hidden_grid[width*height_d + width_d] == 3){
        isError();
    }
    
    // if mine --> print lost
    if(hidden_grid[width*height_d + width_d] == 1){
        isLost();
    }
    
    //else -> safe ==> cont.
    
    int dis_mine = 0; // number of mine discovered
    
    
    //if safe => print number of adjacent mine
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            
            //check if it is out of boundaries
            if(!(height_d+i >0) || (height_d+i) > height - 1 || (width_d+i) < 0 || (width_d + i) > width - 1 ){
                continue;
            }
            
            // detect mine
            for(int x = 0; x< 4; x++){
                if(hidden_grid[width*(height_d+i) + (width_d+j)] <= 1+0.001){
                    dis_mine++;
                }
            }
            
        }
    }
    
    //update display_grid && hidden_grid
    hidden_grid[height_d*width + width_d] = 4;
    display_grid[(width+2)*(height_d+1)+width_d+1] = '0' + dis_mine ;
    return 1;
}

// update flagged cell in both grid
int flagged(char*display_grid, int*hidden_grid, int width, int height, int width_f, int height_f, int numFlag){
    
    // check if the cells is previously flagged
    if(hidden_grid[width*height_f + width_f] == 3){
        isError();
    }
    
    // check if the cells is previously discovered
    if( hidden_grid[width*height_f + width_f] == 4){
        isError();
    }
    
    // check if the number of planted flag is more than mine number
    if(numFlag > 10){
        isError();
    }
    
    // flag on mine
    // change status from 1 to 3
    // update display grid
    if( hidden_grid[width*height_f + width_f] == 1){
        hidden_grid[width*height_f + width_f] = 3;
        display_grid[(width+2)*(height_f+1) + width_f + 1] = 'f';
        return 1;
    }
    
    //else safe cell --> flagged
    //just update the grid.
    hidden_grid[width*height_f + width_f] = 3;
    display_grid[(width+2)*(height_f+1) + width_f + 1] = 'f';
    
    return 0;
}

// all action -- from flag uncover to determining if he win or not
void gameplay(char *display_grid, int *hidden_grid,int width, int height){
    
    int m; // My prefered name :)
    int l; // initial of my last name :)
    char firstLetter; // the command users input ( b, g , f,u)
    int u_Flag = 0;  // number of used flag
    int woohoo = 0; // safe zone discovered
    int bang = 0; // found mine
    char input[10];
    
    while(!gameover){
        
        // win
        if(woohoo + bang == width*height){
            printf("won\n");
            return;
        }
        
        fgets(input,10,stdin);
        int isValid = sscanf(input,"%c %d %d",&firstLetter, &m, &l);
        
        //check for valid input
        
        //check if the format is right or not
        if(isValid < 3){
            isError();
        }
        
        //check if it is out of boundary
        if(m < 0 || l < 0|| m > width - 1 || l > height - 1){
            isError();
        }
        
        // if flag command detected, do following
        // * number of used flag + 1
        // * if flagged on mined cells, add number of flag to discovered mine
        // * print updated display_grid
        
        if(firstLetter == 'f'){
            u_Flag++;
            bang = bang + flagged(display_grid,hidden_grid,width,height,m, l, u_Flag);
            printf("%s",input);
            print_grid(display_grid,width,height);
            continue;
        }
        
        // if command is "u",do following
        // * number of discovered safe cell + 1 if safe cell is discovered
        // * print out the command
        // * print out the updated grid
        
        if(firstLetter == 'u'){
            woohoo += discover(display_grid,hidden_grid,width,height,m,l);
            printf("%s",input);
            print_grid(display_grid,width,height);
            continue;
        }
        
        isError();
        
    }
}



int main(){
    
    int width,height;
    char firstLetter;
    char input[10];
    
    fgets(input,10,stdin);
    int isValid = sscanf(input,"%c %d %d", &firstLetter, &width, &height);
    
    //check for invalid input
    if(firstLetter != 'g'){
        isError();
    }
    
    //check if the input length is less than 3
    if(isValid < 3){
        isError();
    }
    
    // check if the input is out of bound or not.
    if(width*height < 10 || width > 100 || height > 100){
        isError();
    }
    
    //check if it is negative
    if(width < 0 || height < 0){
        isError();
    }
    
    // print out the command again if Valid
    printf("%s",input);
    //printf("\n");
    
    //allocate memory for the display_grid & hidden_grid
    display_grid = (char*) malloc( ((width+2)*(height+2)+1)*sizeof(char) );
    hidden_grid = (int*) malloc( ((width+2)*(height+2)+1)*sizeof(int) );
    
    // initiate the display_grid && hidden_grid
    display_grid = init_display_grid(display_grid,width,height);
    hidden_grid = init_hidden(hidden_grid,width,height);
    
    // Place mine
    place_mine(hidden_grid,width,height);
    
    // print display_grid
    print_grid(display_grid,width,height);
    
    //player action - flag, uncover,win,lose
    gameplay(display_grid,hidden_grid,width,height);
    
    // Free the D. A. M for display and hidden grid after finishing programme
    free((void*)display_grid);
    free((void*)hidden_grid);
    
    return 0;
}
