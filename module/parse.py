import sys

from generator_wrapper import generate_wrapper
from generator_loginit import generate_log_init
from generator_makefile import generate_makefile

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
	functions, log_wrap_functions = [], []
	include_headers, libraries = [], []
	lib_in_make, inclds_in_make = "", ""
	mode = ""
	with (open(filename, 'r')) as f:
		for line in f:
			if line.startswith("#"): continue
			if not line.strip("\n"):
				continue
			if (line.strip("\n") == "FUNCTIONS" or line.strip("\n") == "LIBRARY"\
						 or line.strip("\n") == "LOG_MPI_FINALIZE"\
						 or line.strip("\n") == "USER_WRAP_FILE"\
						 or line.strip("\n") == "LOG_ATEXIT"\
						 or line.strip("\n") == "INCLUDE_HEADERS"\
						 or line.strip("\n") == "LIBRARIES_IN_MAKE"\
						 or line.strip("\n") == "INCLUDES_IN_MAKE"): 
				mode = line.strip("\n");
				continue	
			if mode=="FUNCTIONS":
				try:	
					[func_nm, func_wrap] = line.split(":")
					functions.append(func(func_nm))
					if func_wrap.strip("\n"):
						func_wrap = func_wrap.lstrip()
						log_wrap_functions.append(func_wrap.strip("\n"))
					else:
						print "wrap for function %s not given" % \
										 func_nm.name
				except ValueError:
					functions.append(func(line.strip("\n")))
					print "wrap for function %s not given" % functions[-1].name
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
	log_mpi_finalize_fn, log_atexit_fn = map(addparasemicolon,\
			 [log_mpi_finalize_fn, log_atexit_fn])
	if len(log_wrap_functions) != len(functions):
		log_wrap_functions.extend([""]*(len(functions)-len(log_wrap_functions)))
	return functions, log_wrap_functions, libraries, log_mpi_finalize_fn,\
			log_file_nm, log_atexit_fn, include_headers, lib_in_make,\
								inclds_in_make


def main(modulename):
	functions, log_wrap_functions, libraries, log_mpi_finalize_fn,\
				log_file_nm, log_atexit_fn, include_headers, \
				lib_in_make, inclds_in_make \
						 = read_config_file(modulename+".config")
	#write_gotcha_file("gotcha_"+modulename+".c", functions, modulename)
	generate_log_init(log_atexit_fn, log_mpi_finalize_fn, log_file_nm)
	generate_wrapper("../wrapper.c", functions, modulename, log_file_nm, include_headers,\
									log_wrap_functions)
	generate_makefile(modulename, lib_in_make, inclds_in_make)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Usage: python", sys.argv[0], "modulename")
	else:
		main(sys.argv[1])
