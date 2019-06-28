def generate_makefile(modulename, libraries_in_make, includes_in_make):
	'''
	#FUTURE TODO: Add header string as well
	str_libs = ""
	str_libs_path = ""
	for lib_path in libs_path:
		str_libs_path = "%s-L%s" % (str_libs_path, lib_path)
	for lib in libs:
		str_libs = "%s-l%s" % (str_libs, lib)
	'''
	str_makefile = "TARGET = lwrap%s.so\n" % modulename
	str_makefile = "%s %s" %(str_makefile, 
"""
#GOTCHA=/global/homes/j/jialin/gotcha/gotcha/install
CC = cc
CFLAGS = -fPIC 
""" + 
"\nLIBRARIES = -L/$(GOTCHA)/lib64 %s\nINCLUDES = -I/usr/include -I$(GOTCHA)/include %s\n"\
				 % (libraries_in_make, includes_in_make)
+ """
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
""" )
	with open("../Makefile", "w") as f:
		f.write(str_makefile)	
