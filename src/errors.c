/**
 * @file errors.c
 * A Star pathfinding simulation
 * 
 * @author Jonas Wilkens
 */
 
#include <stdio.h>

#include "errors.h"

void print_error (Error error) {
	switch (error) {
		case ERR_NULL: 
			break;
		case ERR_WRONG_ARG_COUNT:
			fprintf(stderr, "Error: Wrong Argument count!\n");
			break;
		case ERR_WRONG_ARG:
			fprintf(stderr, "Error: Wrong Argument(s)!\n");
			break;
		case ERR_OUT_OF_BOUNDS:
			fprintf(stderr, "Error: Point out of Bounds!\n");
			break;
		case ERR_NO_PATH:
			fprintf(stderr, "Error: No path found!\n");
			break;
		case ERR_MEMORY_LEAK:
			fprintf(stderr, "Error: Memory Leak!\n");
			break;
		case ERR_OUT_OF_MEMORY:
			fprintf(stderr, "Error: Out of Memory!\n");
			break;
		default:
			break;
	}
}