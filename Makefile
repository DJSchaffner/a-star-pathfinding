CC := gcc
CFLAGS := -c -Wall -Wextra

# Folders
SRCDIR 	:= src
OBJDIR	:= obj

SRC			:= $(shell find $(SRCDIR) -type f -name '*.c')
OBJ			:= $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC:.c=.o))

# Output Name
BIN			:= astar

.PHONY: all clean directories
default: all

all: directories $(BIN)

directories:
	@mkdir -p $(OBJDIR)

$(BIN) : $(OBJ)
	$(CC) -o $(BIN) $(OBJ)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean: 
	rm -f $(BIN) 
	rm -rf $(OBJDIR)
	rm -f $(BIN).exe.stackdump