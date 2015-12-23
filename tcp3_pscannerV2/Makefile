CC = gcc
CFLAGS = -g -Wall

%:
	@echo "Building target..."
	$(CC) $(CFLAGS) -o $*.o -C $*.c hresolver.c

clean:
	@echo "Cleaning up.."
	rm -rf *.o
