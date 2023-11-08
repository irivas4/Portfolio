#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "puzzle.h"



int handle_C_command_File(FILE *in, Puzzle **p){ // when were reading in commands from a file
  int num; // variable to hold given row/column number of puzzle
  if (fscanf(in," %d", &num) != 1) { // tries to scan in an integer value
    fprintf(stderr, "Invalid input\n"); // error: did not scan in an int value
    return 1;
  }
  if (num < 2 || num > 20) { // checks to see if the number is in the valid range
    fprintf(stderr, "Invalid puzzle size\n"); // error: row/column size not valid
    return 1;
  }
  Puzzle *ptr; // creates a pointer to a puzzle grid
  ptr = puzzle_create(num); // should return a pointer and because Puzzle is a pointer to a pointer (want the address of the pointer returned)
  if (ptr == NULL) { // check to make sure puzzle was created
    fprintf(stderr, "Could not create puzzle\n");
    return 1;
  }
  (*p) = ptr; // assigns the newly created puzzle to the parameter puzzle's pointer address
  return 0;
}
          
int handle_T_command_File(FILE *in, Puzzle *p){ // when we are reading in commands from the buffer
  int size = p->rcNum; // row/column number of the puzzle grid
  int collected = 0; // makes sure there is a tile to be collected and stored in grid
  int currPos = 0; // current position in the puzzle grid
  int tileNum = 0; // the tile number
  int numGaps = 0; // By the end of scanning in the values theres should be exactly one gap
  do {
    collected = fscanf(in, " %d", &tileNum); // sets given tile number to tileNum and collected to 1 (assuming value is scanned in)
    if (collected == 1) {
      if ((tileNum >= 0) && (tileNum <= (size * size))) { // makes sure given tile number is valid (between 0 and size^2 - 1) 
	p->pos[currPos] = tileNum; // sets current tile to the tile number
	currPos++; // updates current position
      } else { // invalid tile
	fprintf(stderr, "Invalid tile value\n");
	return 1;
      }
    }
  } while (collected == 1 && currPos <= (size * size)); // do for each posititon in grid unless there is no tile to be collected

  if (currPos != ((size * size))) { // either too many or too few tile inputs
    fprintf(stderr, "Invalid input\n");
    return 1;
  }
  for (int i = 0; i < (size * size); i++) { // checks for excess gap positions
    if((p->pos[i]) == 0) { // gap position
      numGaps++;
      p->gapPos = i; // declares the gap position of a puzzle
    }
  }
  if (numGaps != 1) { // either too many or no gap positions
    fprintf(stderr, "Invalid input\n");
    return 1;
  }
  return 0;
}

Puzzle *puzzle_create(int size){ // creates and returns a pointer to a puzzle grid
  Puzzle *puzzle = (Puzzle*)malloc(sizeof(Puzzle)); // has to be freed later, replace comment when freed implemented
  if (puzzle == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }
  
  puzzle->rcNum = size; // sets 
  puzzle->pos = (int*)malloc(sizeof(int) * size * size); // dynamic array thats part of struct. has to be freed later, replace comment when freed implemented
  if (puzzle->pos == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }
  for (int i = 0; i < (size * size); i++) { // Initialize each position with gap value (0)
    puzzle->pos[i] = 0;
  }
  return puzzle;
}

int puzzle_move_tile(Puzzle *p, char move, int gap_position){
  switch (move) {
  case 'd':
    if ((gap_position/(p->rcNum)) == 0) { //if its something in the first row i cant move anything down to switch with gap
      return 1;
    }
    p->pos[gap_position] = p->pos[gap_position - p->rcNum];
    p->pos[gap_position - p->rcNum] = 0;
    p->gapPos = gap_position - p->rcNum;
    return 0;
  case 'u':
    if ((gap_position/p->rcNum) == (p->rcNum - 1)) { //if its something in the last row i cant move anything up to switch with gap
      return 1;
    }
    p->pos[gap_position] = p->pos[gap_position + p->rcNum];
    p->pos[gap_position + p->rcNum] = 0;
    p->gapPos = gap_position + p->rcNum;
    return 0;
  case 'l':
    if (gap_position % (p->rcNum) == (p->rcNum - 1)) { //if its something in the first column i cant move anything to the right to switch with gap
      return 1;
    }
    p->pos[gap_position] = p->pos[gap_position + 1];
    p->pos[gap_position + 1] = 0;
    p->gapPos = gap_position + 1;
    return 0;
  case 'r':
    if (gap_position % (p->rcNum) == 0) { //if its something in the last column i cant move anything to the left to switch with gap
      return 1;
    }
    p->pos[gap_position] = p->pos[gap_position - 1];
    p->pos[gap_position - 1] = 0;
    p->gapPos = gap_position - 1;
    return 0;
  default:
    fprintf(stderr, "Invalid input\n"); //not l, r, u, or d
    return 2;
  }
}

int puzzle_find_gap(const Puzzle *p){ //helper function for S whih locates gap pos.
  return p->gapPos;
}

void puzzle_print_positions(Puzzle *p){ //Iterates through postion array and prints out current status of puzzle
  for (int i = 0; i < p->rcNum * p->rcNum; i++) {
    printf("%d ", p->pos[i]);
  }
  printf("\n");
}

int handle_K_Command(const Puzzle *p) {
  int win = 0;
  for (int i = 0; i < ((p->rcNum) * (p->rcNum) - 1); i++) { //makes sure all positions are right exccept lower right corner which should be 0
    if (p->pos[i] == (i+1)) {
      win++;
    }
  }
  if (((p->rcNum) * (p->rcNum) - 1) == 0) { //Checks lower right corner is 0
    win++;
  }
  if (win == ((p->rcNum) * (p->rcNum) - 1)){ //if win doesn't match N^2 its not in solved status
    return 1;
  } else {
    return 0;//print statements are in main so we can reuse K for V command
  }
}
  
