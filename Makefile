PROGRAM_NAME := boid

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

# Toolchain
CC := gcc
CFLAGS := -I $(INCLUDE_DIR) -std=c23 -Wall -Wextra -pedantic -o $(BUILD_DIR)/$(PROGRAM_NAME)
CFLAGS += $(shell pkg-config --cflags raylib)

EMCC := emcc
EMFLAGS := -o $(BUILD_DIR)/$(PROGRAM_NAME).html $(CFLAGS) -Os ./web/libraylib.a -L ./web/libraylib.a -s USE_GLFW=3 --shell-file ./web/minshell.html -DPLATFORM_WEB --preload-file ./assets/

LDFLAGS := -lraylib -lm

C_SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(C_SRCS:%=$(BUILD_DIR)/%.o)

# Check stuff
DEBUG ?= 0
ifeq ($(DEBUG),1)
	CFLAGS += -g -O0 -DDEBUG
else
	CFLAGS += -O2 -DNDEBUG -s
endif

# Default target
all: buildDesktop

# Run program
.PHONY: runDesktop runWeb
runDesktop: buildDesktop
	@./build/$(PROGRAM_NAME)
runWeb: buildWeb
	@emrun ./build/$(PROGRAM_NAME).html

# Fetch all necessary dependencies
.PHONY: fetchDeps
fetchDeps:
	# Raygui
	wget https://raw.githubusercontent.com/raysan5/raygui/refs/heads/master/src/raygui.h -O $(INCLUDE_DIR)/raygui.h

	# Fetch basic shell for Raylib apps to work on the web or whatever
	mkdir -p web
	wget https://raw.githubusercontent.com/raysan5/raylib/refs/heads/master/src/minshell.html -O web/minshell.html

	# Fetch Raylib library file (WebAssembly build)
	wget https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_webassembly.zip -O web/temp.zip
	cd web && unzip temp.zip && rm -rf temp.zip
	mv web/raylib-5.5_webassembly/lib/libraylib.a web/

# Final program
.PHONY: buildDesktop buildWeb
buildDesktop: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS)
buildWeb: $(C_SRCS)
	$(EMCC) $(EMFLAGS) $(C_SRCS)

# C compilation rule
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean everything
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)