CC=gcc
CFLAGS=-g -Wall -Werror -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -I.
LDFLAGS=-lcunit
OBJ=main.o display.o engine.o keyboard.o 
TEST_OBJ=TestCUNIT/test_all.o
EXEC=launchMe
TEST_EXEC=run_tests

all: $(EXEC)
$(EXEC): $(OBJ) 
	$(CC) $(OBJ) $(LDFLAGS) -o $(EXEC)
	
test: $(TEST_EXEC)
	./$(TEST_EXEC)


$(TEST_EXEC): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) $(LDFLAGS) -o $(TEST_EXEC)
main.o: main.c
	$(CC) -c main.c $(CFLAGS) -o main.o

display.o: display.c display.h
	$(CC) -o display.o -c display.c $(CFLAGS)

engine.o: engine.c engine.h
	$(CC) -o engine.o -c engine.c $(CFLAGS)

keyboard.o: keyboard.c keyboard.h
	$(CC) -o keyboard.o -c keyboard.c $(CFLAGS)

TestCUNIT/test_all.o: TestCUNIT/test_all.c
	$(CC) -c TestCUNIT/test_all.c $(CFLAGS) -o TestCUNIT/test_all.o


doc:
	doxygen Doxyfile

clean:
	rm -f  $(OBJ) TestCUNIT/*.o

mrproper: clean
	rm -rf $(EXEC) $(TEST_EXEC)
	rm -rf docs/

