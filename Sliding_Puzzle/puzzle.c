#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"
#include "puzzle.h"

int main(int argc, char **argv) {
  if (argc < 1 || argc > 2) {
    fprintf(stderr, "Usage: ./puzzle [<command file>]\n");
    return 1;
  }
  
  char command = '\0';
  int created = 0; //For C command
  int collected; // To check how many things fscanf read in
  int imgLoaded = 0; //For I commanded, updated once image is loaded in
  char img[256]; //Name of image file passed in as part of I command 
  Image *image; //Pointer to image passed in for I
  FILE *fileIm; //Pointer to Image file from I command
  Puzzle *puzz = NULL; //I do this so when I actually call C command it'll work otherwise were not allowed to acces memory we dont own and we get an error leading to a seg fault 
  Puzzle **puzzle = &puzz; //ptr to a ptr to a instance of puzzle struct
  FILE *in; //file were reading in commands from or stdin depending on number of arguments from command line
  
  if (argc == 2) { //checks to see if were reading in from a file or from stdin
    in = fopen(argv[1], "r"); // there is a file to be opened
  } else {
    in = stdin; // no file
  }
  
  while (fscanf(in, " %c", &command) == 1) {
      
    switch (command) { //I decided to put {} brackets around everything to prevent confusion for the compiler or us 
      case 'C':
	 {if ((handle_C_command_File(in, puzzle)) == 1){ // if it returns one that means there is an error
	  return 1;
	 }
	 created = 1; //this helps us keep track whether the puzzle was even created for commands dependent on it being created  
         break;}
      case 'T':
	 {if (created == 0) { //Every command dependent on puzzle being created will have this, checks to see if puzzle was created
	    fprintf(stderr, "No puzzle\n");
	    return 1;
	  }
	  if (handle_T_command_File(in, *puzzle) == 1) { //errors already printed out in helper func so we just have to return 1, similar for other commands
	    return 1;
	  }
	  break;}
        case 'I':
	  {int collected = fscanf(in, " %s", img);
	  if (collected != 1) {
	    fprintf(stderr, "Invalid input\n");
	    return 1;
	  }
	  fileIm = fopen(img, "rb");
	  if (fileIm == NULL) {
	    fprintf(stderr, "Could not open image file '%s'\n", img);
	    return 1;
	  }
	  image = ReadPPM(fileIm);
	  assert(image);
	  imgLoaded = 1; //this helps us keep track whether the image was read in for commands dependent on it being loaded in mainly W 
	  break;}
        case 'P':
	  {if (created == 0) {
	    fprintf(stderr, "No puzzle\n");
	    return 1;
	  }
	  puzzle_print_positions(*puzzle);
	  break;}
        case 'W':
	  {if (created == 0) {
	    fprintf(stderr, "No puzzle\n");
	    return 1;
	  }
	  if (imgLoaded != 1) {
	    fprintf(stderr, "No image\n");
	    return 1;
	  }
	  char outFile[256]; //max length for both is 255 plus null terminator
	  char outIm[256];
	  collected = fscanf(in, " %s %s", outIm, outFile);
	  if (collected != 2) {
	    fprintf(stderr, "Invalid input\n");
            return 1;
          }
	  int check = handle_W_Command(image, *puzzle, outIm, outFile);
	  if (check == 1) {
	    return 1;
	  }
	  break;}
        case 'S':
	  {int gap = puzzle_find_gap(*puzzle); // finds gap position of given puzzle
	  char move = '\0';
	  fscanf(in, " %c", &move); // scans for move
	  int result = puzzle_move_tile(*puzzle, move, gap); // equals 1 if move cannot be performed
	  if (result == 1) {
	    fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
	    return 1;
	  } else if (result == 2) { // not a valid move (u d l r)
	    fprintf(stderr, "Invalid input\n");
	    return 1;
	  }
	  break;}
        case 'K':
	  {if (created == 0) {
	    fprintf(stderr, "No puzzle\n");
	    return 1;
	  }
	  if(handle_K_Command(*puzzle)) { // returns 1 if puzzle is solved 
	    printf("Solved\n");
	  }else {
	    printf("Not solved\n");
	  }
	  break;}
        case 'V':
	  {if (created == 0) {
	    fprintf(stderr, "No puzzle\n");
	    return 1;
	  }
	  int max = (*puzzle)->rcNum;
	  max *= (max * max * 2); //max dependent on size of puzzle
	  char steps[max];
	  int solved = solve_Puzzle((*puzzle), steps, max, 0, 'r'); //Even though there was no last move i use r as a holder but my helper function takes this into account and ignores it
	  if (solved != -1) {
	    for (int i = 0; i < solved; i++) { //prints out steps used to find solution
	      printf("S %c\n", steps[i]);
	    }
	  } else {
	    printf("No solution found");
	  }
	  break;}
        case 'Q':
	  {if (created == 1) { //I dont want to free stuff I never allocated so i check to see if things were allocated in the C and I command and then free if those commands were executed
	    free((*puzzle)->pos);
	    free(*puzzle);
	  }
	  if (imgLoaded == 1) {
	    free(image->data);
	    free(image);
	    fclose(fileIm);
	  }
	  return 0;
	  break;}
        default:
	  fprintf(stderr, "Invalid command '%c'\n", command);
	  return 1;
      }
   } //in case EOF is reached i still want anything I allocated to be free but only if i allocated so it checks similar to the Q command
   if (created == 1) {
     free((*puzzle)->pos);
     free(*puzzle);
   }
   if (imgLoaded == 1) {
     free(image->data);
     free(image);
     fclose(fileIm);
   }
  return 0;
}
