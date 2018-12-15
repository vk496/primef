TARGET = primef
LIBS = -lgmp -lpthread
CC = g++
CFLAGS = -O3 -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = primef.o

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
