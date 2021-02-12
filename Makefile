CC = gcc
ifeq ($(RELEASE),1)
CFLAGS = -O2 -DNDEBUG -c
LDFLAGS = -O2 -DNDEBUG -o
else
CFLAGS = -c -g -O0 -DDEBUG -W -Wall
LDFLAGS = -O0 -DDEBUG -W -Wall -o
endif
LDLIBS = -lm
SRCS = $(notdir $(wildcard *.c))
OBJS = $(SRCS:.c=.o)
TARGET = bmputill.out
 
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET) $(OBJS) $(LDLIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) $<

run:
	./$(TARGET) -e

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -rf result