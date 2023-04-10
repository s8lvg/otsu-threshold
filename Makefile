all: example

example: example.c
	gcc example.c -o example

clean:
	rm example