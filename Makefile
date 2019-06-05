TARGET = lgotchah5.so 
LIBS = -lgotcha
JANSSON=$(HOME)/projectcode/jansson-2.12
#GOTCHA=/global/homes/j/jialin/gotcha/gotcha/install
#export LD_LIBRARY_PATH=$(HOME)/projectcode/rabbitmq-c-0.9.0/install/lib:$(LD_LIBRARY_PATH)
CC = LD_LIBRARY_PATH=$(GOTCHA)/lib64 cc
CFLAGS = -fPIC -I$(GOTCHA)/include -L/$(GOTCHA)/lib64 -I$(JANSSON)/install/include 
Rabbitmq_PATH = $(HOME)/projectcode/rabbitmq-c-0.9.0
LIBRARIES = -L$(JANSSON)/install/lib -L /global/common/cori/software/openssl/1.1.0a/hsw/lib -l rabbitmq -l jansson -l ssl -l crypto -l dl 
#LIBRARIES = -L $(Rabbitmq_PATH)/install/lib64 -L $(JANSSON)/install/lib -L /global/common/cori/software/openssl/1.1.0a/hsw/lib -l rabbitmq -l jansson -l ssl -l crypto -l dl 
INCLUDES = -I /usr/include -I $(Rabbitmq_PATH)/install/include
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
