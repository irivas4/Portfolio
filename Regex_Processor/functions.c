//irivas4
//Itel Rivas

#include "functions.h"

#include <stdio.h>
#include <string.h>

int read_file(FILE *fp, char words[][MAX_WORD_SIZE + 1], int size) {
  int wordCount = 0;
  int collected = 0;
  do {//puts in words to array
    collected = fscanf(fp, " %s", words[wordCount]);
    if (collected == 1) {
      wordCount++;
    }
  }while (collected == 1 && wordCount < size);
  return wordCount;//amount of words actually put in array ie. how many words there were in file with max being size passed in which is the amount of words we expect
}

int match(const char *regex, const char *word, int restriction) {
  char nextChar = regex[1];
  char currChar = regex[0];
  if (nextChar == '*') {
    if (word[0] == currChar) {
      // Let the '<char>*' match the initial '<char>' in the word
      if (match(regex, &(word[1]), restriction)){
       return 1;
     }
    }
    // Let the '<char>*' try and match nothing
      if (match(regex + 2, word, restriction)) {
      return 1;
    }
      if ((word[0] == '\0') && (regex[2] == '\0')) {
      return 1;
    }
  } else if (nextChar == '?') {
    if (word[0] == currChar) {
      // try letting the '<char>?' match the initial '<char>' in the word
      if (match(regex, &(word[1]), restriction)) {
        return 1;
      }
    }
    // Let the '<char>?' try and match nothing
    if (match(regex + 2, word, restriction)) {
      return 1;
    }
  } else if (currChar == '~') {
    if(nextChar == '~') {
      if(restriction >= (int)strlen(word)) { //I can automatically match it and dont have to check the next '~' to make sure its less than restriction times how many '~' there are
        return 1;
      }else {//if less than one restriction I'm going to check if its less than both combined
        return (match(regex + 1, &(word[restriction]), restriction));
      }
    } else if (nextChar == '\0') {
      if(restriction >= (int)strlen(word)) { //check to see if word is shorter than restriction if yes then it counts as a match
        return 1;
      }else {
        return 0;
      }
    }else {//protection for cases where the next char in regex must match with something in the word passed in
      if (word[1] == regex[1]) {
	if (match(regex, &(word[1]), restriction)) { //Let the '~' try and match initial '<char>' in the word
        return 1;
      }
    }
    if (match(regex + 1, word, restriction)) { //Let the '~' try and match nothing
      return 1;
    }
    }
  }else { //when currChar simply matches to char of word
      if (currChar == word[0]) {
	if (nextChar != '\0'|| word[1] != '\0'){ //to check if theres a next char in the word that must be checked
	  return match(regex + 1, &(word[1]), restriction);
	} else if (nextChar == '\0' && word[1] == '\0') { //Base case ie. if char in word matches char in regex and theres nothing left in either return 1
	  return 1;
	}
      }
  }
  return 0;
}
