CFLAG += -Wall

EXE := ../../bin/rvsim
OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LFLAG) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAG) -c $<

clean:
	$(shell rm -f *.o)