.PHONY: setup build run clean help

# Default target
.DEFAULT_GOAL := help

# Build directory
BUILD_DIR := build
EXECUTABLE := $(BUILD_DIR)/bin/engine2d

# Default to demo-snake if no path is provided
GAME_DIR ?= demos/demo-snake

# Catch-all target to prevent Make from complaining about unknown targets
%:
	@:

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
# Usage: make run [path]
#   make run                    # Uses default (demos/demo-snake)
#   make run demos/demo-squares # Runs demos/demo-squares
run: build
	@$(eval RUN_ARGS := $(filter-out $@,$(MAKECMDGOALS)))
	@if [ -n "$(RUN_ARGS)" ]; then \
		GAME_DIR="$(firstword $(RUN_ARGS))"; \
	else \
		GAME_DIR="$(GAME_DIR)"; \
	fi; \
	echo "Running $(EXECUTABLE) with game directory: $$GAME_DIR"; \
	$(EXECUTABLE) $(abspath $$GAME_DIR)

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
	@echo "  make run demos/demo-squares # Run demo-squares"
	@echo "  make run demos/demo-dogs    # Run demo-dogs"
	@echo ""
	@echo "Quick start: make setup && make run"