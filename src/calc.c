/**
 * @file calc.c
 * A Star pathfinding simulation
 *
 * @author DJSchaffner
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "calc.h"
#include "errors.h"
#include "library.h"

int field_refs = 0;

Field field_empty() { return NULL; }

int info_inbounds(Field field, Info e) {
  return (INFOX(e) >= 0 && INFOX(e) < ROWS(field) && INFOY(e) >= 0 &&
          INFOY(e) < COLS(field));
}

int point_inbounds(Point p, int rows, int cols) {
  return (p.x >= 0 && p.x < rows && p.y >= 0 && p.y < cols);
}

int point_equal(Point p1, Point p2) { return (p1.x == p2.x && p1.y == p2.y); }

Field field_init(int rows, int cols, Point src, Point dst, Error* error) {
  Field field = field_empty();
  int i = 0, j = 0;

  if (!point_inbounds(src, rows, cols) && !point_inbounds(dst, rows, cols)) {
    *error = ERR_OUT_OF_BOUNDS;
  } else {
    /* Allocate memory for the field */
    if (!(field = malloc(sizeof(struct Field)))) {
      exit(ERR_OUT_OF_MEMORY);
    }

    /* Allocate memory for the cells */
    if (!(field->cells = malloc(sizeof(struct Cell) * rows * cols))) {
      exit(ERR_OUT_OF_MEMORY);
    }

    field->rows = rows;
    field->cols = cols;
    field->src = src;
    field->dst = dst;
    field_refs++;

    /* Initialize values of each cell */
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        H(field, i, j) = abs(dst.x - i) + abs(dst.y - j);
        STATE(field, i, j) = TODO;
      }
    }

    /* Set dst and src Cells */
    STATE(field, field->src.x, field->src.y) = PATH;
    STATE(field, field->dst.x, field->dst.y) = DEST;
  }

  return field;
}

Field field_calc(Field field, Error* error) {
  Library lib = library_empty(), done = library_empty();
  Info tmp = {0};

  INFOVALID(tmp) = 1;
  lib = field_libInit(field);

  while (!library_isEmpty(lib) && !(INFOX(head(lib)) == field->dst.x &&
                                    INFOY(head(lib)) == field->dst.y)) {
    /* Copy values to tmp */
    INFOX(tmp) = INFOX(head(lib));
    INFOY(tmp) = INFOY(head(lib));
    INFOTRAVEL(tmp) = INFOTRAVEL(head(lib));

    /* Move current Location to done and add new candidates */
    done = library_insert(done, head(lib));
    STATE(field, INFOX(tmp), INFOY(tmp)) = DONE;
    lib = library_removeMin(lib);
    lib = field_addNeighbors(field, lib, tmp);
  }

  /* No valid path to dst found */
  if (library_isEmpty(lib)) {
    *error = ERR_NO_PATH;
  }
  /* Add dest to <done> AND paint the path */
  else {
    done = library_insert(done, head(lib));
    field = field_paintPath(field, done);
  }

  /* DEBUG INFO */
  /*
  printf("########\n");
  library_print(done);
  printf("########\n");
  */

  /* Free memory */
  lib = library_clear(lib);
  done = library_clear(done);

  return field;
}

Library field_libInit(Field field) {
  Library lib = library_empty();
  Info e = {0};

  /* Set Info */
  INFOX(e) = field->src.x;
  INFOY(e) = field->src.y;
  INFOVALID(e) = 1;
  INFOTOTAL(e) = H(field, INFOX(e), INFOY(e));
  INFOTRAVEL(e) = 0;

  lib = library_insert(lib, e);

  return lib;
}

Library field_addNeighbors(Field field, Library lib, Info e) {
  Library res = lib;

  /* Set travel time for neighbors to current + 1 */
  INFOTRAVEL(e)++;
  e.prev = e.coord;

  /* Go through all 4 neighbors
   * --X--
   * -X0X-
   * --X--
   */

  /* NORTH */
  INFOX(e)--;
  if (info_inbounds(field, e) && (STATE(field, INFOX(e), INFOY(e)) == TODO ||
                                  STATE(field, INFOX(e), INFOY(e)) == DEST)) {
    INFOTOTAL(e) = INFOTRAVEL(e) + H(field, INFOX(e), INFOY(e));
    res = library_insert(res, e);
  }
  /* EAST */
  INFOX(e)++;
  INFOY(e)++;
  if (info_inbounds(field, e) && (STATE(field, INFOX(e), INFOY(e)) == TODO ||
                                  STATE(field, INFOX(e), INFOY(e)) == DEST)) {
    INFOTOTAL(e) = INFOTRAVEL(e) + H(field, INFOX(e), INFOY(e));
    res = library_insert(res, e);
  }
  /* SOUTH */
  INFOX(e)++;
  INFOY(e)--;
  if (info_inbounds(field, e) && (STATE(field, INFOX(e), INFOY(e)) == TODO ||
                                  STATE(field, INFOX(e), INFOY(e)) == DEST)) {
    INFOTOTAL(e) = INFOTRAVEL(e) + H(field, INFOX(e), INFOY(e));
    res = library_insert(res, e);
  }
  /* WEST */
  INFOX(e)--;
  INFOY(e)--;
  if (info_inbounds(field, e) && (STATE(field, INFOX(e), INFOY(e)) == TODO ||
                                  STATE(field, INFOX(e), INFOY(e)) == DEST)) {
    INFOTOTAL(e) = INFOTRAVEL(e) + H(field, INFOX(e), INFOY(e));
    res = library_insert(res, e);
  }

  return res;
}

Field field_paintPath(Field field, Library lib) {
  Info tmp = {0};

  /* Try to retrieve destination entry from done set */
  tmp = library_getEntry(lib, field->dst);

  /* Entry found? */
  if (INFOVALID(tmp)) {
    /* Continue painting until at the start location */
    while (INFOX(tmp) != field->src.x || INFOY(tmp) != field->src.y) {
      tmp = library_getEntry(lib, tmp.prev);
      STATE(field, INFOX(tmp), INFOY(tmp)) = PATH;
    }
  }
  return field;
}

Field field_addBlock(Field field, Point p, Error* error) {
  /* p is outside of the field */
  if (!point_inbounds(p, ROWS(field), COLS(field))) {
    *error = ERR_OUT_OF_BOUNDS;
  }
  /* p is equal to src or dst */
  else if (point_equal(p, field->src) || point_equal(p, field->dst)) {
    *error = ERR_OUT_OF_BOUNDS;
  } else {
    STATE(field, p.x, p.y) = BLOCKED;
  }

  return field;
}

Field field_clear(Field field) {
  free(field);
  field_refs--;

  return field_empty();
}

void field_print(Field field) {
  int i = 0, j = 0;

  for (i = 0; i < ROWS(field); i++) {
    for (j = 0; j < COLS(field); j++) {
      /* DEBUG INFO */
      /* printf("[%d]", ((i*field->cols) + j)); */
      /* printf("%d ", H(field, i, j)); */
      switch (STATE(field, i, j)) {
        case TODO:
          printf(RED "X " RESET);
          break;
        case DONE:
          printf(YELLOW "X " RESET);
          break;
        case PATH:
          printf(GREEN "X " RESET);
          break;
        case DEST:
          printf(BLUE "X " RESET);
          break;
        default:
          printf(WHITE "X " RESET);
          break;
      }
    }
    printf("\n");
  }
}