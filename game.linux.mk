DIR := $(CURDIR)
BUILD_DIR := bin/linux
OBJ_DIR := obj

ASSEMBLY := game
EXTENSION := 
COMPILER_FLAGS := -g -Wno-missing-braces  #-fPIC
INCLUDE_FLAGS := -Iengine/headers -Igame/headers 
LINKER_FLAGS := -L$(BUILD_DIR) -lengine -Wl,-rpath,./$(BUILD_DIR)/
DEFINES := -D_DEBUG -DIMPORT

# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRC_FILES := $(call rwildcard,$(ASSEMBLY)/,*.c) # Get all .c files
DIRECTORIES := $(subst $(DIR)/,,$(shell find $(ASSEMBLY)/src -type d)) # Get all directories under src.
OBJ_FILES := $(SRC_FILES:$(ASSEMBLY)/%.c=$(OBJ_DIR)/%.c.o) # Get all compiled .c.o objects for game

all: scaffold compile link

.PHONY: scaffold
scaffold: # create build directory
	@echo Scaffolding folder structure...
	-@mkdir -p $(addprefix $(OBJ_DIR)/, $(DIRECTORIES))

.PHONY: link
link: scaffold $(OBJ_FILES) # link
	@echo Linking $(ASSEMBLY)...
	@clang $(OBJ_FILES) -o $(BUILD_DIR)/$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)

.PHONY: compile
compile: $(OBJ_FILES) #compile .c files
	@echo Compiling...

.PHONY: clean
clean: # clean build directory
	@echo Cleaning game...
	@rm -f $(BUILD_DIR)/$(ASSEMBLY).*
	@rm -rf $(OBJ_DIR)/$(ASSEMBLY)

$(OBJ_DIR)/%.c.o: $(ASSEMBLY)/%.c # compile .c to .c.o object
	@echo   $<...
	@clang $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
