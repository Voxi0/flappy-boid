PROGRAM_NAME := boid

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

# Toolchain
CC := gcc
CFLAGS := -I $(INCLUDE_DIR) -std=c23 -Wall -Wextra -pedantic
CFLAGS += $(shell pkg-config --cflags raylib)
LDFLAGS := -lraylib -lm
C_SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(C_SRCS:%=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR)/$(PROGRAM_NAME)

# Run program
run: $(BUILD_DIR)/$(PROGRAM_NAME)
	@./build/$(PROGRAM_NAME)

# Fetch all necessary dependencies
fetchDeps:
	# Raygui
	wget https://raw.githubusercontent.com/raysan5/raygui/refs/heads/master/src/raygui.h -O $(INCLUDE_DIR)/raygui.h

# Final program
$(BUILD_DIR)/$(PROGRAM_NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# C compilation rule
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean everything
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)