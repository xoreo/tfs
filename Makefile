SRC  := ./src
BIN  := ./bin
EXEC := $(BIN)/tfs.out

FLAGS   := -Wall -g
LIBS    := -lm -L/usr/local/lib -lprotobuf-c
INCLUDE := -I/usr/local/include

SOURCES := $(shell find $(SRC) -name '*.c')
HEADERS := $(shell find $(SRC) -name '*.h')

PROTO := ./proto/build/
PROTO_SOURCES := $(shell find $(PROTO) -name '*.c')
PROTO_HEADERS := $(shell find $(PROTO) -name '*.h')

INCLUDE_DIRS := -I$(PROTO) -I$(SRC)
tfs: $(SOURCES) $(HEADERS) $(PROTO_HEADERS)
	mkdir -p $(BIN)
	gcc $(FLAGS) -o $(EXEC) $(INCLUDE_DIRS) $(SOURCES) $(PROTO_SOURCES) $(LIBS) $(INCLUDE)

# TESTS
test_ftable: ./src/util.c ./src/ftable.c ./tests/ftable.c
	mkdir -p $(BIN)/tests/
	gcc -o $(BIN)/tests/$@.out $(FLAGS) $^ $(LIBS)

clean:
	rm -rf bin/*