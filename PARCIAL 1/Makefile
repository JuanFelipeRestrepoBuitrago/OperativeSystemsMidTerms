CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -I/usr/include/gtest
LDFLAGS = -lgtest -lgtest_main -pthread -lcrypto
MACFLAGS = -I/opt/homebrew/opt/openssl@3/include -I/opt/homebrew/opt/googletest/include -L/opt/homebrew/opt/openssl@3/lib -L/opt/homebrew/opt/googletest/lib

SOURCE_DIR = src
OUTDIR = out

all: $(OUTDIR)/rsa
compile: $(OUTDIR)/rsa

$(OUTDIR)/rsa: $(OUTDIR)/$(SOURCE_DIR)/main.o $(OUTDIR)/$(SOURCE_DIR)/helpers/FileManager.o $(OUTDIR)/$(SOURCE_DIR)/helpers/Utils.o $(OUTDIR)/$(SOURCE_DIR)/core/RSA.o $(OUTDIR)/$(SOURCE_DIR)/core/Huffman.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

TEST_DIR = src/tests/core
TEST_EXECUTABLES = $(OUTDIR)/$(TEST_DIR)/testUtils $(OUTDIR)/$(TEST_DIR)/testRSA $(OUTDIR)/$(TEST_DIR)/testHuffman


# Run All Tests
test: clean $(TEST_EXECUTABLES)
	./$(OUTDIR)/$(TEST_DIR)/testUtils
	./$(OUTDIR)/$(TEST_DIR)/testRSA
	./$(OUTDIR)/$(TEST_DIR)/testHuffman

# Run tests
testUtils: clean $(OUTDIR)/$(TEST_DIR)/testUtils
	./$(OUTDIR)/$(TEST_DIR)/testUtils

testRSA: clean $(OUTDIR)/$(TEST_DIR)/testRSA
	./$(OUTDIR)/$(TEST_DIR)/testRSA

testHuffman: clean $(OUTDIR)/$(TEST_DIR)/testHuffman
	./$(OUTDIR)/$(TEST_DIR)/testHuffman



$(OUTDIR)/$(TEST_DIR)/testUtils: $(OUTDIR)/$(TEST_DIR)/testUtils.o $(OUTDIR)/$(SOURCE_DIR)/helpers/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testUtils.cpp
$(OUTDIR)/$(TEST_DIR)/testUtils.o: $(TEST_DIR)/testUtils.cpp $(SOURCE_DIR)/helpers/Utils.h | $(OUTDIR)/$(TEST_DIR)
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@


$(OUTDIR)/$(TEST_DIR)/testRSA: $(OUTDIR)/$(TEST_DIR)/testRSA.o $(OUTDIR)/$(SOURCE_DIR)/core/RSA.o $(OUTDIR)/$(SOURCE_DIR)/helpers/Utils.o $(OUTDIR)/$(SOURCE_DIR)/helpers/FileManager.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testRSA.cpp
$(OUTDIR)/$(TEST_DIR)/testRSA.o: $(TEST_DIR)/testRSA.cpp $(SOURCE_DIR)/core/RSA.h | $(OUTDIR)/$(TEST_DIR)
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@


$(OUTDIR)/$(TEST_DIR)/testHuffman: $(OUTDIR)/$(TEST_DIR)/testHuffman.o $(OUTDIR)/$(SOURCE_DIR)/core/Huffman.o $(OUTDIR)/$(SOURCE_DIR)/helpers/FileManager.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testHuffman.cpp
$(OUTDIR)/$(TEST_DIR)/testHuffman.o: $(TEST_DIR)/testHuffman.cpp $(SOURCE_DIR)/core/Huffman.h | $(OUTDIR)/$(TEST_DIR)
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile Source Files

# Compile main.cpp
$(OUTDIR)/$(SOURCE_DIR)/main.o: $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/core/RSA.h $(SOURCE_DIR)/helpers/FileManager.h | $(OUTDIR)/$(SOURCE_DIR)
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile FileManager.cpp
$(OUTDIR)/$(SOURCE_DIR)/helpers/FileManager.o: $(SOURCE_DIR)/helpers/FileManager.cpp $(SOURCE_DIR)/helpers/FileManager.h | $(OUTDIR)/$(SOURCE_DIR)/helpers
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile Utils.cpp
$(OUTDIR)/$(SOURCE_DIR)/helpers/Utils.o: $(SOURCE_DIR)/helpers/Utils.cpp $(SOURCE_DIR)/helpers/Utils.h | $(OUTDIR)/$(SOURCE_DIR)/helpers
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile RSA.cpp
$(OUTDIR)/$(SOURCE_DIR)/core/RSA.o: $(SOURCE_DIR)/core/RSA.cpp $(SOURCE_DIR)/core/RSA.h $(SOURCE_DIR)/helpers/Utils.h $(SOURCE_DIR)/helpers/FileManager.h | $(OUTDIR)/$(SOURCE_DIR)/core
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile Huffman.cpp
$(OUTDIR)/$(SOURCE_DIR)/core/Huffman.o: $(SOURCE_DIR)/core/Huffman.cpp $(SOURCE_DIR)/core/Huffman.h $(SOURCE_DIR)/helpers/FileManager.h | $(OUTDIR)/$(SOURCE_DIR)/core
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@



# Create output directories if they don't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/$(TEST_DIR): | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(TEST_DIR)

$(OUTDIR)/$(SOURCE_DIR)/helpers: | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)/helpers

$(OUTDIR)/$(SOURCE_DIR)/core: | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)/core

$(OUTDIR)/$(SOURCE_DIR): | $(OUTDIR)
	mkdir -p $(OUTDIR)/$(SOURCE_DIR)


# Clean output files
clean:
	rm -rf $(OUTDIR)