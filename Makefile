CC := g++ # This is the main compiler
# CC := gcc # TODO: Only c compiler
# CC := clang --analyze # and comment out the linker last line for sanity (under $(TARGET))
SRCDIR := src
BUILDDIR := build
TARGET := bin/app
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
LIB := -L lib
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

# UNNCOMENT BELLOW IF NEEDED
# Clean compilation for production (removes source and build dir)
#clean:
#	@echo " Cleaning..."; 
#	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests - unit tests
#test:
#	$(CC) $(CFLAGS) test/main.cpp $(INC) $(LIB) -o bin/test

# Spikes - small chunks of code, classes keeped for testing, future reference 
#ticket:
#	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
