CC := gcc
CFLAGS := -Wall -Werror -Wextra -std=c99

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

Simple: main.o simple.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@