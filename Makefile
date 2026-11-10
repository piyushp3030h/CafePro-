CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
TARGET = cafepro.exe
SRC = main.c $(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	-del /Q $(TARGET) *.o 2>nul || rm -f $(TARGET) *.o

