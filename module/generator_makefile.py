def generate_makefile(fl_nm, modulename, libraries_in_make, includes_in_make, build):
	'''
	#FUTURE TODO: Add header string as well
	str_libs = ""
	str_libs_path = ""
	for lib_path in libs_path:
		str_libs_path = "%s-L%s" % (str_libs_path, lib_path)
	for lib in libs:
		str_libs = "%s-l%s" % (str_libs, lib)
	'''
	str_makefile = "TARGET = libwrap%s.so\nTARGET2 = libwrap%s.a\n" %(modulename,modulename)
	str_makefile = "%s %s" %(str_makefile, 
"""
ALL_OBJECTS_DIR = ALL_OBJECTS
CC = LD_LIBRARY_PATH=$(GOTCHA)/lib64 cc
CFLAGS = -fPIC -I$(GOTCHA)/include -L$(GOTCHA)/lib64
LIBS = -lgotcha

""" + 
"\nLIBRARIES = -L/$(GOTCHA)/lib64 -L$(GOTCHA)/ %s\nINCLUDES = -I/usr/include -I$(GOTCHA)/include %s\n"\
				 % (libraries_in_make, includes_in_make)
+ """
.PHONY: default all clean
""")
	if build=="both":
		str_makefile = "%s%s" % (str_makefile,"\ndefault: $(TARGET) $(TARGET2)\n")
	elif build=="static":
		str_makefile = "%s%s" %  (str_makefile,"\ndefault: $(TARGET2)\n")
	elif build=="shared":
		str_makefile = "%s%s" % (str_makefile, "\ndefault: $(TARGET)\n")
	str_makefile = "%s%s" % (str_makefile, 
"""
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

""")
	if build=="both":
		str_makefile = "%s%s" % (str_makefile, 
"""
clean:
	-rm -f *.o
	-rm -f $(TARGET2)
	-rm -f $(TARGET)
""")
	elif build == "static":
		str_makefile = "%s%s" % (str_makefile, 
"""
clean:
	-rm -f *.o
	-rm -f $(TARGET2)
""")
	elif build == "shared":
		str_makefile = "%s%s" % (str_makefile, 
"""
clean:
	-rm -f *.o
	-rm -f $(TARGET)
""")

	with open(fl_nm, "w") as f:
		f.write(str_makefile)
