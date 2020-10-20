
from __future__ import print_function
import sys


def generate_pkg(filename, func_list, lib_list, lib_path, modulename):

    wrapper_libdir=lib_path

    f = open(filename, 'w')

    f.write("wrapper_prefix="+lib_path+"\n")
    f.write("exec_prefix=${wrapper_prefix}\n")
    f.write("wrapper_libdir="+lib_path+"\n")

    f.write("Name:wrapper-config\n")
    f.write("Description: the static wrapper\n")
    f.write("Version: 1\n")
    f.write("\n")
    # write each function wraper and wrapee
    f.write("Libs:")
    for function in func_list:
            f.write( "-Wl,--wrap="+function.name+" ")
    f.write(" "+"-L${"+wrapper_libdir+"}"+" ")
    for lib in lib_list:
            f.write( "-l{"+lib+"}"+" ")
    f.write( "-lwrap"+modulename+" ")
    f.write("\n\n")
    f.write("Libs.private:")
    for function in func_list:
            f.write( "-Wl,--wrap="+function.name+" ")
    f.write("\n")
    f.close()



