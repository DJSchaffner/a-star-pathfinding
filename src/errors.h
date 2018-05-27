/**
 * @file errors.h
 * A Star pathfinding simulation
 *
 * @author DJSchaffner
 */

#ifndef __ERRORS_H__
#define __ERRORS_H__

enum Error {
  ERR_NULL = 0,
  ERR_WRONG_ARG_COUNT,
  ERR_WRONG_ARG,
  ERR_OUT_OF_BOUNDS,
  ERR_NO_PATH,
  ERR_MEMORY_LEAK,
  ERR_OUT_OF_MEMORY
};

typedef enum Error Error;

/**
 * Prints the errormessage for error on stderr.
 *
 * @param error The error that should be printed.
 */
void print_error(Error error);

#endif