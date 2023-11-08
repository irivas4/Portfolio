#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "puzzle.h"

// also tests puzzle_destroy
void test_puzzle_create(void) {

  Puzzle *p3 = puzzle_create(3);
  Puzzle *p5 = puzzle_create(5);

  assert(p3->rcNum == 3);
  for (int i = 0; i < 9; ++i) {
    assert(p3->pos[i] == 0);
  }

  assert(p5->rcNum == 5);
  for (int i = 0; i < 25; ++i) {
    assert(p5->pos[i] == 0);
  }

  puzzle_destroy(p3);
  puzzle_destroy(p5);
}

// also tests puzzle_get_tile
void test_puzzle_set_tile(void) {
  int init[4][4] = {
    { 5, 7, 1, 2 },
    { 13, 9, 3, 4 },
    { 0, 8, 6, 11 },
    { 14, 15, 10, 12 },
  };

  Puzzle *p4 = puzzle_create(4);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      puzzle_set_tile(p4, j, i, init[i][j]);
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      assert(puzzle_get_tile(p4, j, i) == init[i][j]);
    }
  }
  puzzle_destroy(p4);
}

// ...other test functions...

// also tests puzzle_find_gap
void test_S_command(void) {
  int grid[4][4]= {
    { 2, 6, 3, 7 },
    { 14, 10, 8, 9 },
    { 1, 0, 4, 12 },
    { 5, 11, 13, 15 },
  };

  Puzzle *puzzle = puzzle_create(4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      puzzle_set_tile(puzzle, j, i, grid[i][j]);
    }
  }
  int gap = puzzle_find_gap(puzzle);
  assert(gap == 9);
  puzzle_move_tile(puzzle, 'd', gap); // testing down move
  assert(puzzle_find_gap(puzzle) == 5);
  assert(puzzle->pos[9] == 10);
  gap = puzzle_find_gap(puzzle);
  puzzle_move_tile(puzzle, 'l', gap); // tesing left move
  assert(puzzle_find_gap(puzzle) == 6);
  assert(puzzle->pos[5] == 8);
  gap = puzzle_find_gap(puzzle);
  puzzle_move_tile(puzzle, 'u', gap); // testing up move
  assert(puzzle_find_gap(puzzle) == 10);
  assert(puzzle->pos[6] == 4);
  gap = puzzle_find_gap(puzzle);
  puzzle_move_tile(puzzle, 'r', gap); // testing right move
  assert(puzzle_find_gap(puzzle) == 9);
  assert(puzzle->pos[10] == 10);
}

// also tests k command
void test_solve_puzzle(void) {
  int grid[3][3]= {
    { 2, 6, 3 },
    { 8, 7, 0 },
    { 1, 5, 4 },
  
  };

  Puzzle *puzzle = puzzle_create(3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      puzzle_set_tile(puzzle, j, i, grid[i][j]);
    }
  }

  assert(handle_K_command(puzzle) == 0);
  solve_puzzle(puzzle);
  assert(handle_K_command(puzzle) == 1);
}

void test_image_commands(void) {
}

int main(void) {
  test_puzzle_create();
  test_puzzle_set_tile();
  test_S_command();
  test_solve_puzzle();
  printf("All tests passed!\n");
  return 0;
}
