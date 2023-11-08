#include <assert.h>
#include <stdio.h>

#include "functions.h"

#define TEST_RESTRICTION_1 10
#define TEST_RESTRICTION_2 2
#define TEST_RESTRICTION_3 5
#define TEST_RESTRICTION_4 30

#define MAX_WORD_NUM 1024

void test_read_files() {
  FILE* fptr = fopen("test_file_1.txt", "w");//Test to make sure that if theres the correct amount words, they're all scanned in
  fprintf(fptr, "5\nintermediate\ninttterrmeediateeee\nprogram\nppproograaamm\naaa");
  fclose(fptr);
  FILE* input = fopen("test_file_1.txt", "r");
  char words[ MAX_WORD_NUM ][ MAX_WORD_SIZE + 1 ];
  int numWords = 0;
  int collected = fscanf(input, "%d", &numWords);
  assert(collected == 1);
  collected = read_file(input, words, numWords);
  assert(collected == 5);
  fclose(input);


  FILE* fptr2 = fopen("test_file_2.txt", "w"); //Test to make sure that if theres more words in the file our function tells us how many it actually scanned in so we can tell user
  fprintf(fptr2, "5\nintermediate\ninttterrmeediateeee\nprogram\n");
  fclose(fptr2);
  FILE*	input2 = fopen("test_file_2.txt", "r");
  char words2[ MAX_WORD_NUM ][ MAX_WORD_SIZE + 1 ];
  int numWords2 = 0;
  int collected2 = fscanf(input2, "%d", &numWords2);
  assert(collected2 == 1);
  collected2 = read_file(input2, words2, numWords2);
  assert(collected2 != 5);
  fclose(input2);

  
  FILE* fptr3 = fopen("test_file_3.txt", "w"); //Test to make sure that if theres less words in the file our function still only scans in the amount specified by the first line 
  fprintf(fptr3, "5\nintermediate\ninttterrmeediateeee\nprogram\nppproograaamm\naaa\ncat");
  fclose(fptr3);
  FILE* input3 = fopen("test_file_3.txt", "r");
  char words3[ MAX_WORD_NUM ][ MAX_WORD_SIZE + 1 ];
  int numWords3 = 0;
  int collected3 = fscanf(input3, "%d", &numWords3);
  assert(collected3 == 1);
  collected3 = read_file(input3, words3, numWords3);
  assert(collected3 == 5);
  fclose(input3);
}

void test_match_regex() {
  assert(match("abc", "abc", TEST_RESTRICTION_1) == 1);
  assert(match("abcd", "abc", TEST_RESTRICTION_1) == 0);
  assert(match("", "", TEST_RESTRICTION_1) == 1);
  assert(match("ABC", "ABC", TEST_RESTRICTION_1) == 1);//Test for Upper cases
  assert(match("AbCdEf", "AbCdEf", TEST_RESTRICTION_1) == 1);//Test for combo of upper and lower case
  assert(match("!@#", "!@#", TEST_RESTRICTION_1) == 1);//Test for specual chars not used for regex pattern
  assert(match("157", "157", TEST_RESTRICTION_1) == 1); //Test for numbers
  assert(match("wrdvc345%@aaAA", "wrdvc345%@aaAA",TEST_RESTRICTION_1) == 1);//Test for mixed "word"                                                                                                        
}

void test_match_regex_star() {
  assert(match("abcd*", "abc", TEST_RESTRICTION_1) == 1);
  assert(match("abc*d", "abcccccccd", TEST_RESTRICTION_1) == 1);
  assert(match("abc*de", "abcccccccd", TEST_RESTRICTION_1) == 0);
  assert(match("i*n*t*t*t*e*r*r*m*e*e*d*i*a*t*e*e*e*e*", "inttterrmeediateeee",
	       TEST_RESTRICTION_1) == 1);//Test where every char should only count for one
  assert(match("a*", "", TEST_RESTRICTION_1) == 1); //Test to make sure if passed in empty string as word it'll match with a regex that doesnt actually match anything(edge case)
  assert(match("1233*4", "1234", TEST_RESTRICTION_1) == 1);//Test for numbers   
  assert(match("ABC*", "ABC", TEST_RESTRICTION_1) == 1);//Test for Upper cases  
  
}