int handle_W_Command(Image * im, Puzzle * p, const char image[], const char file[]){
  if(((im->rows % p->rcNum) != 0) ||((im->cols % p->rcNum) != 0)) {
    fprintf(stderr, "Invalid image dimensions");
    return 1; //added in closes in case of failure before all returns
  }

   FILE* writeFile = fopen(file, "w");
  if (writeFile == NULL) {
    fprintf(stderr, "Could not open output puzzle file '%s'", file);
    return 1;
  }

  int written = 0; //This prints out to txt file given
  int rowCol = (p->rcNum);
  for (int i = 0; i < (rowCol * rowCol); i++) {
    written += fprintf(writeFile, "%d ", p->pos[i]);
  }
  if (written <= 0) {
    fprintf(stderr, "Could not write puzzle data '%s'", file);
    return 1;
  }

  fclose(writeFile);
  
  FILE* writeIm = fopen(image, "wb");
  if (writeIm == NULL) {
    fprintf(stderr, "Could not open output image file '%s'", image);
    return 1;
  }
  
  Image *copy = malloc(sizeof(Image));
  if (copy == NULL) {
    fclose(writeIm);
    return 1;
  }
  copy->rows = im->rows;
  copy->cols = im->cols;
  copy->data = malloc(sizeof(Pixel) * (im->rows) * (im->cols));
  //read in array from puzzle and then copy data as such and then write out
  int squares_Copied = 0;
  while(squares_Copied < (rowCol * rowCol)) {
    int startCopy = start(rowCol, im->rows, im->cols, squares_Copied);//start index of copy 
    int startOg = start(rowCol, im->rows, im->cols, (p->pos[squares_Copied] - 1));//start index of original that will be copied into copy
    int oneRow = ((im->rows) * (im->cols) / rowCol);
    for (int x = 0; x < oneRow; x+=im->rows) {//have to increment by im-> rows becauses its a 1D dynamic array
      for (int y = 0; y < ((im->cols)/rowCol); y++) {
	if ((p->pos[squares_Copied]) == 0) { //set gap to all black
	  copy->data[(startCopy + x + y)].r = 0;
	  copy->data[(startCopy + x + y)].g = 0;
	  copy->data[(startCopy + x + y)].b = 0;
	} else { //copy pixels from original image and cirresponding sub square
	  copy->data[(startCopy + x + y)] = im->data[(startOg + x + y)];
	}
      }
    }
    squares_Copied++;
  }
  int check = WritePPM(writeIm, copy); //write in binary
  if (check == -1) {
    fclose(writeIm);
    free(copy->data);
    free(copy);
    return 1;
  }
  fclose(writeIm);
  free(copy->data);
  free(copy);
  return 0;
}


int start(int rcNum, int rows, int cols, int ind) {
  int x = (ind / rcNum) * (rows * cols) / rcNum; //first chunk tells what row its in, 0 means 1st row, 1 2nd row and so on;
  int y = (ind % rcNum) * (cols / rcNum); // first chunk tells what column its in, a remainder of 0 means 1st, 1 means 2nd and so on;
  return (x + y);
}

Puzzle* create_Copy(Puzzle *p){
  Puzzle * copy;
  int size = p->rcNum;
  copy = puzzle_create(size);
  copy->pos = (int*)malloc(sizeof(int) * size * size); //dynamic array thats part of struct. has to be freed later, replace comment when freed implemented                                                
  if (copy->pos == NULL) {
    printf("Memory allocation failed.\n");
    free(copy);
    return NULL;
  }
  for (int i = 0; i < ((p->rcNum) * (p->rcNum)); i++) { //copy down current status of puzzle 
    copy->pos[i] = p->pos[i];
    if (p->pos[i] == 0) {
      copy->gapPos = i;
    }
  }
  return copy;
}

int solve_Puzzle(Puzzle *p, char steps[], int max_Steps, int curr_Steps, char last_Move) {
  if (handle_K_Command(p)) {
    return curr_Steps;
  } else if (curr_Steps >= max_Steps) {
    return -1;
  }
  
  for (int m = 0; m < 4; m++) {
    char move;
    if (m == 0) {move = 'u';}
    if (m == 1) {move = 'l';}
    if (m == 2) {move = 'd';}
    if (m == 3) {move = 'r';}

    // meant to skip over opposite commands like i dont want to do l and then try r as it undoes what i just did. 
    if (((move == 'u' && last_Move == 'd') ||
        (move == 'd' && last_Move == 'u') || 
        (move == 'l' && last_Move == 'r') || 
	 (move == 'r' && last_Move == 'l')) && (curr_Steps != 0)) { //in order to not have two functions ill call command for first time with r as last step but this and statement helps to  ignore it
      continue;
    }

    Puzzle *copy = create_Copy(p);
    if (copy == NULL) {
      return -1;
    }
    if (puzzle_move_tile(copy, move, copy->gapPos) == 0) { //if move was successful
      int result = solve_Puzzle(copy, steps, max_Steps, (curr_Steps + 1), move); //recrusively call on function
      if (result != -1) { //I need all these frees in order to make sure that no matter what i free any memory i allocated when creating copies
        steps[curr_Steps] = move;
        free(p->pos);
        p->pos = copy->pos;
        free(copy);
        return result;
      } else {
        free(copy->pos);
        free(copy);
      }
    } else {
      free(copy->pos);
      free(copy);
    }
  }
  return -1;
}
  
