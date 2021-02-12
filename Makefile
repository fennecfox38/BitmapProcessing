CC = clang
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
RESULT = result
INTERNAL = ~
STORAGE = $(INTERNAL)/storage/shared/project/bitmap
 
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET) $(OBJS) $(LDLIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) $<

run:
	cp $(TARGET) $(INTERNAL)/$(TARGET)
	chmod 777 $(INTERNAL)/$(TARGET)
	$(INTERNAL)/$(TARGET) -e
	rm $(INTERNAL)/$(TARGET)

clean:
	rm -f *.o 
	rm -f $(TARGET)
	rm -rf result
