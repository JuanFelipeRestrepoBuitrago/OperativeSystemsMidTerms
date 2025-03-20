CC = g++
CFLAGS = -Wall -Wextra -std=c++17 
MACFLAGS = -I/opt/homebrew/opt/googletest/include -L/opt/homebrew/opt/googletest/lib
TEST_CFLAGS = -I/usr/include/gtest
SOURCE_DIR = src
OUTDIR = out

# all: $(OUTDIR)/program
# compile: $(OUTDIR)/program

# $(OUTDIR)/program: $(OUTDIR)/$(SOURCE_DIR)/main.o $(OUTDIR)/$(SOURCE_DIR)/helpers/FileManager.o $(OUTDIR)/$(SOURCE_DIR)/helpers/Utils.o $(OUTDIR)/$(SOURCE_DIR)/core/RSA.o $(OUTDIR)/$(SOURCE_DIR)/core/Huffman.o
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

TEST_DIR = tests
# TEST_EXECUTABLES = $(OUTDIR)/$(TEST_DIR)/testUtils $(OUTDIR)/$(TEST_DIR)/testRSA $(OUTDIR)/$(TEST_DIR)/testHuffman

# # Run All Tests
# test: clean $(TEST_EXECUTABLES)
# 	./$(OUTDIR)/$(TEST_DIR)/testUtils
# 	./$(OUTDIR)/$(TEST_DIR)/testRSA
# 	./$(OUTDIR)/$(TEST_DIR)/testHuffman

# # Run individual tests
# testUtils: clean $(OUTDIR)/$(TEST_DIR)/testUtils
# 	./$(OUTDIR)/$(TEST_DIR)/testUtils

# # Compile testUtils
# $(OUTDIR)/$(TEST_DIR)/testUtils: $(OUTDIR)/$(TEST_DIR)/testUtils.o $(OUTDIR)/$(SOURCE_DIR)/helpers/Utils.o $(OUTDIR)/$(SOURCE_DIR)/helpers/FileManager.o
# 	$(CC) $(TEST_CFLAGS) -o $@ $^ $(LDFLAGS)

# $(OUTDIR)/$(TEST_DIR)/testUtils.o: $(TEST_DIR)/testUtils.cpp $(SOURCE_DIR)/helpers/Utils.h $(SOURCE_DIR)/helpers/FileManager.h | $(OUTDIR)/$(TEST_DIR)
# 	$(CC) $(TEST_CFLAGS) -c $(word 1, $^) -o $@


# Compile Source Files

# # Compile main.cpp
# $(OUTDIR)/$(SOURCE_DIR)/main.o: $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/core/RSA.h $(SOURCE_DIR)/helpers/FileManager.h $(SOURCE_DIR)/helpers/Utils.h | $(OUTDIR)/$(SOURCE_DIR)
# 	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# # Compile FileManager.cpp
# $(OUTDIR)/$(SOURCE_DIR)/helpers/FileManager.o: $(SOURCE_DIR)/helpers/FileManager.cpp $(SOURCE_DIR)/helpers/FileManager.h | $(OUTDIR)/$(SOURCE_DIR)/helpers
# 	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Create output directories if they don't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/$(TEST_DIR): | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(TEST_DIR)

$(OUTDIR)/$(SOURCE_DIR): | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)

# Clean output files
clean:
	rm -rf $(OUTDIR)
