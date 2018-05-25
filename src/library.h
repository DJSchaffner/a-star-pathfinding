#ifndef __LIBRARY_H__
#define __LIBRARY_H__

/**
 * @file library.h
 * A Star pathfinding simulation
 * 
 * @author Jonas Wilkens
 */
 
#define INFOX(n) n.coord.x 
#define INFOY(n) n.coord.y
#define INFOVALID(n) n.valid
#define INFOTRAVEL(n) n.travel
#define INFOTOTAL(n) n.total
 
 typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Info {
	Point coord;
	int valid;	
	int travel;
	int total;
	Point prev;
} Info;

typedef struct Node *Library;

struct Node {	
	Info info;
	Library next;
};

/**
 * Creates an empty Library.
 *
 * @return Empty Library.
 */ 
Library library_empty();

/**
 * Gets the Info of a Library.
 *
 * @param lib The library containing info.
 *
 * @return Info.
 */
Info head( Library lib );

/**
 * Gets the next element of a Library.
 *
 * @param lib The library containing info.
 *
 * @return Next Library.
 */
Library tail( Library lib );

/**
 * Checks if library is empty.
 *
 * @param lib The library to be checked.
 *
 * @return Boolean.
 */
int library_isEmpty( Library lib );

/**
 * Inserts e into the library lib.
 *
 * @param lib The library to be modified.
 * @param e The info to be inserted.
 *
 * @return The modified Library.
 */
Library library_insert( Library lib, Info e );

/**
 * Updates e in the library if it is the same Point with a lower total value.
 *
 * @param lib The library to be modified.
 * @param e The info to be updated.
 *
 * @return The modified Library.
 */
Library library_update( Library lib, Info e );

/**
 * Removes the current min from lib.
 *
 * @param lib The library to be modified.
 *
 * @return The modified Library.
 */
Library library_removeMin( Library lib );

/**
 * Checks if e is contained in lib (Only the coordinates!).
 *
 * @param lib The library to be checked.
 * @param e The info to be checked.
 *
 * @return Boolean.
 */
int library_contains( Library lib, Info e);

/**
 * Frees the entire library.
 *
 * @param lib The library to be deleted.
 *
 * @return NULL
 */
Library library_clear( Library lib );

/**
 * Finds an entry for a specific point in a library.
 *
 * @param lib The library to be searched.
 * @param p The point of the entry to be found.
 *
 * @return The found entry.
 */
Info library_getEntry( Library lib, Point p );

/**
 * Prints a library to stdout.
 *
 * @param lib The Library to be printed.
 */
void library_print( Library lib );

#endif