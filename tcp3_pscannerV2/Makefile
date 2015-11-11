CC = gcc
CFLAGS = -g -Wall

%:
	@echo "Building target..."
	$(CC) $(CFLAGS) -o $*.o -C $*.c

clean:
	@echo "Cleaning up.."
	rm -rf *.o
