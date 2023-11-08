//irivas4
//Itel Rivas

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"


#define MAX_WORD_NUM 1024
#define MAX_INPUT_SIZE 128
#define DEFAULT_RESTRICTION 10

int main(int argc, const char *argv[]) {
  if(argc < 2) {//A check to make sure file was passed in
    fprintf(stderr, "Error, no input file passed in.\n");
    return 1;
  }
  int restriction = DEFAULT_RESTRICTION;
  if (argc == 3) {//A check to see if a restriction was passed in and if its valid ie. not negative
    if (atoi(argv[2]) < 1 ) {
      fprintf(stderr, "Invalid restriction length\n");
      return 2;
    }
    restriction = atoi(argv[2]);
  }

  FILE *input = fopen(argv[1], "r");//Open file from command line argument
  if (input == NULL) {
    printf("Error: could not open input file\n");
    return 1; // indicate error
  }
 
  char words[ MAX_WORD_NUM ][ MAX_WORD_SIZE + 1 ];
  int numWords = 0;//Will be updated to number of words that should be in the file/ the amount of words we'll put in array
  
  int collected = fscanf(input, "%d", &numWords);
  if(collected != 1) {//Makes sure first line actually contained number of words to expect
    fprintf(stderr, "First line of file does not contain valid number of words\n");
    return 3;
  }
  collected = read_file(input, words, numWords);
  if(collected != numWords) {//Check to make sure there weren't fewer words than expected, if there were more than expected we ignore them
    fprintf(stderr, "There are fewer words than specified by the number in the first line of the file.\n");
  return 4;
  }
  char pattern[MAX_INPUT_SIZE];//WIll be updated with regex 
  int parse = scanf(" %s", pattern);
  while(parse != -1){//parse will be -1 when user tries to terminate program using ctrl+d
    int matches = 0; //How many matches there were for that regex, if 0 we let user no they're werent any matches
    for(int wordNum = 0; wordNum < numWords; wordNum++) {
      int holder = 0;//Var to check if its a match in order to print out word, also if match is added to total match count var 'matches'
      holder = match(pattern, words[wordNum], restriction);
      if(holder == 1) {
	printf("%s\n", words[wordNum]);
	matches++;
      }
    }
    if(matches < 1) {
      printf("No match found for regex '%s'\n", pattern);
    }
    parse = scanf(" %s", pattern);
  }
  fclose(input);//close file from command line argument
}
