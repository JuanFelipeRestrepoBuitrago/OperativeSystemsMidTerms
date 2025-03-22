CC = g++
CFLAGS = -Wall -Wextra -std=c++17 
MACFLAGS = -I/opt/homebrew/opt/googletest/include -L/opt/homebrew/opt/googletest/lib
TEST_CFLAGS = -I/usr/include/gtest -lgtest
SOURCE_DIR = src
OUTDIR = out

all: $(OUTDIR)/program
compile: $(OUTDIR)/program

$(OUTDIR)/program: $(OUTDIR)/$(SOURCE_DIR)/utils/Utils.o $(OUTDIR)/$(SOURCE_DIR)/standardImage/stb_wrapper.o $(OUTDIR)/$(SOURCE_DIR)/memory/BuddyAllocator.o $(OUTDIR)/$(SOURCE_DIR)/memory/FileManager.o $(OUTDIR)/$(SOURCE_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^

TEST_DIR = $(SOURCE_DIR)/tests
TEST_EXECUTABLES = $(OUTDIR)/$(TEST_DIR)/testUtils $(OUTDIR)/$(TEST_DIR)/testFileManager

# Run All Tests
test: clean $(TEST_EXECUTABLES)
	./$(OUTDIR)/$(TEST_DIR)/testUtils
	./$(OUTDIR)/$(TEST_DIR)/testFileManager

# Run individual tests
testUtils: clean $(OUTDIR)/$(TEST_DIR)/testUtils
	./$(OUTDIR)/$(TEST_DIR)/testUtils

testFileManager: clean $(OUTDIR)/$(TEST_DIR)/testFileManager
	./$(OUTDIR)/$(TEST_DIR)/testFileManager

# Compile testUtils
$(OUTDIR)/$(TEST_DIR)/testUtils: $(OUTDIR)/$(TEST_DIR)/testUtils.o $(OUTDIR)/$(SOURCE_DIR)/utils/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(TEST_CFLAGS)

$(OUTDIR)/$(TEST_DIR)/testUtils.o: $(TEST_DIR)/testUtils.cpp $(SOURCE_DIR)/utils/Utils.h | $(OUTDIR)/$(TEST_DIR)
	$(CC) $(CFLAGS) $(TEST_CFLAGS) -c $(word 1, $^) -o $@

# Compile testFileManager
$(OUTDIR)/$(TEST_DIR)/testFileManager: $(OUTDIR)/$(TEST_DIR)/testFileManager.o $(OUTDIR)/$(SOURCE_DIR)/utils/Utils.o $(OUTDIR)/$(SOURCE_DIR)/standardImage/stb_wrapper.o $(OUTDIR)/$(SOURCE_DIR)/memory/BuddyAllocator.o $(OUTDIR)/$(SOURCE_DIR)/memory/FileManager.o
	$(CC) $(CFLAGS) -o $@ $^ $(TEST_CFLAGS)

$(OUTDIR)/$(TEST_DIR)/testFileManager.o: $(TEST_DIR)/testFileManager.cpp $(SOURCE_DIR)/utils/Utils.h $(SOURCE_DIR)/memory/FileManager.h $(SOURCE_DIR)/memory/BuddyAllocator.h $(SOURCE_DIR)/standardImage/stb_image.h $(SOURCE_DIR)/standardImage/stb_image_write.h | $(OUTDIR)/$(TEST_DIR)
	$(CC) $(CFLAGS) $(TEST_CFLAGS) -c $(word 1, $^) -o $@

# Compile Source Files

# Compile main.cpp
$(OUTDIR)/$(SOURCE_DIR)/main.o: $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/memory/FileManager.h | $(OUTDIR)/$(SOURCE_DIR)
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile Utils.cpp
$(OUTDIR)/$(SOURCE_DIR)/utils/Utils.o: $(SOURCE_DIR)/utils/Utils.cpp $(SOURCE_DIR)/utils/Utils.h | $(OUTDIR)/$(SOURCE_DIR)/utils
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile BuddyAllocator.cpp
$(OUTDIR)/$(SOURCE_DIR)/memory/BuddyAllocator.o: $(SOURCE_DIR)/memory/BuddyAllocator.cpp $(SOURCE_DIR)/memory/BuddyAllocator.h $(SOURCE_DIR)/utils/Utils.h | $(OUTDIR)/$(SOURCE_DIR)/memory
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile FileManager.cpp
$(OUTDIR)/$(SOURCE_DIR)/memory/FileManager.o: $(SOURCE_DIR)/memory/FileManager.cpp $(SOURCE_DIR)/memory/FileManager.h $(SOURCE_DIR)/memory/BuddyAllocator.h $(SOURCE_DIR)/utils/Utils.h $(SOURCE_DIR)/standardImage/stb_image.h $(SOURCE_DIR)/standardImage/stb_image_write.h | $(OUTDIR)/$(SOURCE_DIR)/memory
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile stb_wrapper.cpp
$(OUTDIR)/$(SOURCE_DIR)/standardImage/stb_wrapper.o: $(SOURCE_DIR)/standardImage/stb_wrapper.cpp $(SOURCE_DIR)/standardImage/stb_image.h $(SOURCE_DIR)/standardImage/stb_image_write.h | $(OUTDIR)/$(SOURCE_DIR)/standardImage
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Create output directories if they don't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/$(TEST_DIR): | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(TEST_DIR)

$(OUTDIR)/$(SOURCE_DIR): | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)

$(OUTDIR)/$(SOURCE_DIR)/memory: | $(OUTDIR)/$(SOURCE_DIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)/memory

$(OUTDIR)/$(SOURCE_DIR)/utils: | $(OUTDIR)/$(SOURCE_DIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)/utils

$(OUTDIR)/$(SOURCE_DIR)/standardImage: | $(OUTDIR)/$(SOURCE_DIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)/standardImage

# Clean output files
clean:
	rm -rf $(OUTDIR)
