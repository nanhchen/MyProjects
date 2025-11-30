# -------------------------------
# Makefile for C++ project with src/ and include/
# -------------------------------

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
TARGET = app

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INCDIR) -g

# Source, object, and dependency files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

# Default rule: build the executable
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile .cpp files to .o files in build/ and generate .d files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files if they exist
-include $(DEPS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean compiled files and build folder
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Phony targets (not real files)
.PHONY: all clean run
