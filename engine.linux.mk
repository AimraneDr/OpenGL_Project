DIR := $(CURDIR)
BUILD_DIR := bin/linux
OBJ_DIR := obj

ASSEMBLY := engine
EXTENSION := .so
COMPILER_FLAGS := -g -fPIC
INCLUDE_FLAGS := -Iengine/headers -I$(HOME)/Dev/libs/glad/include/ -I$(HOME)/Dev/libs/stb/
LINKER_FLAGS := -shared -lGL -lX11 -lm -ldl #-L$(OBJ_DIR)/engine 
DEFINES := -D_DEBUG -DAPI -D_CRT_SECURE_NO_WARNINGS

# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))


SRC_FILES := $(call rwildcard,$(ASSEMBLY)/,*.c) # Get all .c files
DIRECTORIES := $(ASSEMBLY)/src $(subst $(DIR),,$(shell find $(ASSEMBLY)/src -type d)) # Get all directories under src
OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o) # Get all compiled .c.o objects for engine

all: scaffold compile link

.PHONY: scaffold
scaffold: # create build directory
	@echo Scaffolding folder structure...
	-@mkdir -p $(addprefix $(OBJ_DIR)/, $(DIRECTORIES))
	-@mkdir -p $(BUILD_DIR)
	@echo done

.PHONY: link
link: scaffold $(OBJ_FILES) # link
	@echo Linking $(ASSEMBLY)...
	@clang $(OBJ_FILES) -o $(BUILD_DIR)/lib$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)

.PHONY: compile
compile: $(OBJ_FILES) #compile .c files
	@echo Compiling...

.PHONY: clean
clean: # clean build directory
	@echo Cleaning engine...
	@rm -f $(BUILD_DIR)/$(ASSEMBLY).*
	@rm -rf $(OBJ_DIR)/$(ASSEMBLY)

$(OBJ_DIR)/%.c.o: %.c # compile .c to .c.o object
	@echo   obj : $<...
	@clang $(COMPILER_FLAGS) -c $< -o $@ $(DEFINES) $(INCLUDE_FLAGS)