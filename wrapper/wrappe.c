#include <stdio.h>
#include <hdf5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

typedef hid_t (*H5Fcreate_fptr)(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);
typedef hid_t (*H5Fopen_fptr)(const char *name, unsigned flags, hid_t fapl_id);
typedef hid_t (*H5Fclose_fptr)(hid_t file_id);


int __wrap_foo(int v) {
    printf("wrap foo\n");
   // __real_foo();
     return 0;
}

int __wrap_cool(){
printf("wrap cool\n");
//__real_cool();
   return 0;
}

void __wrap_H5Fcreate(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){
	printf("wrap H5Fcreate\n");
        return  __real_H5Fcreate(name,flags,fcpl_id,fapl_id);
        //return 0;
}

int __wrap_H5Fopen(const char *name, unsigned flags, hid_t fapl_id){
        printf("wrap H5Fopen");
        return 0;
}

int __wrap_H5Fclose(hid_t file_id){
        printf("wrap H5Fclose");
        return 0;
}


