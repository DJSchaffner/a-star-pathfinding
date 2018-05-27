/**
 * @file main.c
 * A Star pathfinding simulation
 *
 * @author DJSchaffner
 */

#include <stdio.h>
#include <string.h>

#include "calc.h"
#include "errors.h"

extern int field_refs;

/**
 * Prints the usage.
 *
 * @param stream Output stream.
 */
static void print_usage(FILE *stream);

static void print_usage(FILE *stream) {
  fprintf(stream, "USAGE:  astar SIZE SRC DST [BLOCK ...]\n");
  fprintf(stream, "            SIZE \t- INTEGERxINTEGER.\n");
  fprintf(stream,
          "            SRC  \t- INTEGER,INTEGER. (Starting from upper left)\n");
  fprintf(stream,
          "            DST  \t- INTEGER,INTEGER. (Starting from upper left)\n");
  fprintf(stream,
          "            BLOCK\t- INTEGER,INTEGER. (Starting from upper left)\n");
  fprintf(stream, "            or: astar -h -> print this Usage.\n");
  fprintf(stream, "\n");
  fprintf(stream, "        Colors:\n");
  fprintf(stream, BLUE "            BLUE:\tDestination\n");
  fprintf(stream, GREEN "            GREEN:\tPath\n");
  fprintf(stream, RED "            RED:\tTodo\n");
  fprintf(stream, WHITE "            WHITE:\tBlocked\n");
  fprintf(stream, YELLOW "            YELLOW:\tDone\n" RESET);
}

/**
 * ------------
 * MAIN PROGRAM
 * ------------
 */
int main(int argc, char *argv[]) {
  Field field = field_empty();
  Error error = ERR_NULL;
  Point src = {0}, dst = {0}, block = {0};

  int rows = 0, cols = 0, i = 0, usage = 0;
  char dummy = '\0';

  /* Parse input arguments */
  if (argc != 2 && argc < 4) {
    error = ERR_WRONG_ARG_COUNT;
  } else {
    /* Help option */
    if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
      usage = 1;
    }

    /* Creating the Field */
    else if (argc >= 4) {
      if (sscanf(argv[1], "%dx%d%c", &rows, &cols, &dummy) == 2 &&
          sscanf(argv[2], "%d,%d%c", &src.x, &src.y, &dummy) == 2 &&
          sscanf(argv[3], "%d,%d%c", &dst.x, &dst.y, &dummy) == 2) {
        /* Start the calcualtion if src and dst are valid */
        field = field_init(rows, cols, src, dst, &error);

        if (!error) {
          /* Loop block settings */
          for (i = 4; i < argc; i++) {
            if (sscanf(argv[i], "%d,%d%c", &block.x, &block.y, &dummy) == 2) {
              field = field_addBlock(field, block, &error);
            } else {
              error = ERR_WRONG_ARG;
            }
          }
        }
      } else {
        error = ERR_WRONG_ARG;
      }
    }

    else {
      error = ERR_WRONG_ARG;
    }
  }

  if (usage) {
    print_usage(stdout);
  }
  /* All main output of the program! */
  else if (!error) {
    field_print(field);
    field = field_calc(field, &error);

    /* Only print the result if no error occured */
    if (!error) {
      /* Temp fix: Print new line */
      printf("\n");
      field_print(field);
    }

    /* Free memory */
    field = field_clear(field);
  }

  /* Program leaks memory? */
  if (field_refs != 0) {
    error = ERR_MEMORY_LEAK;
  }

  if (error) {
    print_error(error);
    print_usage(stderr);
  }

  return error;
}