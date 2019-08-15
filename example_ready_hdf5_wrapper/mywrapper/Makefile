TARGET = libwraphdf5.so
TARGET2 = libwraphdf5.a
 
GOTCHA_DIR = ../gotcha_utils
ALL_OBJECTS_DIR = ALL_OBJECTS
CC = cc  
CFLAGS = -fPIC -I$(GOTCHA_DIR)/include 



LIBRARIES = -L/global/common/cori_cle6/software/jansson/2.12/lib -L/global/common/cori_cle6/software/rabbitmq/0.9.0/lib64 -l rabbitmq -l jansson -l ssl -l crypto -l dl 
INCLUDES = -I/usr/include -I$(GOTCHA_DIR)/include -I /usr/include -I /global/common/cori_cle6/software/rabbitmq/0.9.0/include -I/global/common/cori_cle6/software/jansson/2.12/include 

.PHONY: default all clean

default: $(TARGET) $(TARGET2)

all: default
SRC=wrapper.c

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
ALL_OBJS := $(patsubst %.c,$(ALL_OBJECTS_DIR)/*.o,$(SRC))
GOTCHA_OBJS := $(patsubst %.c,$(GOTCHA_DIR)/gotcha_util.o,$(SRC))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LIBRARIES) 

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(GOTCHA_OBJS)  -o $@ $(INCLUDES) $(LIBRARIES)

$(TARGET2): $(OBJECTS)
	ar rcs $(TARGET2) $(ALL_OBJS) $(GOTCHA_OBJS)  $(OBJECTS)


clean:
	-rm -f *.o
	-rm -f $(TARGET2)
	-rm -f $(TARGET)
