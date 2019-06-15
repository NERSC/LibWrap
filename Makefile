TARGET = lgotchah5.so 
#JANSSON=$(HOME)/projectcode/jansson-2.12
#GOTCHA=/global/homes/j/jialin/gotcha/gotcha/install
Rabbitmq_PATH = /usr/common/software/rabbitmq/0.9.0
CC = cc
CFLAGS = -fPIC 

LIBRARIES = -L$(JANSSON)/install/lib -L/$(GOTCHA)/lib64 -L/global/common/cori/software/openssl/1.1.0a/hsw/lib -l rabbitmq -l jansson -l ssl -l crypto -l dl 
INCLUDES = -I /usr/include -I $(Rabbitmq_PATH)/install/include -I$(GOTCHA)/include -I$(JANSSON)/install/include 
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
