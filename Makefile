HEADER_FILES=src/*.h
SRC_FILES=src/*.c
TEST = test/test
TEST_SRC = test/test.c
CFLAGS = -std=c99 -Wall -W -O -Isrc -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lsodium
RM = rm -f

all: $(TEST)

test: $(TEST)
	./test/test

$(TEST): $(HEADER_FILES) $(SRC_FILES) $(COMBINED) $(TEST_SRC) Makefile
	$(CC) $(CFLAGS) -o $(TEST) $(TEST_SRC) $(SRC_FILES) $(COMBINED) $(LDFLAGS)

clean:
	$(RM) $(TEST)