void test_match_regex_question() {
  assert(match("a?", "", TEST_RESTRICTION_1) == 1);
  assert(match("a?", "b", TEST_RESTRICTION_1) == 0);
  assert(match("a?", "a", TEST_RESTRICTION_1) == 1);
  assert(match("a?aa", "aaa", TEST_RESTRICTION_1) == 1); //Test to make sure it'll match the one
  assert(match("a?a?aa", "aaa", TEST_RESTRICTION_1) == 1); //Test to make sure it'll match only one and no more
  assert(match("1233?4", "12334", TEST_RESTRICTION_1) == 1);//Test for numbers
  assert(match("ABC?", "ABC", TEST_RESTRICTION_1) == 1);//Test for Upper cases
  
}

void test_match_regex_tilde() {
  assert(match("~", "aaaa", TEST_RESTRICTION_1) == 1);
  assert(match("~", "", TEST_RESTRICTION_1) == 1);
  assert(match("~", "a", TEST_RESTRICTION_1) == 1);
  assert(match("~", "inttterrmeediateeee", TEST_RESTRICTION_1) == 0); //Doesnt match word longer than restriction
  assert(match("~", "inttterrme", TEST_RESTRICTION_1) == 1); //Matches word as long as restriction
  assert(match("~~", "a", TEST_RESTRICTION_1) == 1);
  assert(match("~a", "a", TEST_RESTRICTION_1) == 1);
  
}

void test_match_regex_multiple() {
  assert(match("!2*keK?ee?rCC?C*aP?E*eR*T*", "!2222keKerCCCaeRRRT",
               TEST_RESTRICTION_1) == 1);
  assert(match("!~2*keK?ee?rCC?C*aP?E*eR*T*", "!2222keKerCCCaeRRRT",
               TEST_RESTRICTION_1) == 1);
  assert(match("~abc", "sda12!$2", TEST_RESTRICTION_1) == 0);
  assert(match("~ab*", "aaa", TEST_RESTRICTION_1) == 1);
  assert(match("a?a*a?a*A?A*A?A*aaa", "aaa", TEST_RESTRICTION_1) == 1);
  assert(match("ina?ttt?t*e*r*ms?e*diate*", "intermediate", TEST_RESTRICTION_1) == 0);
  assert(match("a?a*", "", TEST_RESTRICTION_1) == 1); //Test to make sure if passed in empty string as word it'll match with a regex that doesnt actually match anything
  assert(match("ina?ttt?t*e*r*ms?e*diate*", "inttterrmeediateeee", TEST_RESTRICTION_1) == 1);
  assert(match("Mr.C?a?t?Rivas-D*o*g*Salazar", "Mr.Rivas-Salazar", TEST_RESTRICTION_1) == 1); //Test to make sure word inside another word wont affect match
  
}

void test_match_regex_tilde_restriction() {
  assert(match("~~", "aaaa", TEST_RESTRICTION_2) == 1); //Test for edge case to make sure match correctly matches as ~~ would match exaclty the 4 letters in "aaaa"
  assert(match("~", "a", TEST_RESTRICTION_2) == 1);
  assert(match("~a", "aaa", TEST_RESTRICTION_2) == 1);//Test for edge case to make sure match correctly matches as ~ would match exaclty only 2 letters in "aaa"
  assert(match("~~", "a", TEST_RESTRICTION_3) == 1);
  assert(match("~", "aaaaa", TEST_RESTRICTION_3) == 1);//Test for edge case to make sure match correctly matches as ~ would match exaclty the 5 letters in "aaaaa"
  assert(match("~", "inttterrmeediateeee", TEST_RESTRICTION_4) == 1);
  assert(match("~", "", TEST_RESTRICTION_4) == 1);//Test to make sure even though the restriction is very large itll match an empty string
  
}

int main() {
  printf("Starting Tests...\n");
  test_read_files();
  test_match_regex();
  test_match_regex_star();
  test_match_regex_question();
  test_match_regex_tilde();
  test_match_regex_multiple();
  test_match_regex_tilde_restriction();
  printf("All tests passed!!!\n");
}
