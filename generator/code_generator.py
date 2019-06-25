
from __future__ import print_function
import sys

class func:

	def __init__(self, prototype):
		l = prototype.strip().split('(')
		x = filter(None, l[0].split(' '))
		self.ret_type = ' '.join(x[0:-1])
		self.name = x[-1]
		self.arg_string = l[1].strip(")")
		# set up useful vars
		self.name_quote = "\"" + self.name + "\""
		self.wraper = self.name
		self.wrapee =  self.name
		self.string_param_names = ""
		# param list of tuples: (type, name)
		self.list_params = []
		params = filter(None, self.arg_string.split(','))
		for y in params:
			param_name = y.split(' ')[-1].strip().strip('*')
			param_type = y[0:-1*len(param_name)].strip()
			self.string_param_names += param_name + ", "
			self.list_params.append((param_type, param_name))
		self.string_param_names = self.string_param_names[:-2]

	def __str__(self):
		return self.ret_type + ' ' + self.name + ' (' + self.arg_string + ')'

def read_functions_file(filename):
	functions = []
	f = open(filename, 'r')
	for line in f:
		functions.append(func(line))
	f.close()
	return functions

def read_libs_file(filename):
        functions = []
        f = open(filename, 'r')
        for line in f:
                functions.append(func(line))
        f.close()
        return functions

def write_wrapper_file(filename, func_list, lib_list, modulename):
        """static gotcha_wrappee_handle_t WRAP_Orig_Func_handle;
static int (*WRAP_Orig_Func) (int param);
static int dissectio_Orig_Func (int param){
	printf("Orig_Func\n");
        WRAP_Orig_Func = gotcha_get_wrappee(WRAP_Orig_Func_handle);
        return WARP_Orig_Func ? (WRAP_Orig_Func(param) ) : 0;
}
"""
	m = modulename.split('_')[0]
	f = open(filename, 'w')
	# write header
	f.write("#include <"+m+".h>\n")
	f.write("#include <stdio.h>\n")
	f.write("#include <stdlib.h>\n")
	f.write("#include <unistd.h>\n");
	f.write("#include <time.h>\n");
	f.write("#include \"gotcha/gotcha.h\"\n");
	f.write("#include <pthread.h>\n");
	f.write("#include <mpi.h>\n");
	f.write("#include <hdf5.h>\n");
	f.write("#include <sys/types.h>\n");
	f.write("#include <sys/stat.h>\n");
	f.write("#include <fcntl.h>\n");
	f.write("#include \"log_init.h\"\n");
	f.write("#include \"log.h\"\n");
	f.write("\n#define NFUNCS " + str(len(func_list)) + "\n\n")
	f.write("\n")


	
	
		
	# write each function wrapper
	for function in func_list:
		#write static wrapper
		f.write( function.ret_type + " "+"__wrap_" + function.wraper + "(" + function.arg_string + "){\n")
                f.write("\treturn __real_" + function.wrapee +  "(" + function.string_param_names + "));\n")
                f.write("}\n\n")
		
		#write shared wrapper
		f.write("static gotcha_wrappee_handle_t " + function.wrapee + "_handle;\n")
                f.write("static " + function.ret_type + " (*" + function.wrapee + ")(" + function.arg_string + ");\n")
                f.write("static " + function.ret_type + " " + function.wraper + "_wrapper(" + function.arg_string + "){\n")
                f.write("\t" + function.wrapee + " = gotcha_get_wrappee(" + function.wrapee  + "_handle);\n" )
                f.write("\treturn " + function.wrapee + " ? (" + function.wrapee + "(" + function.string_param_names + ")) : 0;\n")
                f.write("}\n\n")

		#write log routine
		f.write("void "+ function.wrapee + "_log(" + function.arg_string + "){\n")
		f.write("\tprintf(\"This is log\");\n")
		f.write("}\n\n")
		
		#write common routine
		#f.write("void "+ function.wrapee + "_common(" + function.arg_string + "){\n")
                #f.write("\tt_start = clock();\n")
		#f.write("\t")
                #f.write("}\n\n")

        f.write("\n")

	
	
	# write map struct
        f.write("struct gotcha_binding_t wrap_" + modulename + " [] = {\n")
        for function in func_list:
                f.write("\t{ " + function.name_quote + ", " + function.wraper + ", &" + function.wrapee + " },\n")
        f.write("};\n\n")
        
	# write initializer
        f.write("void init_gotcha_" + modulename + "(){\n")
        f.write("\tgotcha_wrap(wrap_"+ modulename +", NFUNCS, \"wrapper\");\n")
        f.write("}\n")
        
	#constructor
	f.write("__attribute__((constructor)) void construct_me(){\n")
	f.write("\tinit_gotcha_"+modulename + "();\n")
	f.write("}\n")
	f.close()


def main(modulename,lib):
	functions = read_functions_file(modulename+".txt")
	libs = read_libs_file(lib+".txt")
	write_wrapper_file("wrapper_"+modulename+".c", functions, libs, modulename)


if __name__ == "__main__":
	if len(sys.argv) < 3:
		print("Usage: python", sys.argv[0], sys.argv[1], "modulename", "lib")
	else:
		main(sys.argv[1],sys.argv[2])
