/**
 * @file library.c
 * A Star pathfinding simulation
 * 
 * @author Jonas Wilkens
 */

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>

#include "library.h"
#include "calc.h"
#include "errors.h"

/**
 * Compares e1 to e2 and returns if e1 is smaller, bigger or equal to e2.
 *
 * @param e1 Info giving the perspective.
 * @param e2 Info target.
 *
 * @return -1(smaller), 0(equal), 1(greater)
 */
int info_compare( Info e1, Info e2 ) {
	if( e1.total < e2.total ) {
		return -1;
	}
	if( e1.total == e2.total ) {
		return 0;
	}
	if( e1.total > e2.total ) {
		return 1;
	}
	
	/* Never executed but necessary */
	return 0;
}

/**
 * Compares e1 to e2 in terms of coordinates.
 *
 * @param e1 Info to be compared.
 * @param e2 Info to be compared.
 *
 * @return Boolean.
 */
int info_isEqual( Info e1, Info e2 ) {
	return( e1.coord.x == e2.coord.x && e1.coord.y == e2.coord.y );
}

Library library_empty() {
	return NULL;
}

Info head( Library lib ) {
	assert(!library_isEmpty(lib));
	
	return lib->info;
}

Library tail( Library lib ) {
	assert(!library_isEmpty(lib));
	
	return lib->next;
}

int library_isEmpty( Library lib ) {
	return lib == library_empty();
}

Library library_removeMin( Library lib ) {
	assert( !library_isEmpty(lib) ); {
		Library res = library_empty();
	
		if( !library_isEmpty(lib) ) {
			res = tail(lib);
			free(lib);
		}	
		
		return res;
	}
}

Library library_cons( Library lib, Info e ) {
	Library res = library_empty();
	
	if( !(res = malloc(sizeof(*lib))) ) {
		exit(ERR_OUT_OF_MEMORY);
	}
	
	res->info = e;
	res->next = lib;
	
	return res;
}

Library library_insert( Library lib, Info e ) {
	if( library_contains(lib, e) ) {
		return library_update(lib, e);
	}
	else {
		if( (library_isEmpty(lib) || info_compare(e, head(lib)) <= 0) ) {
			return library_cons( lib, e );
		}
		else if( info_compare(e, head(lib)) > 0 ) {
			lib->next = library_insert(lib->next, e);
		}
	}	
	
	return lib;
}

Library library_update( Library lib, Info e ) {
	if( library_isEmpty(lib) ) {
		return lib;
	}
	
	if( info_isEqual(head(lib), e) ) {
		if( info_compare(e, head(lib)) == -1 ) {
			lib = library_removeMin(lib);
			lib = library_insert(lib, e);
			
			return lib;
		}
	}
	
	lib->next = library_update(tail(lib), e);;
	return lib;
}

int library_contains( Library lib, Info e ) {
	if( library_isEmpty(lib) ) {
		return 0;
	}
	else {
		return (info_isEqual(head(lib), e) || library_contains(tail(lib), e));
	}
}

Info library_getEntry( Library lib, Point p ) {
	Info e = {0};
	
	/* Return <invalid> info */
	if( library_isEmpty(lib) ) {
		return e;
	}
	
	e.coord = p;
	if( info_isEqual(head(lib), e) ) {
		return head(lib);
	}
	
	return library_getEntry( tail(lib), p );
}

Library library_clear( Library lib ) {	
	if( !library_isEmpty(lib) ) {
		lib->next = library_clear(lib->next);
		free(lib);
	}
	
	return library_empty();
}

void library_print( Library lib ) {
	if( !library_isEmpty(lib) ) {
		printf("[%d,%d,%d, PREV[%d, %d]]\n", head(lib).coord.x, head(lib).coord.y, head(lib).total, head(lib).prev.x, head(lib).prev.y);
		
		if( !library_isEmpty(tail(lib)) ) {
			library_print(tail(lib));
		} 	
	}
	/* Empty library */
	else {
		printf("[ ]\n");
	}
}