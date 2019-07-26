TARGET = lwrapposix_rdwt.so
 
CC = cc
CFLAGS = -fPIC 

LIBRARIES = -L/$(GOTCHA)/lib64 -L/usr/common/software/jansson/2.12/lib -L/global/common/cori/software/openssl/1.1.0a/hsw/lib   -l rabbitmq -l jansson -l ssl -l crypto -l dl 
INCLUDES = -I$(GOTCHA)/include -I /usr/include -I /usr/common/software/rabbitmq/0.9.0/install/include -I/usr/common/software/jansson/2.12/include 

.PHONY: default all clean
default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LIBRARIES) 

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS)  -o $@ $(INCLUDES) $(LIBRARIES)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
