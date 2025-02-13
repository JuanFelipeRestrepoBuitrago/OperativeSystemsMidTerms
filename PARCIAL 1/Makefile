# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -I/usr/include/gtest -I$(SRC_DIR) \
         -I/opt/homebrew/opt/openssl@3/include -I/opt/homebrew/opt/googletest/include
LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -L/opt/homebrew/opt/googletest/lib \
          -lgtest -lgtest_main -pthread -lcrypto -lssl

OUTDIR = out
SRC_DIR = src
TEST_DIR = src/tests

# Main executable
TARGET = $(OUTDIR)/rsa_exec

# Source files
SOURCES = main.cpp \
          $(SRC_DIR)/core/RSA.cpp \
          $(SRC_DIR)/helpers/Utils.cpp \
          $(SRC_DIR)/helpers/FileManager.cpp

# Object files
OBJECTS = $(patsubst %.cpp, $(OUTDIR)/%.o, $(SOURCES))

# Default target: Compile everything
all: $(OUTDIR) $(OUTDIR)/src $(OUTDIR)/src/core $(OUTDIR)/src/helpers $(TARGET) test

# Build the main program
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each .cpp file into .o
$(OUTDIR)/%.o: %.cpp | $(OUTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/src/core/%.o: $(SRC_DIR)/core/%.cpp | $(OUTDIR)/src/core
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/src/helpers/%.o: $(SRC_DIR)/helpers/%.cpp | $(OUTDIR)/src/helpers
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------
# CREATE NECESSARY DIRECTORIES
# -----------------------
$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/src:
	mkdir -p $(OUTDIR)/src

$(OUTDIR)/src/core:
	mkdir -p $(OUTDIR)/src/core

$(OUTDIR)/src/helpers:
	mkdir -p $(OUTDIR)/src/helpers

# -----------------------
# TESTS
# -----------------------
# Test executables
TEST_EXECUTABLES = $(OUTDIR)/testUtils $(OUTDIR)/testRSA

test: $(TEST_EXECUTABLES)
	$(OUTDIR)/testUtils
	$(OUTDIR)/testRSA

# Build testUtils
$(OUTDIR)/testUtils: $(OUTDIR)/src/tests/testUtils.o $(OUTDIR)/src/helpers/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testUtils.cpp
$(OUTDIR)/src/tests/testUtils.o: $(SRC_DIR)/tests/testUtils.cpp | $(OUTDIR)/src/tests
	$(CC) $(CFLAGS) -c $< -o $@

# Build testRSA
$(OUTDIR)/testRSA: $(OUTDIR)/src/tests/testRSA.o $(OUTDIR)/src/core/RSA.o $(OUTDIR)/src/helpers/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testRSA.cpp
$(OUTDIR)/src/tests/testRSA.o: $(SRC_DIR)/tests/testRSA.cpp | $(OUTDIR)/src/tests
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/src/tests:
	mkdir -p $(OUTDIR)/src/tests

# -----------------------
# CLEAN BUILD FILES
# -----------------------
clean:
	rm -rf $(OUTDIR)

# Run the main program
run: $(TARGET)
	./$(TARGET)
