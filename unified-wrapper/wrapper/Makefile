GOTCHA=/global/homes/a/aalmamun/gotcha-static/install
GOTCHA_OBJECTS_DIR = /global/homes/a/aalmamun/gotcha/build/src/CMakeFiles/gotcha.dir
CC = LD_LIBRARY_PATH=$(GOTCHA)/lib64 cc
CFLAGS = -fPIC -I$(GOTCHA)/include -L$(GOTCHA)/lib64
LIBS = -lgotcha

SRC = libgotchah5.c


TARGET = libgotchah5.a
TARGET2 = libgotchah5.so

default: $(TARGET) $(TARGET2)
all: default


.PHONY : default all clean

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS) 


OBJS    := $(patsubst %.c,$(GOTCHA_OBJECTS_DIR)/*.o,$(SRC))


$(TARGET): $(OBJECTS)
	ar rcs $(TARGET) $(OBJS) libgotchah5.o


$(TARGET2): $(OBJECTS)
	$(CC) -shared  $(OBJECTS)  -o $@


clean:
	-rm libgotchah5.o
	-rm *.a
	-rm *.so
