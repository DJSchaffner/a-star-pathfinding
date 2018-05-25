CC = gcc
CFLAGS = -c -Wall Wextra

SRC		= $(shell find . -type f -name '*.cpp')
OBJ		= $(SRC:%.c=%.o)
BIN		= astar

.PHONY: all clean 
default: all

all: $(BIN)

$(BIN) : $(OBJ)
	$(CC) -o astar $(OBJ)
	
%.o: %.c
	$(CC) $(CFLAGS) $<
	
clean: 
	rm -f $(BIN) $(OBJ) $(BIN).exe.stackdump