CC := gcc
CFLAGS := -Wall -Wextra -std=c99

SIMPLE_OBJ := simple.o slexer.o svec.o sparser.o scompiler.o svm.o

.PHONY: run
run: Simple
	./Simple

.PHONY: clean
clean:
	rm -f *.o Simple

.PHONY: commit
commit:
	git add .
	git commit
	git push

Simple: main.o $(SIMPLE_OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@