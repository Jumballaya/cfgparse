#
# Config Parser
#
#

# Utilities
CLEANUP = rm -rf
MKDIR = mkdir -p
CP = cp


# Sources
DIR_SRC = src/

DIR_BUILD = build/
DIR_BUILD_OBJECTS = $(DIR_BUILD)objects/
DIR_INSTALL = /usr/bin/
DIR_BIN = bin/
DIR_TESTS = tests/
DIR_INC = include/


# Compiler settings
CC = gcc
FLAGS = -Wall
INC = -I $(DIR_INC)

# General info
BIN_NAME = cfgparse
TARGET_EXTENSION = out

# Files
ENTRY = main.c

SOURCES 			= $(wildcard $(DIR_SRC)*.c)
HEADERS 			= $(wildcard $(DIR_SRC)*h)
OBJ 					= $(patsubst $(DIR_SRC)%,$(DIR_BUILD_OBJECTS)%,$(SOURCES:.c=.o))

# Create binary
$(DIR_BIN)$(BIN_NAME): $(OBJ)
	$(MKDIR) $(DIR_BIN)
	$(CC) $(FLAGS) -o $@ $(ENTRY) $^

# Create the build files
$(DIR_BUILD_OBJECTS)%.o: $(DIR_SRC)%.c $(HEADERS) $(DIR_BUILD_OBJECTS)
	$(MKDIR) $(DIR_BUILD_OBJECTS)
	$(CC) -c -o $@ $< $(FLAGS) $(INC)

$(DIR_BUILD_OBJECTS):
	$(MKDIR) $(DIR_BUILD_OBJECTS)

$(DIR_BUILD):
	$(MKDIR) $(DIR_BUILD)

# Clean
clean:
	$(CLEANUP) $(DIR_BIN)
	$(CLEANUP) $(DIR_BUILD)

# Install the binary
install:
	$(CP) $(DIR_BIN)$(BIN_NAME) $(DIR_INSTALL)$(BIN_NAME)

.PHONY: build
.PHONY: clean
.PHONY: install
