import sys
import os

sys.path.append('./module')

from generator_wrapper import generate_wrapper
from generator_loginit import generate_log_init
from generator_makefile import generate_makefile
from generator_joblog import generate_job_log_info
from generator_pkg import generate_pkg 
from generator_static_obj import generate_static_obj

def writable(dir_nm, fl_nm):
	if not os.path.exists(dir_nm):
		os.mkdir(dir_nm)
	return os.path.join(dir_nm, fl_nm)


class func:
	def __init__(self, prototype):
		l = prototype.strip().split('(')
		x = filter(None, l[0].split(' '))
		self.ret_type = ' '.join(x[0:-1])
		self.name = x[-1]
		self.arg_string = l[1].strip(")")
		# set up useful vars
		self.name_quote = "\"" + self.name + "\""
		self.wraper = self.name + "_wrapper"
		self.wrapee = self.name + "_wrappee"
		self.string_param_names = ""
		# param list of tuples: (type, name)
		self.list_params = []
		params = filter(None, self.arg_string.split(','))
		for y in params:
			y=y.rstrip()
			param_name = y.split(' ')[-1].strip().strip('*')
			param_type = y[0:-1*len(param_name)].strip()
			self.string_param_names += param_name + ", "
			self.list_params.append((param_type, param_name))
		self.string_param_names = self.string_param_names[:-2]

	def __str__(self):
		return self.ret_type + ' ' + self.name + ' (' + self.arg_string + ')'


def read_config_file(filename):
	def addparasemicolon(fn_nm):
		if fn_nm:
			fn_nm = "%s();" % fn_nm
		return fn_nm
	log_atexit_fn, log_mpi_finalize_fn = "", ""
	log_file_nm = ""
	functions, log_wrap_functions, log_pre_wrap_functions = [], [], []
	include_headers, libraries, static_libs  = [], [], []
	#static_libs = []
	lib_in_make, inclds_in_make = "", ""
	wrapper_mode = ""
	mode = ""
	out_dir = ""
	with (open(filename, 'r')) as f:
		for line in f:
			if line.startswith("#"): continue
			if not line.strip("\n"):
				continue
			if (line.strip("\n") == "MODE" or line.strip("\n") == "FUNCTIONS"\
						 or line.strip("\n") == "LIBRARY"\
						 or line.strip("\n") == "LOG_MPI_FINALIZE"\
						 or line.strip("\n") == "USER_WRAP_FILE"\
						 or line.strip("\n") == "LOG_ATEXIT"\
						 or line.strip("\n") == "INCLUDE_HEADERS"\
						 or line.strip("\n") == "LIBRARIES_IN_MAKE"\
						 or line.strip("\n") == "INCLUDES_IN_MAKE"\
						 or line.strip("\n") == "STATIC_LIBRARIES" 
						 or line.strip("\n") == "OUTPUT_DIR"):
				mode = line.strip("\n");
				continue	
			if mode=="MODE":
				wrapper_mode=line.strip("\n");
			if mode=="FUNCTIONS":
					lst_col_split = line.strip("\n").split(":")
					if len(lst_col_split)==2:
						[func_nm, func_wrap] = lst_col_split
						log_pre_wrap_functions.append("")
					elif len(lst_col_split)==3:
						[func_nm, func_wrap, func_pre_wrap] = lst_col_split
						log_pre_wrap_functions.append(func_pre_wrap.lstrip())
					elif len(lst_col_split)==1:
						functions.append(func(line.strip("\n")))
						print "wrap for function \"%s\" not given" % \
									functions[-1].name
						log_wrap_functions.append("")
						log_pre_wrap_functions.append("")
						continue
					functions.append(func(func_nm))
					if func_wrap.strip("\n"):
						func_wrap = func_wrap.lstrip()
						log_wrap_functions.append(func_wrap.strip("\n"))
					#this will happen when no postlog but prelog is given
					#or when only function name is given and no log
					else:	
						log_wrap_functions.append("")
						
			elif mode=="LIBRARY":
				libraries.append(line.strip("\n"))
			elif mode == "LOG_MPI_FINALIZE":
				log_mpi_finalize_fn = line.strip("\n");
			elif mode == "USER_WRAP_FILE":
				log_file_nm = line.strip("\n");
			elif mode == "LOG_ATEXIT":
				log_atexit_fn = line.strip("\n");
			elif mode == "INCLUDE_HEADERS":
				include_headers.append(line.strip("\n"));
			elif mode == "LIBRARIES_IN_MAKE":
				lib_in_make = line.strip("\n")
			elif mode == "INCLUDES_IN_MAKE":
				inclds_in_make = line.strip("\n")
			elif mode == "STATIC_LIBRARIES":
                                static_libs.append(line.strip("\n"))
			elif mode == "OUTPUT_DIR":
				out_dir = line.strip("\n")
	log_mpi_finalize_fn, log_atexit_fn = map(addparasemicolon,\
			 [log_mpi_finalize_fn, log_atexit_fn])
	if len(log_wrap_functions) != len(functions):
		log_wrap_functions.extend([""]*(len(functions)-len(log_wrap_functions)))
	return functions, log_wrap_functions, log_pre_wrap_functions, libraries,\
		 log_mpi_finalize_fn, log_file_nm, log_atexit_fn, include_headers,\
			lib_in_make, inclds_in_make, out_dir, static_libs, wrapper_mode


def main(modulename):
	if modulename.endswith(".config"):
		filename = modulename
		modulename = modulename.replace(".config","") 
	else:
		filename = modulename + ".config"
	functions, log_wrap_functions, log_pre_wrap_functions, libraries, \
		log_mpi_finalize_fn,log_file_nm, log_atexit_fn, include_headers, \
				lib_in_make, inclds_in_make, out_dir, static_libs ,wrapper_mode \
						 = read_config_file(filename)
	generate_log_init(writable(out_dir, "log_init"), log_atexit_fn, log_mpi_finalize_fn, log_file_nm)
	generate_wrapper(writable(out_dir, "wrapper.c"), functions, modulename, log_file_nm,\
					include_headers, log_wrap_functions, wrapper_mode,
									log_pre_wrap_functions)
	generate_makefile(writable(out_dir, "Makefile"), modulename, lib_in_make, inclds_in_make, \
										wrapper_mode)
	generate_job_log_info(writable(out_dir, "log_job_info"))
	generate_pkg(writable(out_dir, "wrapper-config.pc"), functions, libraries, out_dir, modulename)
	generate_static_obj(writable(out_dir, "static-object-generator.sh"), static_libs)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Usage: python", sys.argv[0], "modulename")
	else:
		main(sys.argv[1])
