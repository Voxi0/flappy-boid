PROGRAM_NAME := boid

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

# Toolchain
CXX := clang++
CXXFLAGS := -I $(INCLUDE_DIR) -std=c++26 -Wall -Wextra -Wpedantic
CXXFLAGS += $(shell pkg-config --cflags raylib)

LDFLAGS := -lraylib -lm

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Check stuff
RELEASE ?= 0
ifeq ($(RELEASE),1)
	CFLAGS += -O2 -DNDEBUG -s
else
	CFLAGS += -O0 -DDEBUG -g
endif

# Default target
all: $(BUILD_DIR)/$(PROGRAM_NAME)

# Run program
.PHONY: run
run: $(BUILD_DIR)/$(PROGRAM_NAME)
	@./$<

# Fetch all necessary dependencies
.PHONY: fetchDeps
fetchDeps:
	# Raygui
	wget https://raw.githubusercontent.com/raysan5/raygui/refs/heads/master/src/raygui.h -O $(INCLUDE_DIR)/raygui.h

# Final program
$(BUILD_DIR)/$(PROGRAM_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

# C compilation rule
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean everything
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)