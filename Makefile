CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c99 -g 
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean
all: a.exe
	@echo "Build complete."

a.exe : $(OBJS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del -f *.o a.exe
	@echo "Clean complete."