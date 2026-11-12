CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
TARGET = cafepro.exe
TEST_TARGET = test_cafepro.exe

MODULES = src/product.c src/customer.c src/billing.c src/utils.c
SRC = main.c $(MODULES)
TEST_SRC = tests/test_cafepro.c $(MODULES)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC)
	.\$(TEST_TARGET)

clean:
	-del /Q $(TARGET) $(TEST_TARGET) *.o 2>nul || rm -f $(TARGET) $(TEST_TARGET) *.o
