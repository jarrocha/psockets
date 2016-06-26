CC = gcc
CFLAGS = -Wall
OUT= get_http
IN= get_http.c

all:
		@echo "Building target..."
			$(CC) $(CFLAGS) -o $(OUT) -C $(IN)
debug:
		@echo "Building for debugging"
			$(CC) $(CFLAGS) -g -o $(OUT) -C $(IN)
clean:
		@echo "Cleaning up.."
			rm -rf $(OUT)
