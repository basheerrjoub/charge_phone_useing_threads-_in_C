all: threads

threads: threads.c
	gcc -o threads threads.c -lpthread


run:
	./threads

clean:
	rm -f threads
