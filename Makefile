CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
TARGET = cafepro.exe
SRC = main.c src/product.c src/customer.c src/billing.c src/utils.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	-del /Q $(TARGET) *.o 2>nul || rm -f $(TARGET) *.o
