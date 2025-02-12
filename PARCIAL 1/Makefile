CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -I/usr/include/gtest
LDFLAGS = -lgtest -lgtest_main -pthread -lcrypto
OUTDIR = out

all: $(OUTDIR)/tests/testUtils

# Run All Tests
test: testRSA testUtils

# Run tests
testUtils: clean $(OUTDIR)/tests/testUtils
	./$(OUTDIR)/tests/testUtils

testRSA: clean $(OUTDIR)/tests/testRSA
	./$(OUTDIR)/tests/testRSA

# Linking the test executable
$(OUTDIR)/tests/testUtils: $(OUTDIR)/tests/testUtils.o $(OUTDIR)/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testUtils.cpp
$(OUTDIR)/tests/testUtils.o: tests/testUtils.cpp | $(OUTDIR)/tests
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

$(OUTDIR)/tests/testRSA: $(OUTDIR)/tests/testRSA.o $(OUTDIR)/RSA.o $(OUTDIR)/Utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile testRSA.cpp
$(OUTDIR)/tests/testRSA.o: tests/testRSA.cpp | $(OUTDIR)/tests
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile Utils.cpp
$(OUTDIR)/Utils.o: Utils.cpp Utils.h | $(OUTDIR)
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Compile RSA.cpp
$(OUTDIR)/RSA.o: RSA.cpp RSA.h | $(OUTDIR)
	$(CC) $(CFLAGS) -c $(word 1, $^) -o $@

# Create output directories if they don't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/tests:
	mkdir -p $(OUTDIR)/tests

# Clean output files
clean:
	rm -rf $(OUTDIR)