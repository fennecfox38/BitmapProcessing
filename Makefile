CC = gcc
CFLAGS = -c -g
#LDFLAGS = <링크 옵션>
#LDLIBS = <링크 라이브러리 목록>
OBJS = main.o ascii.o bitmap.o gray.o invert.o mirror.o
TARGET = Bitmap

run: $(TARGET)
	.\$(TARGET)

clean:
	del *.o
	del $(TARGET).exe
 
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o : main.c
	$(CC) $(CFLAGS) main.c 

ascii.o : ascii.c
	$(CC) $(CFLAGS) ascii.c 

bitmap.o : bitmap.c
	$(CC) $(CFLAGS) bitmap.c

gray.o : gray.c
	$(CC) $(CFLAGS) gray.c 

invert.o : invert.c
	$(CC) $(CFLAGS) invert.c 

mirror.o : mirror.c
	$(CC) $(CFLAGS) mirror.c 