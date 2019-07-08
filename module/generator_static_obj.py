
from __future__ import print_function
import sys


def generate_static_obj(filename, lib_list):
	

	
	f = open(filename, 'w')
	f.write("mkdir ALL_OBJECTS\n")
	f.write("cd ALL_OBJECTS\n")
		
        for lib in lib_list:
                f.write( "ar -xv "+lib+"\n")
        f.write("\n")
        f.close()



