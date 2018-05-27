#ifndef __CALC_H__
#define __CALC_H__

/**
 * @file calc.h
 * A Star pathfinding simulation
 *
 * @author DJSchaffner
 */

#include "errors.h"
#include "library.h"

/* Macros */
#define COLS(n) n->cols
#define ROWS(n) n->rows
#define H(n, i, j) n->cells[((i * n->cols) + j)].h
#define STATE(n, i, j) n->cells[((i * n->cols) + j)].state

/* COLORS */
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

enum State { TODO = 0, BLOCKED, DONE, PATH, DEST };

struct Cell {
  int state;
  int h;
};

typedef struct Field {
  struct Cell* cells;
  int rows;
  int cols;
  struct Point src;
  struct Point dst;
} * Field;

/**
 * Creates an empty Field.
 *
 * @return Empty field.
 */
Field field_empty();

/**
 * Check if the point in info is inside the field.
 *
 * @param field The field with the boundaries.
 * @param e The info containing the point to be checked.
 *
 * @return Boolean.
 */
int info_inbounds(Field field, Info e);

/**
 * Checks if a point is within given boundaries.
 *
 * @param p The point to check.
 * @param rows The amount of rows (y coord.).
 * @param cols The amount of columns (x coord.).
 *
 * @return boolean.
 */
int point_inbounds(Point p, int rows, int cols);

/**
 * Initialize a Field.
 *
 * @param rows The rows of the field to be created.
 * @param cols The Colums of the field to be created.
 *
 * @return The initialized field.
 */
Field field_init(int rows, int cols, Point src, Point dst, Error* error);

/**
 * Calculate the best route from src to dst of a Field.
 *
 * @param field The field to be calcualted.
 * @param *error Error variable in case something goes wrong.
 *
 * @return The calculated Field.
 */
Field field_calc(Field field, Error* error);

/**
 * Initialize a library for a field.
 *
 * @param field The field containing the Info.
 *
 * @return The initializied Library.
 */
Library field_libInit(Field field);

/**
 * Paint the final path from src to dst in a field based on a final library.
 *
 * @param field The field containing the info.
 * @param lib The Library containing the info.
 *
 * @return The modified field.
 */
Field field_paintPath(Field field, Library lib);

/**
 * Set a cell in a field to be blocked.
 *
 * @param field The field to add the block to.
 * @param p The point to be blocked.
 *
 * @return The modified field.
 */
Field field_addBlock(Field field, Point p, Error* error);

/**
 * Deletes an existing Field.
 *
 * @return Empty Field.
 */
Field field_clear(Field field);

/**
 * Prints the contents of a Field to stdout.
 *
 * @param rows The rows of the field to be printed.
 * @param cols The colums of the field to be printed.
 */
void field_print(Field field);

/**
 * Add all neighbors of the current location, to the library.
 *
 * @param field The field to be traveled.
 * @param lib The Library for the field.
 * @param e The starting location.
 *
 * @return The Library with the added neighbors.
 */
Library field_addNeighbors(Field field, Library lib, Info e);

#endif