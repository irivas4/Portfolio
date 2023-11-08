#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdio.h>
#include "ppm_io.h" // for Image data type

typedef struct _puzzle {
  int rcNum;
  int gapPos;
  int *pos;
} Puzzle;

/** Function to create the starting integer 2D array and initialize all elements with 0.
    @param size The row and column number of the array
    @return The puzzle grid with all its elements initialized to 0
 */
Puzzle *puzzle_create(int size);

/** Helper function for the C command with File input.
    @param in The input File
    @param p The puzzle
    @return 0 if puzzle creation is successful or 1 if unsuccessful
 */
int handle_C_command_File(FILE *in, Puzzle **p);

/** Helper function for T command with File input.
    @param in The input File
    @param p The puzzle
    @return 0 if the initializations are successful or 1 if unsuccessful
 */
int handle_T_command_File(FILE *in, Puzzle *p);

/** Helper function for S command for moving tiles around.
    @param p The puzzle
    @param move The desired move to be performed
    @param gap_position The position of the gap tile
    @return 0 if the move is successful or 1 if unsuccessful
 */
int puzzle_move_tile(Puzzle *p, char move, int gap_position);

/** Function used to find the position of the gap tile.
    @param p The puzzle
    @return The position of the gap tile
 */
int puzzle_find_gap(const Puzzle *p);

/** Helper function for P command to print the current puzzle grid.
    @param p The puzzle
 */
void puzzle_print_positions(Puzzle *p);

/** Function for the K command to check to see if the puzzle is in winning configuration.
    @param p The puzzle who's configuration were checking.
*/
int handle_K_Command(const Puzzle *p);

/** Function for the W command in order to write puzzle image to image and puzzle configuration to file.
    @param im The image were working with that we copy and modify to write out.
    @param p The puzzle where welll get out the current configuration of the puzzle/image.
    @param image The filename we'll be writing the image out to.
    @param file The filename we'll be writing the configuration out to.
*/
int handle_W_Command(Image * im, Puzzle * p, const char image[], const char file[]);

/** Helper function for the W command in order to find the start index of the piece of the image we want.
    @param rcNum The size of the puzzle
    @param rows The number of rows of pixels in the image
    @param cols The number of cols of pixels in the image
    @param the ind of the piece of the puzzle were currently working with
 */
int start(int rcNum, int rows, int cols, int ind);

/** Helper function for V command to recursively find solution to sliding puzzle
    @param p The puzzle
    @param steps The steps successfully executed
    @param max_steps The max_steps will be attempting so it doesn't go forever
    @param curr_Steps The current step were on to check we dont go over limit
 */
int solve_Puzzle(Puzzle *p, char steps[], int max_Steps, int curr_Steps, char last_Move);
                
#endif // PUZZLE_H
