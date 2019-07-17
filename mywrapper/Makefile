TARGET = libwraphdf5.so
TARGET2 = libwraphdf5.a
 
ALL_OBJECTS_DIR = ALL_OBJECTS
CC = LD_LIBRARY_PATH=$(GOTCHA)/lib64 cc
CFLAGS = -fPIC -I$(GOTCHA)/include/ -L$(GOTCHA)/lib64
LIBS = -lgotcha


LIBRARIES = -L/$(GOTCHA)/lib64 -L$(GOTCHA)/ -L$(HOME)/LibWrap-Project/LibWrap/dockers/shared  -l rabbitmq -l jansson -l ssl -l crypto -l dl -l hdf5_intel -l hdf5_parallel
INCLUDES = -I/usr/include -I$(GOTCHA)/include -I /usr/include -I /usr/common/software/rabbitmq/0.9.0/install/include -I$(HOME)/projectcode/jansson-2.12/install/include 

.PHONY: default all clean
default: $(TARGET) $(TARGET2)
all: default
SRC=wrapper.c
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
ALL_OBJS := $(patsubst %.c,$(ALL_OBJECTS_DIR)/*.o,$(SRC))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LIBRARIES) 

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS)  -o $@ $(INCLUDES) $(LIBRARIES)

$(TARGET2): $(OBJECTS)
	ar rcs $(TARGET2) $(ALL_OBJS) $(OBJECTS)


clean:
	-rm -f *.o
	-rm -f *.a
	-rm -f $(TARGET)
