TARGET = libgotchah5.so 
LIBS = -lgotcha
#GOTCHA=/global/homes/j/jialin/gotcha/gotcha/install
CC = LD_LIBRARY_PATH=$(GOTCHA)/lib64 cc
CFLAGS = -fPIC -I$(GOTCHA)/include -L/$(GOTCHA)/lib64

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS)  -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
