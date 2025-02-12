CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -I/usr/include/gtest
LDFLAGS = -lgtest -lgtest_main -pthread -lcrypto
OUTDIR = out
SOURCE = src

all: test

# Run All Tests
test: clean testUtils testRSA

# Run tests
testUtils: $(OUTDIR)/tests/testUtils
	./$(OUTDIR)/tests/testUtils

testRSA: $(OUTDIR)/tests/testRSA
	./$(OUTDIR)/tests/testRSA

# Linking the test executable
$(OUTDIR)/tests/testUtils: $(OUTDIR)/tests/testUtils.o $(OUTDIR)/helpers/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testUtils.cpp
$(OUTDIR)/tests/testUtils.o: $(SOURCE)/tests/testUtils.cpp | $(OUTDIR)/tests
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

$(OUTDIR)/tests/testRSA: $(OUTDIR)/tests/testRSA.o $(OUTDIR)/core/RSA.o $(OUTDIR)/helpers/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testRSA.cpp
$(OUTDIR)/tests/testRSA.o: $(SOURCE)/tests/testRSA.cpp | $(OUTDIR)/tests
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile Utils.cpp
$(OUTDIR)/helpers/Utils.o: $(SOURCE)/helpers/Utils.cpp $(SOURCE)/helpers/Utils.h | $(OUTDIR)/helpers
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile RSA.cpp
$(OUTDIR)/core/RSA.o: $(SOURCE)/core/RSA.cpp $(SOURCE)/core/RSA.h | $(OUTDIR)/core
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Create output directories if they don't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/tests:
	mkdir -p $(OUTDIR)/tests

$(OUTDIR)/helpers:
	mkdir -p $(OUTDIR)/helpers

$(OUTDIR)/core:
	mkdir -p $(OUTDIR)/core

# Clean output files
clean:
	rm -rf $(OUTDIR)