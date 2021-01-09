
�
�this is a test file
                                                                                awesome file!

newline?!?!?

                                                                       SRC  := ./src
BIN  := ./bin
EXEC := $(BIN)/tfs.out
MAIN := ./cli.c

FLAGS   := -Wall -g
LIBS    := -lm -L/usr/local/lib -lprotobuf-c
INCLUDE := -I/usr/local/include

SOURCES := $(shell find $(SRC) -name '*.c')
HEADERS := $(shell find $(SRC) -name '*.h')

PROTO := ./proto/build/
PROTO_SOURCES := $(shell find $(PROTO) -name '*.c')
PROTO_HEADERS := $(shell find $(PROTO) -name '*.h')

INCLUDE_DIRS := -I$(SRC) -I.
# INCLUDE_DIRS := -I$(PROTO) -I$(SRC)
tfs: $(SOURCES) $(HEADERS) $(PROTO_HEADERS) $(MAIN)
	mkdir -p $(BIN)
	gcc $(FLAGS) -o $(EXEC) $(INCLUDE_DIRS) $(SOURCES) $(MAIN) $(PROTO_SOURCES) $(LIBS) $(INCLUDE)

# ----- TESTS ----- #

test:
	make clean
	make test_main      && ./memtest.sh ./bin/tests/test_main.out
	make test_serialize && ./memtest.sh ./bin/tests/test_serialize.out

test_main: ./tests/test_main.c $(SOURCES) $(HEADERS) $(PROTO_HEADERS)
	mkdir -p $(BIN)/tests/
	gcc $(FLAGS) -o $(BIN)/tests/$@.out $(INCLUDE_DIRS) $(SOURCES) $< $(PROTO_SOURCES) $(LIBS) $(INCLUDE)

test_serialize: ./tests/test_serialize.c $(SOURCES) $(HEADERS) $(PROTO_HEADERS)
	mkdir -p $(BIN)/tests/
	gcc $(FLAGS) -o $(BIN)/tests/$@.out $(INCLUDE_DIRS) $(SOURCES) $< $(PROTO_SOURCES) $(LIBS) $(INCLUDE)

clean:
	rm -rf bin/*
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       [
9

testfile.txt

testfile2.txtd

Makefile�	�
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 