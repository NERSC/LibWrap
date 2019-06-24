GOTCHA=/global/homes/a/aalmamun/gotcha-static/install
CC = LIBRARY_PATH=$(GOTCHA)/lib64 cc
CFLAGS = -I$(GOTCHA)/include -L$(GOTCHA)/lib64 -lgotcha

SRC = libgotchah5.c

OBJ = $(SRC:.c=.o)

TARGET = libgotchah5.a

.PHONY : default all clean

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

GOTCHA_OBJS = $(gotcha.c.o gotcha_auxv.c.o gotcha_dl.c.o gotcha_utils.c.o elf_ops.c.o hash.c.o libc_wrappers.c.o library_filters.c.o tool.c.o translations.c.o libgotchah5.o)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS) 


#	gcc -shared -Wl,-soname,libgotcha.so libgotcha.so.1 libgotcha.so.1.0.1  -o libgotchah5.o

$(TARGET): $(OBJECTS)
	$(CC)  $(OBJECTS)  -o $@

GOTCHA_OBJS_2 =  $( *.o)
$(TARGET):	
	ar rcs $(TARGET) *.o

clean:
	-rm *.o
	-rm *.a
