CC = gcc
CFLAGS = -Wall -std=c11 --pedantic -Wextra

ifeq ($(OS),Windows_NT)
    EXE = main.exe
    SHARED_LIB = money_cache.dll
    SHARED_FLAGS = -shared -o $(SHARED_LIB) -Wl,--out-implib,money_cache.a
    LDLIBS =
else
    EXE = main
    SHARED_LIB = money_cache.so
    SHARED_FLAGS = -shared -o $(SHARED_LIB) -fPIC
    LDLIBS = -ldl
endif

OBJS = main.o money_to_string.o memoization.o
SHARED_OBJS = money_cache.o money_to_string.o memoization.o

all: $(EXE) $(SHARED_LIB)

$(SHARED_LIB): $(SHARED_OBJS)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(SHARED_OBJS) -o $(SHARED_LIB)

$(EXE): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDLIBS)

main.o: money_to_string.h memoization.h
money_to_string.o: money_to_string.h
memoization.o: memoization.h money_to_string.h
money_cache.o: money_to_string.h memoization.h

clean:
	rm -f $(EXE) $(OBJS) $(SHARED_LIB) $(SHARED_OBJS)
