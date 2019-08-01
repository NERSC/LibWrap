#!/bin/bash
wrapper_libdir=../mywrapper
cc -o posix_rdwt posix_rdwt.c -Wl,--wrap=read -Wl,--wrap=write  -L${wrapper_libdir} -lwrapposix_rdwt
./posix_rdwt
