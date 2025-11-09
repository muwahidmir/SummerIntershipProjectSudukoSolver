# Makefile for Sudoku Solver Master
# DSA Project - Team of 2 Members

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra
TARGET = sudoku_solver
SOURCE = main.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	@echo "🔨 Building Sudoku Solver Master..."
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)
	@echo "✅ Build successful! Run with: ./$(TARGET)"

# Run the program
run: $(TARGET)
	@echo "🚀 Running Sudoku Solver Master..."
	./$(TARGET)

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	rm -f $(TARGET)
	@echo "✅ Clean complete!"

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build with maximum optimization
release: CXXFLAGS += -O3 -DNDEBUG
release: $(TARGET)

# Check for memory leaks (requires valgrind)
memcheck: $(TARGET)
	@echo "🔍 Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Install dependencies (for Ubuntu/Debian)
install-deps:
	@echo "📦 Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y build-essential valgrind

# Help target
help:
	@echo "🎯 Sudoku Solver Master - Makefile Help"
	@echo "======================================"
	@echo "Available targets:"
	@echo "  all          - Build the program (default)"
	@echo "  run          - Build and run the program"
	@echo "  clean        - Remove build files"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build with maximum optimization"
	@echo "  memcheck     - Run with memory leak detection"
	@echo "  install-deps - Install system dependencies"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  make         # Build the program"
	@echo "  make run     # Build and run"
	@echo "  make clean   # Clean build files"

# Phony targets
.PHONY: all run clean debug release memcheck install-deps help 