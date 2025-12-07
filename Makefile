.PHONY: setup build run clean help

# Default target
.DEFAULT_GOAL := help

# Build directory
BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/bin/inf-cpp

# Default to demo-snake if no path is provided
GAME_DIR ?= demos/demo-snake

# Setup: Install all dependencies and build tools (Ubuntu)
setup:
	@sudo apt-get update
	@sudo apt-get install -y build-essential cmake
	@sudo apt-get install -y libglfw3-dev libgl1-mesa-dev
	@sudo apt-get install -y liblua5.4-dev pkg-config
	@sudo apt-get install -y libopenal-dev libsndfile1-dev

# Build: Configure and compile the project
build:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		mkdir -p $(BUILD_DIR); \
	fi
	@cd $(BUILD_DIR) && cmake ..
	@cd $(BUILD_DIR) && make

# Run: Build and run the project
run: build
	@$(EXECUTABLE) $(abspath $(GAME_DIR))

# Clean: Remove build directory
clean:
	@rm -rf $(BUILD_DIR)

# Help: Show available targets
help:
	@echo "Available targets:"
	@echo "  make setup  - Install all dependencies and build tools (Ubuntu/Debian)"
	@echo "  make build  - Configure and build the project"
	@echo "  make run    - Build and run the project (defaults to demos/demo-snake)"
	@echo "  make clean  - Remove build artifacts"
	@echo ""
	@echo "Usage examples:"
	@echo "  make run                    # Run demo-snake (default)"
	@echo "  make run GAME_DIR=game      # Run main game"
	@echo "  make run GAME_DIR=demos/demo-dogs  # Run demo-dogs"
	@echo ""
	@echo "Quick start: make setup && make run"

