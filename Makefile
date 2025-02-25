
CC = gcc
CFLAGS = -Wall -std=c11 --pedantic -Wextra

ifeq ($(OS),Windows_NT)
    EXE = main.exe
    SHARED_LIB = money_cache.dll
    SHARED_FLAGS = -shared -o $(SHARED_LIB) -Wl,--out-implib,money_cache.a
else
    EXE = main
    SHARED_LIB = money_cache.so
    SHARED_FLAGS = -shared -o $(SHARED_LIB) -fPIC
endif

OBJS = main.o money_to_string.o

all: $(EXE) $(SHARED_LIB)

$(SHARED_LIB): money_cache.c money_to_string.c money_to_string.h
	$(CC) $(CFLAGS) $(SHARED_FLAGS) money_cache.c money_to_string.c -o $(SHARED_LIB)

$(EXE): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS)

main.o: money_to_string.h
money_to_string.o: money_to_string.h

clean:
	rm -f $(EXE) $(OBJS) $(SHARED_LIB)
