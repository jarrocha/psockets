CC = gcc
CFLAGS = -Wall
OUT= psniff
IN= psniff.c

all:
	@echo "Building target..."
	$(CC) $(CFLAGS) -o $(OUT) -C $(IN) hello.c
debug:
	@echo "Building for debugging"
	$(CC) $(CFLAGS) -g -o $(OUT) -C $(IN)
clean:
	@echo "Cleaning up.."
	rm -rf $(OUT)
